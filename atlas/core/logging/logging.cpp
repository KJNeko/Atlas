//
// Created by kj16609 on 1/27/23.
//

#include "logging.hpp"

#include "core/config.hpp"

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
#include <spdlog/spdlog.h>

#pragma GCC diagnostic pop

#else

#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#endif

namespace atlas::logging
{
	void init()
	{
		auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };

#ifndef NDEBUG
		console_sink->set_level( spdlog::level::trace );
#endif

		// file sink will print out to a log file and rotate it out when getting too big.
		auto file_sink {
			std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 )
		};
		file_sink->set_level( spdlog::level::info );

		auto error_file_sink {
			std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/error.txt", 1024 * 1024 * 1, 3 )
		};
		error_file_sink->set_level( spdlog::level::err );

		//TODO: Hook into UI and make a sink for spdlog to output into.

		std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink, error_file_sink };

		auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };

		logger->debug( "Logger setup" );

		spdlog::set_default_logger( logger );
		setFormat();

		spdlog::enable_backtrace( 32 );
		spdlog::debug( "Default logger set" );
	}

	void setFormat()
	{
		spdlog::set_pattern( "[%H:%M:%S] [%^%l%$] [thread %t]:\n\t%v" );
	}
} // namespace atlas::logging
