//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#include <QString>

#include <filesystem>
#include <source_location>

//Formatters needs to exist before we include the other things since notifications.hpp uses some formatters in here for it's own logging
// clang-format: off
#include "formatters.hpp"
// clang-format: on

#include "core/Types.hpp"
#include "core/notifications/notifications.hpp"

namespace atlas::logging
{
	//! Loads the spdlog interfaces for file rotations and logging without a GUI
	void init();

	void setFormat();

	//! Loads the GUI hooks required for some warnings to display to the user
	void initGUIHooks();

	std::string formatSourceLocation( const std::source_location loc, const format_ns::string_view msg );

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
			const format_ns::format_string< Ts... > format_string,
			Ts&&... ts,
			const std::source_location& source_location = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const std::string user_message { format_ns::format( format_string, std::forward< Ts >( ts )... ) };
				const std::string full_message { formatSourceLocation( source_location, user_message ) };

				internal::logDebug( full_message );
			}
			else
			{
				const std::string full_message { formatSourceLocation( source_location, format_string.get() ) };

				internal::logDebug( full_message );
			}
		}
	};

	template < typename... Ts >
	debug( format_ns::format_string< Ts... >, Ts&&... ) -> debug< Ts... >;

	template < typename... Ts >
	struct info
	{
		info(
			const format_ns::format_string< Ts... > format_string,
			Ts&&... ts,
			[[maybe_unused]] const std::source_location& source_location = std::source_location::current() )
		{
			const std::string user_message { format_ns::format( format_string, std::forward< Ts >( ts )... ) };
			const std::string full_message { formatSourceLocation( source_location, user_message ) };

			internal::logInfo( full_message );

			if ( notifications::isNotificationsReady() )
				atlas::notifications::createMessage( user_message, full_message, MessageLevel::ATLAS_INFO );
		}
	};

	template < typename... Ts >
	info( format_ns::format_string< Ts... >, Ts&&... ) -> info< Ts... >;

	template < typename... Ts >
	struct warn
	{
		warn(
			const format_ns::format_string< Ts... > format_string,
			Ts&&... ts,
			const std::source_location& source_location = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const std::string user_message { format_ns::format( format_string, std::forward< Ts >( ts )... ) };
				const std::string full_message { formatSourceLocation( source_location, user_message ) };

				internal::logWarn( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage(
						format_ns::format( format_string, std::forward< Ts >( ts )... ),
						full_message,
						MessageLevel::ATLAS_WARNING );
			}
			else
			{
				const std::string full_message { formatSourceLocation( source_location, format_string.get() ) };

				internal::logWarn( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::
						createMessage( format_string.get(), full_message, MessageLevel::ATLAS_WARNING );
			}
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
				const std::string user_message { format_ns::format( body, std::forward< Ts >( ts )... ) };
				const std::string full_message { formatSourceLocation( loc, user_message ) };

				internal::logError( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage(
						format_ns::format( body, std::forward< Ts >( ts )... ),
						full_message,
						MessageLevel::ATLAS_ERROR );
			}
			else
			{
				const std::string full_message { formatSourceLocation( loc, body.get() ) };

				internal::logError( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage( body.get(), full_message, MessageLevel::ATLAS_ERROR );
			}
		}
	};

	template < typename... Ts >
	error( format_ns::format_string< Ts... >, Ts&&... a ) -> error< Ts... >;

	template < typename... Ts >
	struct errorLoc
	{
		errorLoc( const format_ns::format_string< Ts... > format_string, const std::source_location& loc, Ts&... ts )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const std::string user_message { format_ns::format( format_string, std::forward< Ts >( ts )... ) };
				const std::string full_message { formatSourceLocation( loc, user_message ) };

				internal::logError( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage(
						format_ns::format( format_string, std::forward< Ts >( ts )... ),
						full_message,
						MessageLevel::ATLAS_ERROR );
			}
			else
			{
				const std::string full_message { formatSourceLocation( loc, format_string.get() ) };

				internal::logError( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage( format_string.get(), full_message, MessageLevel::ATLAS_ERROR );
			}
		}
	};

	template < typename... Ts >
	errorLoc( format_ns::format_string< Ts... >, const std::source_location&, Ts&&... a ) -> errorLoc< Ts... >;

	template < typename... Ts >
	struct critical
	{
		critical(
			const format_ns::format_string< Ts... > format_string,
			Ts&&... ts,
			const std::source_location& loc = std::source_location::current() )
		{
			if constexpr ( sizeof...( Ts ) > 0 )
			{
				const std::string user_message { format_ns::format( format_string, std::forward< Ts >( ts )... ) };
				const std::string full_message { formatSourceLocation( loc, user_message ) };

				internal::logCritical( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::createMessage(
						format_ns::format( format_string, std::forward< Ts >( ts )... ),
						full_message,
						MessageLevel::ATLAS_CRITICAL );
			}
			else
			{
				const std::string full_message { formatSourceLocation( loc, format_string.get() ) };

				internal::logCritical( full_message );
				if ( notifications::isNotificationsReady() )
					atlas::notifications::
						createMessage( format_string.get(), full_message, MessageLevel::ATLAS_CRITICAL );
			}
		}
	};

	template < typename... Ts >
	critical( format_ns::format_string< Ts... >, Ts&&... ) -> critical< Ts... >;
} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
