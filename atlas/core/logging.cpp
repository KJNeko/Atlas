//
// Created by kj16609 on 1/27/23.
//

#include "core/logging.hpp"

#include "config.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#pragma GCC diagnostic pop

#else

#include <fmt/ranges.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#endif

void initLogging()
{
	spdlog::enable_backtrace( 32 );

	auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };

	auto file_sink {
		std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 )
	};
	file_sink->set_level( spdlog::level::trace );

	std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink };

	auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };

	logger->debug( "Logger setup" );

	logger->set_level( spdlog::level::debug );
	logger->flush_on( spdlog::level::debug );

	spdlog::set_default_logger( logger );

#ifndef NDEBUG
	switch ( 1 )
#else
	switch ( config::logging::level::get() )
#endif
	{
		case 0:
			logger->set_level( spdlog::level::trace );
			spdlog::debug( "Logging level set to \'trace\'" );
			break;
		case 1:
			logger->set_level( spdlog::level::debug );
			spdlog::debug( "Logging level set to \'debug\'" );
			break;
		case 2:
			logger->set_level( spdlog::level::info );
			spdlog::debug( "Logging level set to \'info\'" );
			break;
		case 3:
			logger->set_level( spdlog::level::warn );
			spdlog::debug( "Logging level set to \'warn\'" );
			break;
		case 4:
			logger->set_level( spdlog::level::err );
			spdlog::debug( "Logging level set to \'error\'" );
			break;
		case 5:
			logger->set_level( spdlog::level::critical );
			spdlog::debug( "Logging level set to \'critical\'" );
			break;
		case 6:
			logger->set_level( spdlog::level::off );
			break;
		default:
			logger->set_level( spdlog::level::info );
			break;
	}

	spdlog::debug( "Default logger set" );
}
