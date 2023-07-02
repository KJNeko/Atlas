//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_BINDER_HPP
#define ATLASGAMEMANAGER_BINDER_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

#include <tracy/TracyC.h>

#include "Database.hpp"
#include "FunctionDecomp.hpp"

template < std::uint64_t index, typename T >
	requires std::is_integral_v< T >
void extract( sqlite3_stmt* stmt, T& t ) noexcept
{
	ZoneScopedN( "extract<integral>" );
	t = static_cast< T >( sqlite3_column_int64( stmt, index ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::string >
void extract( sqlite3_stmt* stmt, std::string& t ) noexcept
{
	ZoneScopedN( "extract<std::string>" );
	const unsigned char* txt { sqlite3_column_text( stmt, index ) };
	t = std::string( reinterpret_cast< const char* >( txt ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, QString >
void extract( sqlite3_stmt* stmt, QString& t ) noexcept
{
	ZoneScopedN( "extract<QString>" );
	const unsigned char* txt { sqlite3_column_text( stmt, index ) };
	t = QString::fromLocal8Bit(
		reinterpret_cast< const char* >( txt ),
		static_cast< qsizetype >( strlen( reinterpret_cast< const char* >( txt ) ) ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::vector< std::byte > >
void extract( sqlite3_stmt* stmt, std::vector< std::byte >& t ) noexcept
{
	ZoneScopedN( "extract<std::vector<std::byte>>" );
	const void* data { sqlite3_column_blob( stmt, index ) };
	const std::size_t size { static_cast< std::size_t >( sqlite3_column_bytes( stmt, index ) ) };

	t.resize( size );
	std::memcpy( t.data(), data, size );
}

template < std::uint64_t index, typename... Args >
void extractRow( sqlite3_stmt* stmt, std::tuple< Args... >& tpl ) noexcept
{
	ZoneScoped;
	auto& ref { std::get< index >( tpl ) };
	extract< index, std::remove_reference_t< decltype( ref ) > >( stmt, ref );

	if constexpr ( index < sizeof...( Args ) - 1 ) extractRow< index + 1, Args... >( stmt, tpl );
}

template < typename T >
	requires( !std::is_integral_v< T > )
int bindParameter( sqlite3_stmt* stmt, const T val, const int idx ) noexcept;

template < typename T >
	requires std::is_integral_v< T >
int bindParameter( sqlite3_stmt* stmt, const T val, const int idx ) noexcept
{
	ZoneScopedN( "bindParameter<integral>" );
	return sqlite3_bind_int64( stmt, idx, static_cast< sqlite3_int64 >( val ) );
}

class Binder
{
	sqlite3_stmt* stmt { nullptr };
	bool done { false };
	int param_counter { 0 };
	int max_param_count { 0 };
	bool ran { false };

	Q_DISABLE_COPY_MOVE( Binder )

  public:

	Binder() = delete;

	Binder( const std::string_view sql );

	template < typename T >
	Binder& operator<<( T t )
	{
		ZoneScopedN( "Bind Value" );
		if ( param_counter > max_param_count )
		{
			throw std::runtime_error( fmt::format(
				"param_counter > param_count = {} > {} for query \"{}\"",
				param_counter,
				sqlite3_bind_parameter_count( stmt ),
				std::string( sqlite3_sql( stmt ) ) ) );
		}

		switch ( bindParameter< T >( stmt, std::move( t ), ++param_counter ) )
		{
			case SQLITE_OK:
				break;
			default:
				{
					throw std::runtime_error( fmt::format(
						"DB: Failed to bind to \"{}\": Reason: \"{}\"",
						sqlite3_sql( stmt ),
						sqlite3_errmsg( &Database::ref() ) ) );
				}
		}

		return *this;
	}

	template < typename T >
	void operator>>( T& t )
	{
		ran = true;
		ZoneScopedN( "Get results into value" );
		TracyCZoneN( step_zone_tracy, "Sqlite3 step", true );
		const auto step_ret { sqlite3_step( stmt ) };
		TracyCZoneEnd( step_zone_tracy );

		if ( step_ret == SQLITE_ROW ) [[likely]]
		{
			extract< 0, T >( stmt, t );
			return;
		}
		else if ( step_ret == SQLITE_DONE )
			return;
		else
		{
			switch ( step_ret )
			{
				default:
					[[fallthrough]];
				case SQLITE_MISUSE:
					[[fallthrough]];
				case SQLITE_BUSY:
					[[fallthrough]];
				case SQLITE_ERROR:
					{
						spdlog::error(
							"DB: Query error: \"{}\", Query: \"{}\"",
							sqlite3_errmsg( &Database::ref() ),
							sqlite3_expanded_sql( stmt ) );
						throw std::runtime_error( fmt::format(
							"DB: Query error: \"{}\", Query: \"{}\"",
							sqlite3_errmsg( &Database::ref() ),
							sqlite3_expanded_sql( stmt ) ) );
					}
			}
		}
	}

	template < typename Function >
	void operator>>( Function&& func )
	{
		using FuncArgs = FunctionDecomp< Function >;
		using Tpl = FuncArgs::ArgTuple;
		ZoneScopedN( "Get results into func" );
		ran = true;

		//Execute the query.
		while ( true )
		{
			if ( stmt == nullptr ) throw std::runtime_error( "stmt was nullptr" );

			const auto step_ret { sqlite3_step( stmt ) };

			if ( step_ret == SQLITE_ROW ) [[likely]]
			{
				Tpl tpl;
				extractRow< 0 >( stmt, tpl );
				std::apply( func, std::move( tpl ) );
				continue;
			}
			else if ( step_ret == SQLITE_DONE )
				return;

			switch ( sqlite3_step( stmt ) )
			{
				default:
					{
						spdlog::error( "Unhandled error in sqlite!" );
						throw std::runtime_error( "Unhandled error in sqlite!" );
					}
			}
		}
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) > 1 )
	void operator>>( std::tuple< Ts... >& tpl )
	{
		ran = true;
		ZoneScopedN( "Get results into func" );

		//Execute the query.
		while ( true )
		{
			if ( stmt == nullptr ) throw std::runtime_error( "stmt was nullptr" );

			const auto step_ret { sqlite3_step( stmt ) };

			if ( step_ret == SQLITE_ROW ) [[likely]]
			{
				extractRow< 0 >( stmt, tpl );
				continue;
			}
			else if ( step_ret == SQLITE_DONE )
				return;

			switch ( sqlite3_step( stmt ) )
			{
				default:
					{
						spdlog::error( "Unhandled error in sqlite!" );
						throw std::runtime_error( "Unhandled error in sqlite!" );
					}
			}
		}
	}

	~Binder();
};

#endif //ATLASGAMEMANAGER_BINDER_HPP
