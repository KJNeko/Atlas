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

bool GameMetadata::isInPlace( Transaction transaction ) const
try
{
	bool in_place { false };

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

std::uint32_t GameMetadata::getPlaytime( Transaction transaction ) const
try
{
	std::uint32_t total_playtime { 0 };

	transaction << "SELECT version_playtime FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> total_playtime;

	return total_playtime;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getPlaytime: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getPlaytime: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getPlaytime: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

std::uint64_t GameMetadata::getLastPlayed( Transaction transaction ) const
try
{
	std::uint64_t last_played { 0 };

	transaction << "SELECT last_played FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> last_played;

	return last_played;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getLastPlayed: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getLastPlayed: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getLastPlayed: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

std::filesystem::path GameMetadata::getPath( Transaction transaction ) const
try
{
	std::string path;
	transaction << "SELECT game_path FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> path;

	if ( isInPlace( transaction ) )
		return { path };
	else
		return config::paths::games::getPath() / std::filesystem::path( path );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getPath: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getPath: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getPath: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

std::filesystem::path GameMetadata::getRelativeExecPath( Transaction transaction ) const
try
{
	std::string str;
	transaction << "SELECT exec_path FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> str;
	return { str };
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getRelativeExecPath: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getRelativeExecPath: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::
		error( "({},{})->GameMetadata::getRelativeExecPath: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

std::filesystem::path GameMetadata::getExecPath( Transaction transaction ) const
try
{
	return getPath( transaction ) / getRelativeExecPath( transaction );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getExecPath: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getExecPath: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getExecPath: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

void GameMetadata::playGame( Transaction transaction )
try
{
	if ( const auto executable = getExecPath( transaction ); std::filesystem::exists( executable ) )
	{
		const std::chrono::time_point< std::chrono::system_clock > now { std::chrono::system_clock::now() };

		executeProc( QString::fromStdString( executable.string() ) );

		const auto duration {
			std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now() - now )
		};

		addPlaytime( static_cast< uint32_t >( duration.count() ), transaction );
		setLastPlayed(
			static_cast< uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() )
		                                 .count() ),
			transaction );
		return;
	}
	else
		spdlog::error( "Failed to launch game with executable {}", executable.string() );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::playGame: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::playGame: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::playGame: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

void GameMetadata::addPlaytime( const std::uint32_t playtime, Transaction transaction )
try
{
	transaction << "UPDATE game_metadata SET version_playtime = ? WHERE record_id = ? AND version = ?"
				<< playtime + getPlaytime( transaction ) << m_parent->getID() << m_version.toStdString();

	const auto current { m_parent->total_playtime.get( transaction ) };

	m_parent->total_playtime.set( playtime + current, transaction );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::
		error( "({},{})->GameMetadata::addPlaytime({}): {}", m_parent->getID(), this->m_version, playtime, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::
		error( "({},{})->GameMetadata::addPlaytime({}): {}", m_parent->getID(), this->m_version, playtime, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error(
		"({},{})->GameMetadata::addPlaytime({}): Unknown exception", m_parent->getID(), this->m_version, playtime );
	std::rethrow_exception( std::current_exception() );
}

void GameMetadata::setLastPlayed( const std::uint64_t last_played, Transaction transaction )
try
{
	transaction << "UPDATE game_metadata SET last_played = ? WHERE record_id = ? AND version = ?" << last_played
				<< m_parent->getID() << m_version.toStdString();
	m_parent->last_played.set( last_played, transaction );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setLastPlayed({}): {}", m_parent->getID(), this->m_version, last_played, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setLastPlayed({}): {}", m_parent->getID(), this->m_version, last_played, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error(
		"({},{})->GameMetadata::setLastPlayed({}): Unknown exception",
		m_parent->getID(),
		this->m_version,
		last_played );
	std::rethrow_exception( std::current_exception() );
}

std::uint64_t GameMetadata::getFolderSize( Transaction transaction ) const
try
{
	std::size_t folder_size { 0 };
	transaction << "SELECT folder_size FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
				<< m_version.toStdString()
		>> folder_size;
	return folder_size;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getFolderSize: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getFolderSize: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getFolderSize: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}

RecordID GameMetadata::getParentID() const
{
	return m_parent->getID();
}

void GameMetadata::setVersionName( const QString str, Transaction transaction )
try
{
	transaction << "UPDATE game_metadata SET version = ? WHERE record_id = ? AND version = ?" << str.toStdString()
				<< m_parent->getID() << m_version.toStdString();
	m_version = str;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setVersionName({}): {}",
		m_parent->getID(),
		this->m_version,
		str.toStdString(),
		e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setVersionName({}): {}",
		m_parent->getID(),
		this->m_version,
		str.toStdString(),
		e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error(
		"({},{})->GameMetadata::setVersionName({}): Unknown exception",
		m_parent->getID(),
		this->m_version,
		str.toStdString() );
	std::rethrow_exception( std::current_exception() );
}

void GameMetadata::setRelativeExecPath( const std::filesystem::path& path, Transaction transaction )
try
{
	transaction << "UPDATE game_metadata SET exec_path = ? WHERE record_id = ? AND version = ?" << path.string()
				<< m_parent->getID() << m_version.toStdString();
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setRelativeExecPath({}): {}",
		m_parent->getID(),
		this->m_version,
		path.string(),
		e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::setRelativeExecPath({}): {}",
		m_parent->getID(),
		this->m_version,
		path.string(),
		e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error(
		"({},{})->GameMetadata::setRelativeExecPath({}): Unknown exception",
		m_parent->getID(),
		this->m_version,
		path.string() );
	std::rethrow_exception( std::current_exception() );
}

std::uint64_t GameMetadata::getImportTime( Transaction trans ) const
try
{
	std::uint64_t import_time { 0 };
	trans << "SELECT date_added FROM game_metadata WHERE record_id = ? AND version = ?" << m_parent->getID()
		  << m_version.toStdString()
		>> import_time;

	return import_time;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"({},{})->GameMetadata::getImportTime: {} [{}, {}]",
		m_parent->getID(),
		this->m_version,
		e.what(),
		e.errstr(),
		e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}
catch ( const std::exception& e )
{
	spdlog::error( "({},{})->GameMetadata::getImportTime: {}", m_parent->getID(), this->m_version, e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "({},{})->GameMetadata::getImportTime: Unknown exception", m_parent->getID(), this->m_version );
	std::rethrow_exception( std::current_exception() );
}
