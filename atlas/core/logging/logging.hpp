//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#include <QString>

#include <filesystem>
#include <source_location>

// clang-format: off
#include "formatters.hpp"
// clang-format: on

#include "core/Types.hpp"
#include "core/notifications/notifications.hpp"

void initLogging();

namespace atlas::logging
{
	//! Loads the spdlog interfaces for file rotations and logging without a GUI
	void init();

	//! Loads the GUI hooks required for some warnings to display to the user
	void initGUIHooks();

	//Everything in this namespace is defined in `spdlogHelpers.cpp`
	namespace internal
	{
		void logDebug( const std::string_view );
		void logInfo( const std::string_view );
		void logWarn( const std::string_view );
		void logError( const std::string_view );
		void logCritical( const std::string_view );
	} // namespace internal

	template < typename... Ts >
	struct debug
	{
		debug(
			const std::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { std::format( "{}: ", loc ) };
				const auto body_str { std::format( body, std::forward< Ts >( ts )... ) };

				internal::logDebug( location_string + body_str );
			}
			else
				internal::logDebug( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	debug( std::format_string< Ts... >, Ts&&... ) -> debug< Ts... >;

	template < typename... Ts >
	struct info
	{
		info(
			const std::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
				internal::logInfo( std::format( "{}: \"{}\"", loc, std::format( body, std::forward< Ts >( ts )... ) ) );
			else
				internal::logInfo( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	info( std::string_view, Ts&&... ) -> info< Ts... >;

	template < typename... Ts >
	struct warn
	{
		warn(
			const std::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { std::format( "{}: ", loc ) };
				const auto body_str { std::format( body, std::forward< Ts >( ts )... ) };

				internal::logWarn( location_string + body_str );
			}
			else
				internal::logWarn( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	warn( std::format_string< Ts... >, Ts&&... ) -> warn< Ts... >;

	template < typename... Ts >
	struct error
	{
		error(
			const std::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { std::format( "{}: ", loc ) };
				const auto body_str { std::format( body, std::forward< Ts >( ts )... ) };

				internal::logError( location_string + body_str );
			}
			else
				internal::logError( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	error( std::format_string< Ts... >, Ts&&... a ) -> error< Ts... >;

	template < typename... Ts >
	struct errorLoc
	{
		errorLoc( const std::format_string< Ts... > body, const std::source_location& loc, Ts&... ts )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { std::format( "{}: ", loc ) };
				const auto body_str { std::format( body, std::forward< Ts >( ts )... ) };

				internal::logError( location_string + body_str );
			}
			else
				internal::logError( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	errorLoc( std::format_string< Ts... >, const std::source_location&, Ts&&... a ) -> errorLoc< Ts... >;

	template < typename... Ts >
	struct critical
	{
		critical(
			const std::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { std::format( "{}: ", loc ) };
				const auto body_str { std::format( body, std::forward< Ts >( ts )... ) };

				internal::logCritical( location_string + body_str );
			}
			else
				internal::logCritical( std::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	critical( std::format_string< Ts... >, Ts&&... ) -> critical< Ts... >;
} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
