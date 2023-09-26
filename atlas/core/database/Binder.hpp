//
// Created by kj16609 on 6/23/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_BINDER_HPP
#define ATLASGAMEMANAGER_BINDER_HPP

#include <concepts>
#include <sqlite3.h>
#include <string>

#include "Database.hpp"
#include "FunctionDecomp.hpp"
#include "binders.hpp"
#include "core/exceptions.hpp"
#include "core/logging/logging.hpp"
#include "extractors.hpp"

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
		requires( !std::is_same_v< std::remove_reference_t< T >, std::string > )
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
		requires( !std::is_reference_v< T > )
	void operator>>( T& t )
	{
		ran = true;
		const auto step_ret { sqlite3_step( stmt ) };

		if ( param_counter != max_param_count )
			throw AtlasException( format_ns::format(
				"param_counter != max_param_count = {} != {} for query \"{}\"",
				param_counter,
				max_param_count,
				std::string( sqlite3_sql( stmt ) ) ) );

		switch ( step_ret )
		{
			case SQLITE_ROW:
				[[likely]]
				{
					extract< 0, T >( stmt, t );
					return;
				}
			case SQLITE_DONE:
				{
					return;
				}
			default:
				[[fallthrough]];
			case SQLITE_MISUSE:
				[[fallthrough]];
			case SQLITE_BUSY:
				[[fallthrough]];
			case SQLITE_ERROR:
				{
					throw DatabaseException( format_ns::format(
						"Query error: \"{}\", Query: \"{}\"",
						sqlite3_errmsg( &Database::ref() ),
						sqlite3_expanded_sql( stmt ) ) );
				}
		}
	}

	template < typename Function >
	void operator>>( Function&& func )
	{
		using FuncArgs = FunctionDecomp< Function >;
		using Tpl = FuncArgs::ArgTuple;
		ran = true;

		//Execute the query.
		while ( true )
		{
			if ( stmt == nullptr ) throw DatabaseException( "stmt was nullptr" );

			const auto result { sqlite3_step( stmt ) };

			switch ( result )
			{
				case SQLITE_ROW:
					[[likely]]
					{
						Tpl tpl;
						extractRow< 0 >( stmt, tpl );
						std::apply( func, std::move( tpl ) );
						break;
					}
				case SQLITE_DONE:
					return;
				case SQLITE_MISUSE:
					[[fallthrough]];
				case SQLITE_BUSY:
					[[fallthrough]];
				case SQLITE_ERROR:
					{
						throw DatabaseException( format_ns::format(
							"Query error: \"{}\", Query: \"{}\"",
							sqlite3_errmsg( &Database::ref() ),
							sqlite3_expanded_sql( stmt ) ) );
					}
				default:
					{
						throw DatabaseException( format_ns::format(
							"Unhandled error in sqlite: {} \"{}\"", result, sqlite3_expanded_sql( stmt ) ) );
					}
			}
		}
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) > 1 )
	void operator>>( std::tuple< Ts... >& tpl )
	{
		ran = true;

		//Execute the query.
		while ( true )
		{
			if ( stmt == nullptr ) throw DatabaseException( "stmt was nullptr" );

			const auto result { sqlite3_step( stmt ) };

			switch ( result )
			{
				case SQLITE_ROW:
					[[likely]]
					{
						extractRow< 0 >( stmt, tpl );
						continue;
					}
				case SQLITE_DONE:
					return;
				default:
					{
						throw DatabaseException( format_ns::format(
							"Unhandled error in sqlite: {} \"{}\"", result, sqlite3_expanded_sql( stmt ) ) );
					}
			}
		}
	}

	~Binder();
};

#endif //ATLASGAMEMANAGER_BINDER_HPP
