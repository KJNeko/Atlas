//
// Created by kj16609 on 6/23/23.
//

#include "Binder.hpp"

Binder::Binder( const std::string_view sql )
{
	const auto prepare_ret {
		sqlite3_prepare_v2( &Database::ref(), sql.data(), static_cast< int >( sql.size() + 1 ), &stmt, nullptr )
	};

	if ( prepare_ret != SQLITE_OK )
	{
		atlas::logging::dev::error( fmt::format( "Failed to prepare statement: \n\t{}", sql ) );
		throw std::runtime_error( fmt::format(
			"DB: Failed to prepare statement: \"{}\", Reason: \"{}\"", sql, sqlite3_errmsg( &Database::ref() ) ) );
	}

	max_param_count = sqlite3_bind_parameter_count( stmt );
}

Binder::~Binder()
{
	if ( !ran ) [[unlikely]]
	{
		sqlite3_step( stmt );
	}

	sqlite3_finalize( stmt );
}

/*
template <>
int bindParameter( sqlite3_stmt* stmt, const std::string val, const int idx ) noexcept
{
	return sqlite3_bind_text( stmt, idx, val.c_str(), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
}
*/

template <>
int bindParameter( sqlite3_stmt* stmt, const QString val, const int idx ) noexcept
{
	const QByteArray utf8_text { val.toUtf8() };
	return sqlite3_bind_text( stmt, idx, utf8_text.data(), static_cast< int >( utf8_text.size() ), SQLITE_TRANSIENT );
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::u8string val, const int idx ) noexcept
{
	return sqlite3_bind_text(
		stmt, idx, reinterpret_cast< const char* >( val.c_str() ), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::string_view val, const int idx ) noexcept
{
#ifdef __linux__
	return sqlite3_bind_text( stmt, idx, val.data(), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
#else
	QString str { QString::fromLocal8Bit( val.data(), static_cast< qsizetype >( val.size() ) ) };
	return bindParameter( stmt, std::move( str ), idx );
#endif
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::filesystem::path val, const int idx ) noexcept
{
	return bindParameter< std::u8string >( stmt, val.u8string(), idx );
}

template <>
int bindParameter( sqlite3_stmt* stmt, const std::vector< std::byte > val, const int idx ) noexcept
{
	return sqlite3_bind_blob( stmt, idx, val.data(), static_cast< int >( val.size() ), nullptr );
}

template <>
int bindParameter( sqlite3_stmt* stmt, [[maybe_unused]] const std::nullopt_t nullopt, const int idx ) noexcept
{
	return sqlite3_bind_null( stmt, idx );
}

template <>
int bindParameter( sqlite3_stmt* stmt, const double val, const int idx ) noexcept
{
	return sqlite3_bind_double( stmt, idx, val );
}
