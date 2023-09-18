//
// Created by kj16609 on 9/11/23.
//

#ifndef ATLASGAMEMANAGER_FORMATTERS_HPP
#define ATLASGAMEMANAGER_FORMATTERS_HPP

#include <QString>

#ifdef __GNUC__
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

#include "fmt/format.h"

#pragma GCC diagnostic pop
#endif

#include <filesystem>
#include <source_location>

//! Formatter for QString in fmt::format
template <>
struct fmt::formatter< QString >
{
	constexpr auto parse( fmt::format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	template < typename FormatContext >
	auto format( const QString& my, FormatContext& ctx ) const -> decltype( ctx.out() )
	{
		return fmt::format_to( ctx.out(), "{}", my.toStdString() );
	}
};

template <>
struct fmt::formatter< std::filesystem::path >
{
	bool print_canonical { false };
	bool print_exists { false };

	constexpr format_parse_context::iterator parse( format_parse_context& ctx )
	{
		//Check if ctx has 'c' 'ce' or 'e' and return the itterator after it
		auto idx { ctx.begin() };
		const auto end { ctx.end() };

		if ( idx != end && *idx == 'c' )
		{
			print_canonical = true;
			++idx;
		}

		if ( idx != end && *idx == 'e' )
		{
			print_exists = true;
			++idx;
		}

		return idx;
	}

	format_context::iterator format( const std::filesystem::path& path, format_context& ctx ) const;
};

template <>
struct fmt::formatter< std::source_location >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const std::source_location& loc, format_context& ctx ) const;
};

#endif //ATLASGAMEMANAGER_FORMATTERS_HPP
