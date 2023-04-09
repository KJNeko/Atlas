//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#include <filesystem>

#include <QString>

#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

#include "Types.hpp"

void initLogging();

//! Formatter for QString in fmt::format
template <>
struct fmt::formatter< QString >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const QString& my, format_context& ctx ) const -> decltype( ctx.out() )
	{
		return format_to( ctx.out(), "{}", my.toStdString() );
	}
};

template <>
struct fmt::formatter< RecordID >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const RecordID id, format_context& ctx ) const -> decltype( ctx.out() )
	{
		return format_to( ctx.out(), "{}", static_cast< uint64_t >( id ) );
	}
};

template <>
struct fmt::formatter< std::filesystem::path >
{
	bool print_canonical { false };
	bool print_exists { false };

	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
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

	auto format( const std::filesystem::path& path, format_context& ctx ) const -> decltype( ctx.out() )
	{
		if ( print_canonical && std::filesystem::exists( path ) )
		{
			if ( print_exists )
				return format_to(
					ctx.out(),
					"[\"{}\", (Canonical: \"{}\") Exists: \"{}\"]",
					path.string(),
					std::filesystem::canonical( path ).string(),
					std::filesystem::exists( path ) ? "True" : "False" );
			else
				return format_to(
					ctx.out(),
					"[\"{}\" (Canonical: \"{}\")]",
					path.string(),
					std::filesystem::canonical( path ).string() );
		}
		else
		{
			if ( print_exists )
				return format_to(
					ctx.out(), "[\"{}\"]", path.string(), std::filesystem::exists( path ) ? "True" : "False" );
			else
				return format_to( ctx.out(), "[\"{}\"]", path.string() );
		}
	}
};

#endif //ATLAS_LOGGING_HPP
