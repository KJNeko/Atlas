//
// Created by kj16609 on 9/8/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_BINDERS_HPP
#define ATLASGAMEMANAGER_BINDERS_HPP

#include <QByteArray>
#include <QString>

#include <concepts>
#include <sqlite3.h>

template < typename T >
[[nodiscard]] int bindParameter( sqlite3_stmt*, const T, const int ) noexcept = delete;

template < typename T >
	requires std::is_integral_v< T >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const T val, const int idx ) noexcept
{
	return sqlite3_bind_int64( stmt, idx, static_cast< sqlite3_int64 >( val ) );
}

template < typename T >
	requires std::is_same_v< T, QString >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const QString val, const int idx ) noexcept
{
	const QByteArray utf8_text { val.toUtf8() };
	return sqlite3_bind_text( stmt, idx, utf8_text.data(), static_cast< int >( utf8_text.size() ), SQLITE_TRANSIENT );
}

template < typename T >
	requires std::is_same_v< T, std::u8string >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const std::u8string val, const int idx ) noexcept
{
	return sqlite3_bind_text(
		stmt, idx, reinterpret_cast< const char* >( val.c_str() ), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
}

template < typename T >
	requires std::is_same_v< T, std::string_view >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const std::string_view val, const int idx ) noexcept
{
#ifdef __linux__
	return sqlite3_bind_text( stmt, idx, val.data(), static_cast< int >( val.size() ), SQLITE_TRANSIENT );
#else
	QString str { QString::fromLocal8Bit( val.data(), static_cast< qsizetype >( val.size() ) ) };
	return bindParameter< QString >( stmt, std::move( str ), idx );
#endif
}

template < typename T >
	requires std::is_same_v< T, std::filesystem::path >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const std::filesystem::path val, const int idx ) noexcept
{
	return bindParameter< std::u8string >( stmt, val.u8string(), idx );
}

template < typename T >
	requires std::is_same_v< T, std::vector< std::byte > >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const std::vector< std::byte > val, const int idx ) noexcept
{
	return sqlite3_bind_blob( stmt, idx, val.data(), static_cast< int >( val.size() ), nullptr );
}

template < typename T >
	requires std::is_same_v< T, std::nullopt_t >
[[nodiscard]] int
	bindParameter( sqlite3_stmt* stmt, [[maybe_unused]] const std::nullopt_t nullopt, const int idx ) noexcept
{
	return sqlite3_bind_null( stmt, idx );
}

template < typename T >
	requires std::is_floating_point_v< T >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const T val, const int idx ) noexcept
{
	return sqlite3_bind_double( stmt, idx, val );
}

template < typename T >
	requires std::is_same_v< std::string, T >
[[nodiscard]] int bindParameter( sqlite3_stmt* stmt, const T val, const int idx ) noexcept
{
	return bindParameter< QString >( stmt, QString::fromStdString( val ), idx );
}

#endif //ATLASGAMEMANAGER_BINDERS_HPP
