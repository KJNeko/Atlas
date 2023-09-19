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
			const format_ns::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { format_ns::format( "{}: ", loc ) };
				const auto body_str { format_ns::format( body, std::forward< Ts >( ts )... ) };

				internal::logDebug( location_string + body_str );
			}
			else
				internal::logDebug( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	debug( format_ns::format_string< Ts... >, Ts&&... ) -> debug< Ts... >;

	template < typename... Ts >
	struct info
	{
		info(
			const format_ns::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
				internal::logInfo(
					format_ns::format( "{}: \"{}\"", loc, format_ns::format( body, std::forward< Ts >( ts )... ) ) );
			else
				internal::logInfo( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	info( std::string_view, Ts&&... ) -> info< Ts... >;

	template < typename... Ts >
	struct warn
	{
		warn(
			const format_ns::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { format_ns::format( "{}: ", loc ) };
				const auto body_str { format_ns::format( body, std::forward< Ts >( ts )... ) };

				internal::logWarn( location_string + body_str );
			}
			else
				internal::logWarn( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	warn( format_ns::format_string< Ts... >, Ts&&... ) -> warn< Ts... >;

	template < typename... Ts >
	struct error
	{
		error(
			const format_ns::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { format_ns::format( "{}: ", loc ) };
				const auto body_str { format_ns::format( body, std::forward< Ts >( ts )... ) };

				internal::logError( location_string + body_str );
			}
			else
				internal::logError( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	error( format_ns::format_string< Ts... >, Ts&&... a ) -> error< Ts... >;

	template < typename... Ts >
	struct errorLoc
	{
		errorLoc( const format_ns::format_string< Ts... > body, const std::source_location& loc, Ts&... ts )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { format_ns::format( "{}: ", loc ) };
				const auto body_str { format_ns::format( body, std::forward< Ts >( ts )... ) };

				internal::logError( location_string + body_str );
			}
			else
				internal::logError( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	errorLoc( format_ns::format_string< Ts... >, const std::source_location&, Ts&&... a ) -> errorLoc< Ts... >;

	template < typename... Ts >
	struct critical
	{
		critical(
			const format_ns::format_string< Ts... > body,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const auto location_string { format_ns::format( "{}: ", loc ) };
				const auto body_str { format_ns::format( body, std::forward< Ts >( ts )... ) };

				internal::logCritical( location_string + body_str );
			}
			else
				internal::logCritical( format_ns::format( "{}: \"{}\"", loc, body ) );
		}
	};

	template < typename... Ts >
	critical( format_ns::format_string< Ts... >, Ts&&... ) -> critical< Ts... >;
} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
