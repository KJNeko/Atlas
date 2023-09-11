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

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "Types.hpp"
#include "logging/dev.hpp"
#include "logging/formatters.hpp"

void initLogging();

namespace atlas::logging
{
	//! Loads the spdlog interfaces for file rotations and logging without a GUI
	void init();

	//! Loads the GUI hooks required for some warnings to display to the user
	void initGUIHooks();

	inline void info( std::string message )
	{
		spdlog::info( message );
	}

	//! Notifies the user of a warning.
	inline void warn( [[maybe_unused]] std::string message )
	{
		spdlog::warn( "userwarn: {}", message );
	}

	template < typename... Ts >
	inline void error( std::string body, Ts... ts )
	{
		spdlog::error( "{}: {}", body, atlas::logging::dev::serializeObjects( ts... ) );
	}

	template <>
	inline void error<>( std::string body )
	{
		spdlog::error( "{}", body );
	}

} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
