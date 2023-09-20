//
// Created by kj16609 on 9/18/23.
//

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"

#include <spdlog/spdlog.h>

#pragma GCC diagnostic pop

#else
#include <spdlog/spdlog.h>
#endif

#include <string_view>

#include "core/notifications/notifications.hpp"

namespace atlas::logging::internal
{
	using enum MessageLevel;

	void logDebug( const std::string_view msg )
	{
		spdlog::debug( msg );
	}

	void logInfo( const std::string_view msg )
	{
		spdlog::info( msg );
		if ( notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( std::string( msg ) ), INFO );
	}

	void logWarn( const std::string_view msg )
	{
		spdlog::warn( msg );
		if ( notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( std::string( msg ) ), WARNING );
	}

	void logError( const std::string_view msg )
	{
		spdlog::error( msg );
		if ( notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( std::string( msg ) ), ERROR );
	}

	void logCritical( const std::string_view msg )
	{
		spdlog::critical( msg );
		if ( notifications::isNotificationsReady() )
			atlas::notifications::createMessage( QString::fromStdString( std::string( msg ) ), CRITICAL );
	}

} // namespace atlas::logging::internal
