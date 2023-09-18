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

namespace atlas::logging::internal
{
	void logDebug( const std::string_view msg )
	{
		spdlog::debug( msg );
	}

	void logInfo( const std::string_view msg )
	{
		spdlog::info( msg );
	}

	void logWarn( const std::string_view msg )
	{
		spdlog::warn( msg );
	}

	void logError( const std::string_view msg )
	{
		spdlog::error( msg );
	}

	void logCritical( const std::string_view msg )
	{
		spdlog::critical( msg );
	}

} // namespace atlas::logging::internal
