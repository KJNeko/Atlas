//
// Created by kj16609 on 1/15/23.
//

#include "core/database/GameMetadata.hpp"

#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/database/record/RecordData.hpp"
#include "core/logging.hpp"
#include "core/utils/execute/executeProc.hpp"

QString GameMetadata::getVersionName() const
{
	return m_version;
}

bool GameMetadata::isInPlace() const
try
{
	ZoneScoped;
	bool in_place { false };
	RapidTransaction transaction;
	transaction << "SELECT in_place FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> in_place;

	return in_place;
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::isInPlace: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::isInPlace: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

std::uint32_t GameMetadata::getPlaytime() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::uint32_t total_playtime { 0 };

	transaction << "SELECT version_playtime FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> total_playtime;

	return total_playtime;
}

std::uint64_t GameMetadata::getLastPlayed() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::uint64_t last_played { 0 };

	transaction << "SELECT last_played FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> last_played;

	return last_played;
}

std::filesystem::path GameMetadata::getPath() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::string path;
	transaction << "SELECT game_path FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> path;

	if ( isInPlace() )
		return { path };
	else
		return config::paths::games::getPath() / std::filesystem::path( path );
}

std::filesystem::path GameMetadata::getRelativeExecPath() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::string str;
	transaction << "SELECT exec_path FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> str;
	return { str };
}

std::filesystem::path GameMetadata::getExecPath() const
{
	ZoneScoped;
	return getPath() / getRelativeExecPath();
}

void GameMetadata::playGame()
{
	ZoneScoped;
	if ( const auto executable = getExecPath(); std::filesystem::exists( executable ) )
	{
		const std::chrono::time_point< std::chrono::system_clock > now { std::chrono::system_clock::now() };

		executeProc( QString::fromStdString( executable.string() ) );

		const auto duration {
			std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now() - now )
		};

		addPlaytime( static_cast< uint32_t >( duration.count() ) );
		setLastPlayed( static_cast<
					   uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() )
		                               .count() ) );
		return;
	}
	else
		spdlog::error( "Failed to launch game with executable {}", executable.string() );
}

void GameMetadata::addPlaytime( const std::uint32_t playtime )
{
	ZoneScoped;
	RapidTransaction transaction;
	transaction << "UPDATE game_metadata SET version_playtime = ? WHERE record_id = ? AND version = ?"
				<< playtime + getPlaytime() << m_parent->getID() << m_version.toStdString();

	const auto current { m_parent->get< "total_playtime" >() };

	m_parent->set< "total_playtime" >( playtime + current );
}

void GameMetadata::setLastPlayed( const std::uint64_t last_played )
{
	ZoneScoped;
	RapidTransaction transaction;
	transaction << "UPDATE game_metadata SET last_played = ? WHERE record_id = ? AND version = ?" << last_played
				<< m_parent->getID() << m_version.toStdString();
	m_parent->set< "last_played_r" >( last_played );
}

std::uint64_t GameMetadata::getFolderSize() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::size_t folder_size { 0 };
	transaction << "SELECT folder_size FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> folder_size;
	return folder_size;
}

RecordID GameMetadata::getParentID() const
{
	return m_parent->getID();
}

void GameMetadata::setVersionName( const QString str )
{
	ZoneScoped;
	RapidTransaction transaction;
	transaction << "UPDATE game_metadata SET version = ? WHERE record_id = ? AND version = ?" << str.toStdString()
				<< m_parent->getID() << m_version.toStdString();
	m_version = str;
}

void GameMetadata::setRelativeExecPath( const std::filesystem::path& path )
{
	ZoneScoped;
	RapidTransaction transaction;
	transaction << "UPDATE game_metadata SET exec_path = ? WHERE record_id = ? AND version = ?" << path.string()
				<< m_parent->getID() << m_version.toStdString();
}

std::uint64_t GameMetadata::getImportTime() const
{
	ZoneScoped;
	RapidTransaction trans;
	std::uint64_t import_time { 0 };
	trans << "SELECT date_added FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
		  << m_version.toStdString()
		>> import_time;

	return import_time;
}
