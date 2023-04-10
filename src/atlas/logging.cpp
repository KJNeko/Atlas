//
// Created by kj16609 on 1/27/23.
//

#include "atlas/logging.hpp"

#include <fmt/ranges.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tracy/Tracy.hpp>

#include "atlas/config.hpp"
#include "atlas/database/Record.hpp"

void initLogging()
{
	ZoneScoped;
	spdlog::enable_backtrace( 32 );

	auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };

	auto file_sink {
		std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 )
	};
	file_sink->set_level( spdlog::level::trace );

	std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink };

	auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };

	logger->info( "Logger setup" );

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
			spdlog::info( "Logging level set to \'info\'" );
			break;
		case 3:
			logger->set_level( spdlog::level::warn );
			spdlog::info( "Logging level set to \'warn\'" );
			break;
		case 4:
			logger->set_level( spdlog::level::err );
			spdlog::info( "Logging level set to \'error\'" );
			break;
		case 5:
			logger->set_level( spdlog::level::critical );
			spdlog::info( "Logging level set to \'critical\'" );
			break;
		case 6:
			logger->set_level( spdlog::level::off );
			break;
		default:
			logger->set_level( spdlog::level::info );
			break;
	}

	spdlog::info( "Default logger set" );
}

//fmt stuff

/*
auto fmt::formatter< Record >::format( const Record& my, fmt::format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_to(
		ctx.out(),
		"RecordID: {}, Title: \"{}\", Creator: \"{}\", Engine: \"{}\", Versions: {}, Banner: {}, Previews: {}",
		my->m_id,
		my->m_title,
		my->m_creator,
		my->m_engine,
		my->m_versions,
		my->m_banner,
		my->m_previews );
}

auto fmt::formatter< GameMetadata >::format( const GameMetadata& my, fmt::format_context& ctx ) const
	-> decltype( ctx.out() )
{
	return format_to(
		ctx.out(),
		"Version: {}, GamePath: {}, ExecPath: {}",
		my.m_version,
		my.m_game_path,
		my.m_exec_path );
}*/