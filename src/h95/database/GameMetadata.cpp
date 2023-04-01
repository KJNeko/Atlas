//
// Created by kj16609 on 1/15/23.
//

#include "h95/database/GameMetadata.hpp"

#include "Record.hpp"
#include "h95/config.hpp"
#include "h95/executeProc.hpp"
#include "h95/logging.hpp"

QString GameMetadata::getVersionName() const
{
	return m_version;
}

bool GameMetadata::isInPlace() const
{
	return m_in_place;
}

std::uint32_t GameMetadata::getPlaytime() const
{
	return m_total_playtime;
}

std::uint64_t GameMetadata::getLastPlayed() const
{
	return m_last_played;
}

std::filesystem::path GameMetadata::getPath() const
{
	return config::paths::games::getPath() / m_game_path;
}

std::filesystem::path GameMetadata::getExecPath( bool full ) const
{
	return full ? getPath() / m_exec_path : m_exec_path;
}

void runGame( RecordData& record, const QString version_str )
{
	auto& version { record.getVersion( version_str ) };

	const std::chrono::time_point< std::chrono::system_clock > now { std::chrono::system_clock::now() };

	executeProc( version.getExecPath().string() );

	const auto duration {
		std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now() - now )
	};

	version.addPlaytime( static_cast< uint32_t >( duration.count() ) );
	version.setLastPlayed( static_cast<
						   uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() )
	                                       .count() ) );
}

void GameMetadata::playGame()
{
	if ( !std::filesystem::exists( getExecPath( true ) ) )
		throw MetadataException( "Executable does not exist" );
	else
		runGame( m_parent, m_version );
}

void GameMetadata::addPlaytime( const std::uint32_t playtime )
{
	m_total_playtime += playtime;

	m_parent.addPlaytime( playtime );

	emit playtimeChanged( playtime );
}

void GameMetadata::setLastPlayed( const std::uint64_t last_played )
{
	m_last_played = last_played;

	m_parent.setLastPlayed( last_played );

	emit lastPlayedChanged( last_played );
}