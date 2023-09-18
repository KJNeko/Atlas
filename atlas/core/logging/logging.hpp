//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif

#include <QString>

#include <filesystem>
#include <source_location>

#include "core/Types.hpp"
#include "core/notifications/notifications.hpp"
#include "dev.hpp"
#include "formatters.hpp"

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
		requires( sizeof...( Ts ) > 0 )
	inline void
		debug( const std::string body, Ts... ts, const std::source_location loc = std::source_location::current() )
	{
		using namespace atlas::logging::dev;

		internal::logDebug( fmt::format( "{}:{} {}", serialize( loc ), body, serializeObjects( std::move( ts... ) ) ) );
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) == 0 )
	inline void
		debug( const std::string body, Ts... ts, const std::source_location loc = std::source_location::current() )
	{
		using namespace atlas::logging::dev;

		internal::logDebug( fmt::format( "{}: {} ", loc, body ) );
	}

	[[maybe_unused]] constexpr bool NOSHOW_USER { false };
	[[maybe_unused]] constexpr bool SHOW_USER { true };

	inline void debug( const std::string_view msg, const std::source_location loc = std::source_location::current() )
	{
		internal::logDebug( fmt::format( "{}: {}", loc, msg ) );
	}

	/**
	 * @param message
	 * @param show_user If true then this message will be passed back to the user via a message
	 */
	inline void info(
		std::string message,
		const bool show_user = true,
		const std::source_location loc = std::source_location::current() )
	{
		internal::logInfo( fmt::format( "{}: {}", loc, message ) );
		if ( show_user && notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( message ) );
	}

	//! Notifies the user of a warning.
	/**
	 * @param message
	 * @param show_user If true then the user will be shown this warning
	 */
	inline void warn(
		std::string message,
		const bool show_user = true,
		const std::source_location loc = std::source_location::current() )
	{
		internal::logWarn( fmt::format( "{}: {}", loc, message ) );
		if ( show_user && notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( message ) );
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) >= 1 )
	inline void error( std::string body, Ts... ts, const std::source_location loc = std::source_location::current() )
	{
		const auto serialized_info { atlas::logging::dev::serializeObjects( std::move( ts... ) ) };

		internal::logError( fmt::format( "{}: \"{}\" \n{}", loc, body, serialized_info ) );
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) == 0 )
	inline void error( std::string body, const std::source_location loc = std::source_location::current() )
	{
		internal::logError( fmt::format( "{}: \"{}\"", loc, body ) );
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) >= 1 )
	inline void critical( std::string body, Ts... ts, const std::source_location loc = std::source_location::current() )
	{
		const auto serialized_info { atlas::logging::dev::serializeObjects( std::move( ts... ) ) };

		internal::logCritical( fmt::format( "{}: \"{}\" \n{}", loc, body, serialized_info ) );
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) == 0 )
	inline void critical( std::string body, const std::source_location loc = std::source_location::current() )
	{
		internal::logCritical( fmt::format( "{}: \"{}\"", loc, body ) );
	}
} // namespace atlas::logging

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif //ATLAS_LOGGING_HPP
