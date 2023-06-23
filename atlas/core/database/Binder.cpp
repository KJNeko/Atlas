//
// Created by kj16609 on 6/23/23.
//

#include "Binder.hpp"

template <>
void bindParameter( sqlite3_stmt* stmt, const std::string val, const int idx )
{
	sqlite3_bind_text( stmt, idx, val.c_str(), static_cast< int >( val.size() ), nullptr );
}

template <>
void bindParameter( sqlite3_stmt* stmt, const std::vector< std::byte > val, const int idx )
{
	sqlite3_bind_blob( stmt, idx, val.data(), static_cast< int >( val.size() ), nullptr );
}