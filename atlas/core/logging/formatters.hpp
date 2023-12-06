//
// Created by kj16609 on 9/11/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_FORMATTERS_HPP
#define ATLASGAMEMANAGER_FORMATTERS_HPP

#include <QString>

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

#include <QSize>

#include <filesystem>
#include <source_location>

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

	format_context::iterator format( const std::source_location& loc, format_context& ctx ) const;
};

template <>
struct format_ns::formatter< format_ns::format_string<> >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const format_ns::format_string<>& str, format_context& ctx ) const
	{
		return format_ns::format_to( ctx.out(), "{}", str.get() );
	}
};

template <>
struct format_ns::formatter< QSize >
{
	constexpr format_parse_context::iterator parse( format_parse_context& ctx ) { return ctx.begin(); }

	format_context::iterator format( const QSize size, format_context& ctx ) const
	{
		return format_ns::format_to( ctx.out(), "{}x{}", size.width(), size.height() );
	}
};
#endif //ATLASGAMEMANAGER_FORMATTERS_HPP
