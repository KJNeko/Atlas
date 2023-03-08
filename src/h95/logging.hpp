//
// Created by kj16609 on 1/27/23.
//

#ifndef HYDRUS95_LOGGING_HPP
#define HYDRUS95_LOGGING_HPP

#include <filesystem>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

#pragma GCC diagnostic pop

#include <QString>

#include "Types.hpp"

void initLogging();

//! Formatter for QString in fmt::format
template<> struct fmt::formatter< QString >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const QString& my, format_context& ctx ) const -> decltype( ctx.out() )
	{
		return format_to( ctx.out(), "{}", my.toStdString() );
	}
};

template<> struct fmt::formatter< RecordID >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const RecordID id, format_context& ctx ) const -> decltype( ctx.out() )
	{
		return format_to( ctx.out(), "{}", static_cast< uint64_t >( id ) );
	}
};

template<> struct fmt::formatter< std::filesystem::path >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const std::filesystem::path& path, format_context& ctx ) const -> decltype( ctx.out() );
};


struct Record;

template<> struct fmt::formatter< Record >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const Record& my, format_context& ctx ) const -> decltype( ctx.out() );
};

struct GameMetadata;

template<> struct fmt::formatter< GameMetadata >
{
	constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	auto format( const GameMetadata& my, format_context& ctx ) const -> decltype( ctx.out() );
};


#endif	//HYDRUS95_LOGGING_HPP
