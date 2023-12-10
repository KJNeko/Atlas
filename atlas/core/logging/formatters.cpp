//
// Created by kj16609 on 9/11/23.
//

#include "formatters.hpp"

#include <QSize>
#include <QUrl>

#include <filesystem>
#include <source_location>

//std stuff

auto format_ns::formatter< std::filesystem::path >::format( const std::filesystem::path& path, format_context& ctx )
	const -> decltype( ctx.out() )
{
	if ( print_canonical && std::filesystem::exists( path ) )
	{
		if ( print_exists )
			return format_ns::format_to(
				ctx.out(),
				"[\"{}\", (Canonical: \"{}\") Exists: \"{}\"]",
				path.string(),
				std::filesystem::canonical( path ).string(),
				std::filesystem::exists( path ) ? "True" : "False" );
		else
			return format_ns::format_to(
				ctx.out(), "[\"{}\" (Canonical: \"{}\")]", path.string(), std::filesystem::canonical( path ).string() );
	}
	else
	{
		if ( print_exists )
			return format_ns::
				format_to( ctx.out(), "[\"{}\"]", path.string(), std::filesystem::exists( path ) ? "True" : "False" );
		else
			return format_ns::format_to( ctx.out(), "[\"{}\"]", path.string() );
	}
}

#ifndef ATLAS_FULL_SOURCE_LOC
#define ATLAS_SANITIZE_SOURCE_LOC ATLAS_CMAKE_SOURCE_LOCATION
#endif

auto format_ns::formatter< std::source_location >::format( const std::source_location& loc, format_context& ctx ) const
	-> decltype( ctx.out() )
{
#ifdef ATLAS_SANITIZE_SOURCE_LOC
	//ATLAS_SANITIZE_SOURCE_LOC will be a string that is the source path.
	constexpr auto sanitize_offset { std::char_traits< char >::length( ATLAS_SANITIZE_SOURCE_LOC ) };

	return format_ns::format_to( ctx.out(), "{}:{}", loc.file_name() + sanitize_offset, loc.line() );
#else
	return format_ns::
		format_to( ctx.out(), "File: {}:{}\n\tFunction: {}\n\t", loc.file_name(), loc.line(), loc.function_name() );
#endif
}

//QURL
auto format_ns::formatter< QUrl >::format( const QUrl& url, format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_ns::format_to( ctx.out(), "{}", url.toString().toStdString() );
}

auto format_ns::formatter< QSize >::format( const QSize size, format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_ns::format_to( ctx.out(), "{}x{}", size.width(), size.height() );
}

auto format_ns::formatter<
	format_ns::format_string<> >::format( const format_ns::format_string<>& str, format_context& ctx ) const
	-> decltype( ctx.out() )
{
	return format_ns::format_to( ctx.out(), "{}", str.get() );
}