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

RecordData::RecordData( const RecordID id ) : m_id( id )
{
	ZoneScoped;
	if ( id == 0 ) throw std::runtime_error( "Invalid record id" );

	bool exists;
	RapidTransaction transaction;
	transaction << "SELECT record_id FROM records WHERE record_id = ?" << m_id >>
		[ & ]( [[maybe_unused]] const RecordID ) noexcept { exists = true; };

	if ( !exists ) throw std::runtime_error( "Record does not exist" );
}

std::optional< GameMetadata > RecordData::getVersion( const QString version_name )
{
	ZoneScoped;
	const auto versions { getVersions() };

	const auto idx { std::find_if(
		versions.begin(),
		versions.end(),
		[ &version_name ]( const GameMetadata& version ) { return version.getVersionName() == version_name; } ) };

	if ( idx == versions.end() )
		return std::nullopt;
	else
		return *idx;
}

std::optional< GameMetadata > RecordData::getLatestVersion()
{
	ZoneScoped;
	const auto versions { getVersions() };
	if ( versions.empty() )
		return std::nullopt;
	else
		return versions.at( 0 );
}

std::vector< GameMetadata > RecordData::getVersions()
{
	ZoneScoped;
	RapidTransaction transaction;
	std::vector< GameMetadata > metadata;
	transaction << "SELECT version FROM game_metadata WHERE record_id = ? ORDER BY date_added DESC" << m_id >>
		[ this, &metadata, &transaction ]( std::string version )
	{ metadata.emplace_back( m_id, QString::fromStdString( std::move( version ) ) ); };

	return metadata;
}

void RecordData::addVersion(
	QString version,
	std::filesystem::path game_path,
	std::filesystem::path exec_path,
	const uint64_t folder_size,
	bool in_place )
{
	ZoneScoped;
	spdlog::debug(
		"RecordData::addVersion() - Adding version {} to record {}:{}",
		version.toStdString(),
		m_id,
		get< "title" >().toStdString() );
	//Check if version is already added
	const auto active_versions { getVersions() };

	auto itter { std::find_if(
		active_versions.begin(),
		active_versions.end(),
		[ &version ]( const GameMetadata& other ) { return version == other.getVersionName(); } ) };
	if ( itter != active_versions.end() ) return;

	RapidTransaction transaction;
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

void RecordData::removeVersion( const GameMetadata& version )
{
	ZoneScoped;
	const auto active_versions { getVersions() };

	auto itter { std::find( active_versions.begin(), active_versions.end(), version ) };
	if ( itter == active_versions.end() ) return;

	RapidTransaction transaction;
	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ?" << m_id
				<< version.getVersionName().toStdString();

	transaction
		<< "INSERT INTO data_change (timestamp, delta) VALUES (?, ?)"
		<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now().time_since_epoch() )
			   .count()
		<< ( 0 - static_cast< std::int64_t >( version.getFolderSize() ) );
}

RecordData::RecordData( QString title_in, QString creator_in, QString engine_in )
{
	ZoneScoped;
	RapidTransaction transaction;
	RecordID record_id { 0 };
	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
				<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
		>> [ & ]( const RecordID id ) noexcept { record_id = id; };

	if ( record_id != 0 )
	{
		throw RecordAlreadyExists( Record( record_id ) );
	}

	transaction
			<< "INSERT INTO records (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, 0, 0) RETURNING record_id"
			<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
		>> [ & ]( const RecordID id ) noexcept { m_id = id; };
}

QString RecordData::getDesc() const
{
	ZoneScoped;
	try
	{
		RapidTransaction transaction;
		std::string str;
		transaction << "SELECT notes FROM game_notes WHERE record_id = ?" << m_id >> str;
		return QString::fromStdString( str );
	}
	catch ( const NoRows& e )
	{
		return {};
	}
}

void RecordData::setDesc( const QString& str )
{
	ZoneScoped;
	bool found { false };
	RapidTransaction transaction;
	transaction << "SELECT count(*) FROM game_notes WHERE record_id = ?" << m_id >> found;

	if ( found )
		transaction << "UPDATE game_notes SET notes = ? WHERE record_id = ?" << str.toStdString() << m_id;
	else
		transaction << "INSERT INTO game_notes (record_id, notes) VALUES (?, ?)" << m_id << str.toStdString();
}

std::vector< QString > RecordData::getAllTags() const
{
	ZoneScoped;
	std::vector< QString > tags;
	RapidTransaction transaction;
	transaction << "SELECT tag FROM full_tags WHERE record_id = ?" << m_id >> [ &tags ]( std::string str ) noexcept
	{ tags.emplace_back( QString::fromStdString( std::move( str ) ) ); };
	return tags;
}

std::vector< QString > RecordData::getUserTags() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::vector< QString > tags;
	transaction << "SELECT tag FROM tag_mappings NATURAL JOIN tags WHERE record_id = ?" << m_id >>
		[ &tags ]( std::string str ) noexcept { tags.emplace_back( QString::fromStdString( std::move( str ) ) ); };

	return tags;
}

std::size_t strToTagID( const QString str )
{
	ZoneScoped;
	RapidTransaction transaction;
	std::size_t id { 0 };
	transaction << "SELECT tag_id FROM tags WHERE tag = ?" << str.toStdString() >> id;
	return id;
}

void RecordData::addUserTag( const QString str )
{
	ZoneScoped;
	RapidTransaction transaction;
	if ( auto tag_id = strToTagID( str ); tag_id != 0 )
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

void RecordData::removeUserTag( const QString str )
{
	ZoneScoped;
	const auto tag_id { strToTagID( str ) };
	if ( tag_id == 0 )
		return;
	else
	{
		RapidTransaction transaction;
		transaction << "DELETE FROM tag_mappings WHERE record_id = ? AND tag_id = ?;" << m_id << tag_id;
	}
}

RecordID recordID( const QString& title, const QString& creator, const QString& engine )
{
	ZoneScoped;
	RecordID record_id { 0 };

	RapidTransaction transaction;
	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << title.toStdString()
				<< creator.toStdString() << engine.toStdString()
		>> [ &record_id ]( [[maybe_unused]] const RecordID id ) noexcept { record_id = id; };

	return record_id;
}

bool recordExists( const QString& title, const QString& creator, const QString& engine )
try
{
	ZoneScoped;
	return recordID( title, creator, engine );
}
catch ( [[maybe_unused]] const NoRows& e )
{
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
