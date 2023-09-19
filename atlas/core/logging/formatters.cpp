//
// Created by kj16609 on 9/11/23.
//

#include "formatters.hpp"

//std stuff

#include <format>

auto std::formatter< std::filesystem::path >::format( const std::filesystem::path& path, format_context& ctx ) const
	-> decltype( ctx.out() )
{
	if ( print_canonical && std::filesystem::exists( path ) )
	{
		if ( print_exists )
			return std::format_to(
				ctx.out(),
				"[\"{}\", (Canonical: \"{}\") Exists: \"{}\"]",
				path.string(),
				std::filesystem::canonical( path ).string(),
				std::filesystem::exists( path ) ? "True" : "False" );
		else
			return std::format_to(
				ctx.out(), "[\"{}\" (Canonical: \"{}\")]", path.string(), std::filesystem::canonical( path ).string() );
	}
	else
	{
		if ( print_exists )
			return std::
				format_to( ctx.out(), "[\"{}\"]", path.string(), std::filesystem::exists( path ) ? "True" : "False" );
		else
			return std::format_to( ctx.out(), "[\"{}\"]", path.string() );
	}
}

#define STR( val ) #val
#define TOSTRING( val ) STR( val )

auto std::formatter< std::source_location >::format( const std::source_location& loc, format_context& ctx ) const
	-> decltype( ctx.out() )
{
#ifdef ATLAS_SANITIZE_SOURCE_LOC
	//ATLAS_SANITIZE_SOURCE_LOC will be a string that is the source path.
	constexpr auto sanitize_offset { std::char_traits< char >::length( ATLAS_SANITIZE_SOURCE_LOC ) };

	return std::format_to( ctx.out(), "{}:{} {}", loc.file_name() + sanitize_offset, loc.line(), loc.function_name() );
#else
	return std::format_to( ctx.out(), "{}:{} {}", loc.file_name(), loc.line(), loc.function_name() );
#endif
}