//
// Created by kj16609 on 1/15/23.
//

#include <string>

#include <h95/database/GameMetadata.hpp>
#include <h95/logging.hpp>

#include "Record.hpp"
#include "h95/config.hpp"
#include "h95/executeProc.hpp"

std::filesystem::path GameMetadata::getPath() const
{
	return config::paths::games::getPath() / m_game_path;
}

std::filesystem::path GameMetadata::getExecPath() const
{
	spdlog::info( "Exec path: {}", m_exec_path );
	spdlog::info( "Output: {}", getPath() / m_exec_path );
	return getPath() / m_exec_path;
}

void runGame( const RecordID id, const QString version_str )
{
	Record record { id };

	auto version { record->getVersion( version_str ) };

	const std::chrono::time_point< std::chrono::system_clock > now { std::chrono::system_clock::now() };

	executeProc( version.getExecPath() );

	const auto duration {
		std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now() - now )
	};

	version.m_last_played =
		static_cast< uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() ).count() );
	version.m_total_playtime += static_cast< uint32_t >( duration.count() );

	record->updateVersion( version );
	record->setTotalPlaytime( record->getTotalPlaytime() + static_cast< uint32_t >( duration.count() ) );
	record->setLastPlayed( static_cast<
						   uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() )
	                                       .count() ) );
}

void GameMetadata::playGame()
{
	if ( !std::filesystem::exists( getExecPath() ) )
		throw MetadataException( "Executable does not exist" );
	else
		runGame( m_record_id, m_version );
}
