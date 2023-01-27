//
// Created by kj16609 on 1/27/23.
//

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/logger.h>
#include <memory>

#include "logging.hpp"

void initLogging()
{
	spdlog::enable_backtrace( 32 );

	auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };
	console_sink->set_level( spdlog::level::info );

	auto file_sink {
		std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 ) };
	file_sink->set_level( spdlog::level::trace );

	std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink };

	auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };
	logger->set_level( spdlog::level::debug );

	spdlog::set_default_logger( logger );

	spdlog::info( "Logger set" );
}
