//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#include <QString>

#include <filesystem>

#ifdef __GNUC__
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Warray-bounds"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif // __clang__
#if __GNUC__ >= 13
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif
#endif // __GNUC__

#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "Types.hpp"

void initLogging();

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

namespace atlas::logging
{
	//! Loads the spdlog interfaces for file rotations and logging without a GUI
	void init();

	//! Loads the GUI hooks required for some warnings to display to the user
	void initGUIHooks();

	//! Notifies the user of a warning.
	inline void userwarn( [[maybe_unused]] std::string message )
	{
		spdlog::warn( "userwarn: {}", message );
	}

} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
