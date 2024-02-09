//
// Created by kj16609 on 9/11/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_FORMATTERS_HPP
#define ATLASGAMEMANAGER_FORMATTERS_HPP

#include <QSize>
#include <QString>
#include <QUrl>

#include <filesystem>
#include <source_location>

// clang-format:off

#ifdef HAVE_STD_FORMAT
#include <format>
namespace format_ns = std;

#else
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include <fmt/format.h>
namespace format_ns = fmt;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif

// clang-format:on

//! Formatter for QString in format_ns::format
template <>
struct format_ns::formatter< QString >
{
	constexpr auto parse( format_ns::format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	template < typename FormatContext >
	auto format( const QString& my, FormatContext& ctx ) const -> decltype( ctx.out() )
	{
		return format_ns::format_to( ctx.out(), "{}", my.toStdString() );
	}
};

template <>
struct format_ns::formatter< std::filesystem::path >
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
struct format_ns::formatter< std::source_location >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const std::source_location& loc, format_context& ctx ) const
	{
#ifndef ATLAS_EMBED_FULL_SOURCE_LOCATION

#ifndef ATLAS_CMAKE_SOURCE_LOCATION
#error "ATLAS_CMAKE_SOURCE_LOCATION must be defined to use ATLAS_EMBED_FULL_SOURCE_LOCATION"
#endif

		//ATLAS_SANITIZE_SOURCE_LOC will be a string that is the source path.
		constexpr std::size_t sanitize_offset { std::char_traits< char >::length( ATLAS_CMAKE_SOURCE_LOCATION ) };

		return format_ns::format_to( ctx.out(), "{}:{}", loc.file_name() + sanitize_offset, loc.line() );
#else
		return format_ns::
			format_to( ctx.out(), "File: {}:{}\n\tFunction: {}\n\t", loc.file_name(), loc.line(), loc.function_name() );
#endif
	}
};

template <>
struct format_ns::formatter< QSize >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const QSize size, format_context& ctx ) const;
};

template <>
struct format_ns::formatter< QUrl >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const QUrl& url, format_context& ctx ) const;
};

#endif //ATLASGAMEMANAGER_FORMATTERS_HPP
