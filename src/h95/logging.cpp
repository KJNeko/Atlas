//
// Created by kj16609 on 1/27/23.
//

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/logger.h>
#include <fmt/ranges.h>
#include <h95/config.hpp>

#pragma GCC diagnostic pop

#include <h95/logging.hpp>

#include <h95/database/Record.hpp>

void flush_logger()
{
	spdlog::default_logger()->flush();
}

void initLogging()
{
	spdlog::enable_backtrace( 32 );

	auto console_sink { std::make_shared< spdlog::sinks::stdout_color_sink_mt >() };

	auto file_sink {
		std::make_shared< spdlog::sinks::rotating_file_sink_mt >( "./data/logs/log.txt", 1024 * 1024 * 1, 3 ) };
	file_sink->set_level( spdlog::level::trace );

	std::vector< spdlog::sink_ptr > sinks { console_sink, file_sink };

	auto logger { std::make_shared< spdlog::logger >( "", sinks.begin(), sinks.end() ) };

	logger->info( "Logger setup" );

	logger->flush_on( spdlog::level::info );

	if ( std::atexit( flush_logger ) != 0 ) { spdlog::warn( "Failed to register logger flush at std::exit" ); }

	spdlog::set_default_logger( logger );

	#ifndef NDEBUG
	console_sink->set_level( spdlog::level::debug );
	//#else
	switch ( getSettings< int >( "logging/level", 2 ) )
	{
		case 0:
			console_sink->set_level( spdlog::level::trace );
			spdlog::debug("Logging level set to \'trace\'");
			break;
		case 1:
			console_sink->set_level( spdlog::level::debug );
			spdlog::info("Logging level set to \'debug\'");
			break;
		case 2:
			console_sink->set_level( spdlog::level::info );
			spdlog::info("Logging level set to \'info\'");
			break;
		case 3:
			console_sink->set_level( spdlog::level::warn );
			spdlog::info("Logging level set to \'warn\'");
			break;
		case 4:
			console_sink->set_level( spdlog::level::err );
			spdlog::info("Logging level set to \'error\'");
			break;
		case 5:
			console_sink->set_level( spdlog::level::critical );
			spdlog::info("Logging level set to \'critical\'");
			break;
		case 6:
			console_sink->set_level( spdlog::level::off );
			break;
		default:
			console_sink->set_level( spdlog::level::info );
			break;
	}
	#endif

	spdlog::info( "Default logger set" );
}

//fmt stuff

auto fmt::formatter< std::filesystem::path >::format( const std::filesystem::path& path, format_context& ctx ) const
	-> decltype( ctx.out() )
{
	bool can_canonical { false };

	try
	{
		if ( std::filesystem::canonical( path ) == path )
			can_canonical = false;
		else
			can_canonical = true;
	}
	catch ( std::filesystem::filesystem_error& e )
	{
		can_canonical = false;
	}

	if ( can_canonical )
		return format_to(
			ctx.out(),
			"[\"{}\" (Canonical: \"{}\"), Exists: {}]",
			path.string(),
			std::filesystem::canonical( path ).string(),
			std::filesystem::exists( path ) ? "True" : "False" );
	else
		return format_to(
			ctx.out(),
			"[\"{}\", Exists: \"{}\"]",
			path.string(),
			std::filesystem::exists( path ) ? "True" : "False" );
}


auto fmt::formatter< Record >::format( const Record& my, fmt::format_context& ctx ) const -> decltype( ctx.out() )
{
	return format_to(
		ctx.out(),
		"RecordID: {}, Title: \"{}\", Creator: \"{}\", Engine: \"{}\", Versions: {}, Banner: {}, Previews: {}",
		my.m_id,
		my.m_title,
		my.m_creator,
		my.m_engine,
		my.m_versions,
		my.m_banner,
		my.m_previews );
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
}
