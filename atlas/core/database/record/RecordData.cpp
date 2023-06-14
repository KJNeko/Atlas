//
// Created by kj16609 on 1/15/23.
//

#include "RecordData.hpp"

#include <iostream>

#include <QPixmapCache>

#include "Record.hpp"
#include "RecordBanner.hpp"
#include "RecordPreviews.hpp"
#include "core/database/Database.hpp"
#include "core/database/GameMetadata.hpp"
#include "core/imageManager.hpp"

RecordData::RecordData( const RecordID id, Transaction transaction ) : m_id( id )
{
	ZoneScoped;
	if ( id == 0 ) throw std::runtime_error( "Invalid record id" );

	bool exists;
	transaction << "SELECT record_id FROM records WHERE record_id = ?" << m_id >>
		[ & ]( [[maybe_unused]] const RecordID ) { exists = true; };

	if ( !exists ) throw std::runtime_error( "Record does not exist" );
}

RecordID RecordData::getID() const
{
	return m_id;
}

std::optional< GameMetadata > RecordData::getVersion( const QString version_name, Transaction transaction )
try
{
	ZoneScoped;
	const auto versions { getVersions( transaction ) };

	const auto idx { std::find_if(
		versions.begin(),
		versions.end(),
		[ &version_name ]( const GameMetadata& version ) { return version.getVersionName() == version_name; } ) };

	if ( idx == versions.end() )
		return std::nullopt;
	else
		return *idx;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getVersion(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return std::nullopt;
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getVersion(): {}", m_id, e.what() );
	return std::nullopt;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getVersion(): unknown exception", m_id );
	return std::nullopt;
}

std::optional< GameMetadata > RecordData::getLatestVersion( Transaction transaction )
try
{
	ZoneScoped;
	const auto versions { getVersions( transaction ) };
	if ( versions.empty() )
		return std::nullopt;
	else
		return versions.at( 0 );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getLatestVersion(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return std::nullopt;
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getLatestVersion(): {}", m_id, e.what() );
	return std::nullopt;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getLatestVersion(): unknown exception", m_id );
	return std::nullopt;
}

std::vector< GameMetadata > RecordData::getVersions( Transaction transaction )
try
{
	ZoneScoped;
	std::vector< GameMetadata > metadata;
	transaction << "SELECT version FROM game_metadata WHERE record_id = ? ORDER BY date_added DESC" << m_id >>
		[ this, &metadata, &transaction ]( std::string version )
	{ metadata.emplace_back( m_id, QString::fromStdString( std::move( version ) ), transaction ); };

	return metadata;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getVersions(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getVersions(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getVersions(): unknown exception", m_id );
	return {};
}

void RecordData::addVersion(
	QString version,
	std::filesystem::path game_path,
	std::filesystem::path exec_path,
	const uint64_t folder_size,
	bool in_place,
	Transaction transaction )
try
{
	ZoneScoped;
	spdlog::debug(
		"RecordData::addVersion() - Adding version {} to record {}:{}",
		version.toStdString(),
		m_id,
		title.get( transaction ).toStdString() );
	//Check if version is already added
	const auto active_versions { getVersions( transaction ) };

	auto itter { std::find_if(
		active_versions.begin(),
		active_versions.end(),
		[ &version ]( const GameMetadata& other ) { return version == other.getVersionName(); } ) };
	if ( itter != active_versions.end() ) return;

	transaction
		<< "INSERT INTO game_metadata (record_id, version, game_path, exec_path, in_place, last_played, version_playtime, folder_size, date_added) VALUES (?, ?, ?, ?, ?, 0, 0, ?, ?)"
		<< m_id << version.toStdString() << game_path.string() << exec_path.string() << in_place << folder_size
		<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now().time_since_epoch() )
			   .count();

	transaction
		<< "INSERT INTO data_change (timestamp, delta) VALUES (?, ?)"
		<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now().time_since_epoch() )
			   .count()
		<< folder_size;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::addVersion({}): {} [{},{}]",
		m_id,
		version.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::addVersion({}): {}", m_id, version.toStdString(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::addVersion({}): unknown exception", m_id, version.toStdString() );
}

void RecordData::removeVersion( const GameMetadata& version, Transaction transaction )
try
{
	ZoneScoped;
	const auto active_versions { getVersions( transaction ) };

	auto itter { std::find( active_versions.begin(), active_versions.end(), version ) };
	if ( itter == active_versions.end() ) return;

	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ?" << m_id
				<< version.getVersionName().toStdString();

	transaction
		<< "INSERT INTO data_change (timestamp, delta) VALUES (?, ?)"
		<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now().time_since_epoch() )
			   .count()
		<< ( 0 - static_cast< std::int64_t >( version.getFolderSize() ) );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::removeVersion({}): {} [{},{}]",
		m_id,
		version.getVersionName().toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::removeVersion({}): {}", m_id, version.getVersionName().toStdString(), e.what() );
}
catch ( ... )
{
	spdlog::
		error( "{}->RecordData::removeVersion({}): unknown exception", m_id, version.getVersionName().toStdString() );
}

