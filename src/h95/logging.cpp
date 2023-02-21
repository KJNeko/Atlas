//
// Created by kj16609 on 1/27/23.
//

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/logger.h>

#pragma GCC diagnostic pop

#include <h95/logging.hpp>

void initLogging()
{
	spdlog::enable_backtrace( 32 );

	auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };

#ifndef NDEBUG
	console_sink->set_level( spdlog::level::debug );
#else
	console_sink->set_level( spdlog::level::info );
#endif


	auto file_sink {
		std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 ) };
	file_sink->set_level( spdlog::level::trace );

	std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink };

	auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };
	logger->set_level( spdlog::level::debug );
	logger->info( "Logger setup" );

	spdlog::set_default_logger( logger );

	spdlog::info( "Default logger set" );
}
