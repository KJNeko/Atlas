//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_BINDER_HPP
#define ATLASGAMEMANAGER_BINDER_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

#include "Database.hpp"
#include "FunctionDecomp.hpp"

template < std::uint64_t index, typename T >
	requires std::is_integral_v< T >
void extract( sqlite3_stmt* stmt, T& t )
{
	t = static_cast< T >( sqlite3_column_int64( stmt, index ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::string >
void extract( sqlite3_stmt* stmt, std::string& t )
{
	const unsigned char* txt { sqlite3_column_text( stmt, index ) };
	t = std::string( reinterpret_cast< const char* >( txt ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, QString >
void extract( sqlite3_stmt* stmt, QString& t )
{
	const unsigned char* txt { sqlite3_column_text( stmt, index ) };
	t = QString::fromRawData(
		reinterpret_cast< const QChar* >( txt ),
		static_cast< qsizetype >( strlen( reinterpret_cast< const char* >( txt ) ) ) );
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::vector< std::byte > >
void extract( sqlite3_stmt* stmt, std::vector< std::byte >& t )
{
	const void* data { sqlite3_column_blob( stmt, index ) };
	const std::size_t size { static_cast< std::size_t >( sqlite3_column_bytes( stmt, index ) ) };

	t.resize( size );
	std::memcpy( t.data(), data, size );
}

template < std::uint64_t index, typename... Args >
void extractRow( sqlite3_stmt* stmt, std::tuple< Args... >& tpl )
{
	auto& ref { std::get< index >( tpl ) };
	extract< index, std::remove_reference_t< decltype( ref ) > >( stmt, ref );

	if constexpr ( index < sizeof...( Args ) - 1 ) extractRow< index + 1, Args... >( stmt, tpl );
}

template < typename T >
	requires( !std::is_integral_v< T > )
void bindParameter( sqlite3_stmt* stmt, const T val, const int idx );

template < typename T >
	requires std::is_integral_v< T >
void bindParameter( sqlite3_stmt* stmt, const T val, const int idx )
{
	sqlite3_bind_int64( stmt, idx, static_cast< sqlite3_int64 >( val ) );
}

class Binder
{
	sqlite3_stmt* stmt { nullptr };
	bool done { false };
	int param_counter { 0 };

  public:

	Binder( const std::string sql )
	{
		if ( sqlite3_prepare_v2( &Database::ref(), sql.c_str(), static_cast< int >( sql.size() + 1 ), &stmt, nullptr )
		     != SQLITE_OK )
		{
			spdlog::error( "Failed to prepare statement {}", sql );
			throw std::runtime_error( "Failed to prepare statement" );
		}
	}

	/*template<typename T>
	Binder& operator<<(const T t)
	{
		bindParameter<T>(stmt, t, ++param_counter);
	}*/

	template < typename T >
	Binder& operator<<( T t )
	{
		if ( param_counter > sqlite3_bind_parameter_count( stmt ) )
		{
			throw std::runtime_error( fmt::format(
				"param_counter > param_count = {} > {} for query \"{}\"",
				param_counter,
				sqlite3_bind_parameter_count( stmt ),
				std::string( sqlite3_sql( stmt ) ) ) );
		}

		bindParameter< T >( stmt, std::move( t ), ++param_counter );
		return *this;
	}

	template < typename T >
	void operator>>( T& t )
	{
		sqlite3_step( stmt );
		extract< 0, T >( stmt, t );
	}

	template < typename T >
	void operator>>( std::vector< T >& vec )
	{
		while ( !done )
		{
			sqlite3_step( stmt );
			T tmp;
			extract< 0, T >( stmt, tmp );

			if constexpr ( std::is_trivially_move_constructible_v< T > )
				vec.emplace_back( std::move( tmp ) );
			else
				vec.emplace_back( tmp );
		}
	}

	template < typename Function >
	void operator>>( Function&& func )
	{
		using FuncArgs = FunctionDecomp< Function >;
		using Tpl = FuncArgs::ArgTuple;

		//Execute the query.
		while ( !done )
		{
			switch ( sqlite3_step( stmt ) )
			{
				case SQLITE_DONE:
					{
						done = true;
						break;
					}
				case SQLITE_ROW:
					{
						//Extract the row
						Tpl tpl;
						extractRow< 0 >( stmt, tpl );
						std::apply( func, tpl );
						break;
					}
				default:
					{
						spdlog::error( "Unhandled error in sqlite!" );
						throw std::runtime_error( "Unhandled error in sqlite!" );
					}
			}

			if ( done ) break;
		}
	}
};

#endif //ATLASGAMEMANAGER_BINDER_HPP
