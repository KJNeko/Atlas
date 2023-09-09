//
// Created by kj16609 on 9/8/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_EXTRACTORS_HPP
#define ATLASGAMEMANAGER_EXTRACTORS_HPP

#include <concepts>
#include <sqlite3.h>

template < std::uint64_t, typename T >
void extract( sqlite3_stmt*, T& ) = delete;

template < std::uint64_t index, typename T >
	requires std::is_integral_v< T >
void extract( sqlite3_stmt* stmt, T& t ) noexcept
{
	//If the size of type T is bigger then 32 bits. Use int64
	if constexpr ( sizeof( T ) > ( 32 / 8 ) )
	{
		if constexpr ( std::same_as< T, sqlite3_int64 > )
			t = sqlite3_column_int64( stmt, index );
		else
			t = static_cast< T >( sqlite3_column_int64( stmt, index ) );
	}
	else
	{
		if constexpr ( std::same_as< T, int > )
			t = sqlite3_column_int( stmt, index );
		else
			t = static_cast< T >( sqlite3_column_int( stmt, index ) );
	}
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::u8string >
void extract( sqlite3_stmt* stmt, std::u8string& t ) noexcept
{
	const unsigned char* const txt { sqlite3_column_text( stmt, index ) };

	if ( txt == nullptr )
	{
		t = {};
		return;
	}
	else
	{
		const auto len { strlen( reinterpret_cast< const char* const >( txt ) ) };
		t = { reinterpret_cast< const char8_t* >( txt ), len };
		return;
	}
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, QString >
void extract( sqlite3_stmt* stmt, QString& t ) noexcept
{
	const unsigned char* const txt { sqlite3_column_text( stmt, index ) };

	if ( txt == nullptr )
	{
		t = {};
		return;
	}
	else
	{
		t = QString::fromUtf8( txt );
		return;
	}
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::filesystem::path >
void extract( sqlite3_stmt* stmt, std::filesystem::path& t ) noexcept
{
	std::u8string str;
	extract< index, std::u8string >( stmt, str );
	t = { std::move( str ) };
}

template < std::uint64_t index, typename T >
	requires std::is_same_v< T, std::vector< std::byte > >
void extract( sqlite3_stmt* stmt, std::vector< std::byte >& t ) noexcept
{
	const void* data { sqlite3_column_blob( stmt, index ) };
	const std::size_t size { static_cast< std::size_t >( sqlite3_column_bytes( stmt, index ) ) };

	t.resize( size );
	std::memcpy( t.data(), data, size );
}

template < std::uint64_t index, typename... Args >
void extractRow( sqlite3_stmt* stmt, std::tuple< Args... >& tpl ) noexcept
{
	auto& ref { std::get< index >( tpl ) };
	extract< index, std::remove_reference_t< decltype( ref ) > >( stmt, ref );

	if constexpr ( index < sizeof...( Args ) - 1 ) extractRow< index + 1, Args... >( stmt, tpl );
}
#endif //ATLASGAMEMANAGER_EXTRACTORS_HPP