RecordData::RecordData( QString title_in, QString creator_in, QString engine_in, Transaction transaction )
try
{
	ZoneScoped;
	RecordID record_id { 0 };
	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
				<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
		>> [ & ]( const RecordID id ) { record_id = id; };

	if ( record_id != 0 )
	{
		transaction.abort();
		throw RecordAlreadyExists( Record( record_id ) );
	}

	transaction
			<< "INSERT INTO records (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, 0, 0) RETURNING record_id"
			<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
		>> [ & ]( const RecordID id ) { m_id = id; };
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): {} [{},{}]",
		title_in.toStdString(),
		creator_in.toStdString(),
		engine_in.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): {}",
		title_in.toStdString(),
		creator_in.toStdString(),
		engine_in.toStdString(),
		e.what() );
}
catch ( ... )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): unknown exception",
		title_in.toStdString(),
		creator_in.toStdString(),
		engine_in.toStdString() );
}

QString RecordData::getDesc( Transaction transaction ) const
try
{
	ZoneScoped;
	try
	{
		std::string str;
		transaction << "SELECT notes FROM game_notes WHERE record_id = ?" << m_id >> str;
		return QString::fromStdString( str );
	}
	catch ( sqlite::exceptions::no_rows& e )
	{
		return {};
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getDesc(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getDesc(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getDesc(): unknown exception", m_id );
	return {};
}

void RecordData::setDesc( const QString& str, Transaction transaction )
try
{
	ZoneScoped;
	bool found { false };
	transaction << "SELECT count(*) FROM game_notes WHERE record_id = ?" << m_id >> found;

	if ( found )
		transaction << "UPDATE game_notes SET notes = ? WHERE record_id = ?" << str.toStdString() << m_id;
	else
		transaction << "INSERT INTO game_notes (record_id, notes) VALUES (?, ?)" << m_id << str.toStdString();
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::setDesc({}): {} [{},{}]", m_id, str.toStdString(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::setDesc({}): {}", m_id, str.toStdString(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setDesc({}): unknown exception", m_id, str.toStdString() );
}

std::vector< QString > RecordData::getAllTags( Transaction transaction ) const
try
{
	ZoneScoped;
	std::vector< QString > tags;
	transaction << "SELECT tag FROM full_tags WHERE record_id = ?" << m_id >> [ &tags ]( const std::string& str )
	{ tags.emplace_back( QString::fromStdString( str ) ); };
	return tags;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getAllTags(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getAllTags(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getAllTags(): unknown exception", m_id );
	return {};
}

std::vector< QString > RecordData::getUserTags( Transaction transaction ) const
try
{
	ZoneScoped;
	std::vector< QString > tags;
	transaction << "SELECT tag FROM tag_mappings NATURAL JOIN tags WHERE record_id = ?" << m_id >>
		[ & ]( const std::string str ) { tags.emplace_back( QString::fromStdString( str ) ); };

	return tags;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getUserTags(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getUserTags(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getUserTags(): unknown exception", m_id );
	return {};
}

std::size_t strToTagID( const QString str, Transaction transaction = Transaction( Autocommit ) )
try
{
	ZoneScoped;
	std::size_t id { 0 };
	transaction << "SELECT tag_id FROM tags WHERE tag = ?" << str.toStdString() >> id;
	return id;
}
catch ( [[maybe_unused]] sqlite::exceptions::no_rows& e )
{
	return 0;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "strToTagID({}): {} [{},{}]", str.toStdString(), e.what(), e.get_code(), e.get_sql() );
	return 0;
}
catch ( const std::exception& e )
{
	spdlog::error( "strToTagID({}): {}", str.toStdString(), e.what() );
	return 0;
}
catch ( ... )
{
	spdlog::error( "strToTagID({}): unknown exception", str.toStdString() );
	return 0;
}

void RecordData::addUserTag( const QString str, Transaction transaction )
try
{
	ZoneScoped;
	if ( auto tag_id = strToTagID( str, transaction ); tag_id != 0 )
	{
		//Tag exists
		transaction << "INSERT INTO tag_mappings (record_id, tag_id) VALUES (?, ?)" << m_id << tag_id;
	}
	else
	{
		transaction << "INSERT INTO tags (tag) VALUES (?) RETURNING tag_id" << str.toStdString() >> tag_id;
		transaction << "INSERT INTO tag_mappings (record_id, tag_id) VALUES (?, ?)" << m_id << tag_id;
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::addUserTag({}): {} [{},{}]", m_id, str.toStdString(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::addUserTag({}): {}", m_id, str.toStdString(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::addUserTag({}): unknown exception", m_id, str.toStdString() );
}

void RecordData::removeUserTag( const QString str, Transaction transaction )
try
{
	ZoneScoped;
	const auto tag_id { strToTagID( str, transaction ) };
	if ( tag_id == 0 )
		return;
	else
	{
		transaction << "DELETE FROM tag_mappings WHERE record_id = ? AND tag_id = ?;" << m_id << tag_id;
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::removeUserTag({}): {} [{},{}]", m_id, str.toStdString(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::removeUserTag({}): {}", m_id, str.toStdString(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::removeUserTag({}): unknown exception", m_id, str.toStdString() );
}

RecordID recordID( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
try
{
	ZoneScoped;
	RecordID record_id { 0 };

	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << title.toStdString()
				<< creator.toStdString() << engine.toStdString()
		>> [ &record_id ]( [[maybe_unused]] const RecordID id ) { record_id = id; };

	return record_id;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"recordID({}, {}, {}): {} [{},{}]",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return 0;
}
catch ( const std::exception& e )
{
	spdlog::
		error( "recordID({}, {}, {}): {}", title.toStdString(), creator.toStdString(), engine.toStdString(), e.what() );
	return 0;
}
catch ( ... )
{
	spdlog::error(
		"recordID({}, {}, {}): unknown exception", title.toStdString(), creator.toStdString(), engine.toStdString() );
	return 0;
}

bool recordExists( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
try
{
	ZoneScoped;
	return recordID( title, creator, engine, transaction );
}
catch ( [[maybe_unused]] sqlite::exceptions::no_rows& e )
{
	return false;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"recordExists({}, {}, {}): {} [{},{}]",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return false;
}
catch ( const std::exception& e )
{
	spdlog::error(
		"recordExists({}, {}, {}): {}", title.toStdString(), creator.toStdString(), engine.toStdString(), e.what() );
	return false;
}
catch ( ... )
{
	spdlog::error(
		"recordExists({}, {}, {}): unknown exception",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString() );
	return false;
}

RecordBanner RecordData::banners()
{
	return RecordBanner( *this );
}

RecordPreviews RecordData::previews()
{
	return RecordPreviews( *this );
}
