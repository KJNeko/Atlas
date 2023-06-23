//
// Created by kj16609 on 6/23/23.
//

#include "Binder.hpp"

Binder::Binder( const std::string sql )
{
	ZoneScoped;
	if ( sqlite3_prepare_v2( &Database::ref(), sql.c_str(), static_cast< int >( sql.size() + 1 ), &stmt, nullptr )
	     != SQLITE_OK )
	{
		spdlog::error( "Failed to prepare statement {}", sql );
		throw std::runtime_error( fmt::format(
			"DB: Failed to prepare statement: \"{}\", Reason: \"{}\"", sql, sqlite3_errmsg( &Database::ref() ) ) );
	}
}

Binder::~Binder()
{
	ZoneScoped;
	if ( stmt != nullptr )
	{
		sqlite3_step( stmt );
		sqlite3_finalize( stmt );
	}
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::string val, const int idx )
{
	ZoneScoped;
	return sqlite3_bind_text( stmt, idx, val.c_str(), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::vector< std::byte > val, const int idx )
{
	ZoneScoped;
	return sqlite3_bind_blob( stmt, idx, val.data(), static_cast< int >( val.size() ), nullptr );
}