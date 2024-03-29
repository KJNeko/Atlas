//
// Created by kj16609 on 6/23/23.
//

#include "Binder.hpp"

#include "core/database/Database.hpp"

Binder::Binder( const std::string_view sql )
{
	const auto prepare_ret {
		sqlite3_prepare_v2( &Database::ref(), sql.data(), static_cast< int >( sql.size() + 1 ), &stmt, nullptr )
	};

	if ( prepare_ret != SQLITE_OK )
	{
		throw DatabaseException( format_ns::format(
			"DB: Failed to prepare statement: \"{}\", Reason: \"{}\"", sql, sqlite3_errmsg( &Database::ref() ) ) );
	}

	max_param_count = sqlite3_bind_parameter_count( stmt );
}

Binder::~Binder() noexcept( false )
{
	if ( !ran ) [[unlikely]]
	{
		atlas::logging::debug( "Binder falloff. Running query" );
		std::optional< std::tuple<> > tpl;
		executeQuery( tpl );
	}

	sqlite3_finalize( stmt );
}
