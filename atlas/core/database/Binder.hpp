//
// Created by kj16609 on 6/23/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_BINDER_HPP
#define ATLASGAMEMANAGER_BINDER_HPP

#include <sqlite3.h>
#include <string>

#include "Database.hpp"
#include "FunctionDecomp.hpp"
#include "binders.hpp"
#include "core/exceptions.hpp"
#include "core/logging/logging.hpp"
#include "extractors.hpp"

template < typename T >
concept has_value = requires( T& t ) {
	{
		t.value()
	} -> std::same_as< typename T::value_type& >;
};

template < typename T >
concept has_value_check = requires( T& t ) {
	{
		t.has_value()
	} -> std::same_as< bool >;
};

template < typename T >
concept has_get = requires( T& t ) {
	{
		std::get< 0 >( t )
	};
};

template < typename T > concept is_tuple = has_get< std::remove_reference_t< T > >;

template < typename T >
concept is_optional = has_value< std::remove_reference_t< T > > && has_value_check< std::remove_reference_t< T > >;

class Binder
{
	sqlite3_stmt* stmt { nullptr };
	int param_counter { 0 };
	int max_param_count { 0 };
	bool ran { false };

	Q_DISABLE_COPY_MOVE( Binder )

  public:

	Binder() = delete;

	Binder( const std::string_view sql );

	template < typename T >
	// Why was this here? What reason did I put it here for?
	//requires( !std::is_same_v< std::remove_reference_t< T >, std::string > )
	Binder& operator<<( T t )
	{
		if ( param_counter > max_param_count )
		{
			throw DatabaseException( format_ns::format(
				"param_counter > param_count = {} > {} for query \"{}\"",
				param_counter,
				sqlite3_bind_parameter_count( stmt ),
				std::string( sqlite3_sql( stmt ) ) ) );
		}

		switch ( bindParameter< std::remove_reference_t< T > >( stmt, std::move( t ), ++param_counter ) )
		{
			case SQLITE_OK:
				break;
			default:
				{
					throw DatabaseException( format_ns::format(
						"Failed to bind to \"{}\": Reason: \"{}\"",
						sqlite3_sql( stmt ),
						sqlite3_errmsg( &Database::ref() ) ) );
				}
		}

		return *this;
	}

	template < typename T >
		requires( (!is_optional< T >) && (!is_tuple< T >))
	void operator>>( T& t )
	{
		std::optional< std::tuple< T > > tpl;

		executeQuery( tpl );

		if ( tpl.has_value() ) t = std::move( std::get< 0, T >( tpl.value() ) );
	}

	template < typename T >
		requires( !is_optional< T > && (!is_tuple< T >))
	void operator>>( std::optional< T >& t )
	{
		std::optional< std::tuple< T > > tpl;

		executeQuery( tpl );

		if ( tpl.has_value() )
			t = std::move( std::get< 0, T >( tpl.value() ) );
		else
			t = std::nullopt;
	}

	template < typename Function >
		requires( (!is_optional< Function >) && (!is_tuple< Function >))
	void operator>>( Function&& func )
	{
		using FuncArgs = FunctionDecomp< Function >;
		using Tpl = FuncArgs::ArgTuple;

		std::optional< Tpl > opt_tpl { std::nullopt };
		executeQuery( opt_tpl );

		while ( opt_tpl.has_value() )
		{
			std::apply( func, std::move( opt_tpl.value() ) );
			executeQuery( opt_tpl );
		}
	}

	template < typename... Ts >
		requires( !( is_optional< Ts > && ... ) ) && ( !( is_tuple< Ts > && ... ) )
	void operator>>( std::tuple< Ts... >& tpl )
	{
		ran = true;

		std::optional< std::tuple< Ts... > > opt_tpl { std::nullopt };
		executeQuery( opt_tpl );

		if ( opt_tpl.has_value() )
		{
			tpl = std::move( opt_tpl.value() );
		}
		return;
	}

  private:

	template < typename... Ts >
		requires( !( is_optional< Ts > || ... ) && !( is_tuple< Ts > || ... ) )
	void executeQuery( std::optional< std::tuple< Ts... > >& tpl_opt )
	{
		if ( param_counter != max_param_count )
			throw AtlasException( format_ns::format(
				"Not enough parameters given for query! Given {}, Expected {}. param_counter != max_param_count = {} != {} for query \"{}\"",
				param_counter,
				max_param_count,
				param_counter,
				max_param_count,
				std::string( sqlite3_sql( stmt ) ) ) );

		ran = true;

		if ( stmt == nullptr ) throw DatabaseException( "stmt was nullptr" );

		atlas::logging::debug( "Executing query {}", stmt );

		const auto step_ret { sqlite3_step( stmt ) };

		switch ( step_ret )
		{
			case SQLITE_ROW:
				[[likely]]
				{
					if constexpr ( sizeof...( Ts ) > 0 )
					{
						std::tuple< Ts... > tpl;
						extractRow< 0, Ts... >( stmt, tpl );
						tpl_opt = std::move( tpl );
						return;
					}
					else
					{
						tpl_opt = std::nullopt;
						return;
					}
				}
			case SQLITE_DONE:
				{
					atlas::logging::debug( "Finished query {}", sqlite3_expanded_sql( stmt ) );
					tpl_opt = std::nullopt;
					return;

					default:
						[[fallthrough]];
					case SQLITE_MISUSE:
						[[fallthrough]];
					case SQLITE_BUSY:
						[[fallthrough]];
					case SQLITE_ERROR:
						{
							throw AtlasException( format_ns::format(
								"DB: Query error: \"{}\", Query: \"{}\"",
								sqlite3_errmsg( &Database::ref() ),
								sqlite3_expanded_sql( stmt ) ) );
						}
				}
		}
	}

  public:

	~Binder() noexcept( false );
};

#endif //ATLASGAMEMANAGER_BINDER_HPP
