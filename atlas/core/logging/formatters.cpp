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

/*
auto format_ns::formatter< std::source_location >::format( const std::source_location& loc, format_context& ctx ) const
	-> decltype( ctx.out() )
{

}
*/

//QURL
auto format_ns::formatter< QUrl >::format( const QUrl& url, format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_ns::format_to( ctx.out(), "{}", url.toString().toStdString() );
}

auto format_ns::formatter< QSize >::format( const QSize size, format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_ns::format_to( ctx.out(), "{}x{}", size.width(), size.height() );
}
