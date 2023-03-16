//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"

#include <iostream>

#include <QPixmapCache>

#include "h95/SHA256.hpp"
#include "h95/config.hpp"
#include "h95/database/Database.hpp"
#include "h95/imageManager.hpp"

enum PreviewType
{
	PREVIEW_UNKNOWN = 0,
	PREVIEW_BANNER,
	PREVIEW_PREVIEW
};

RecordData::RecordData( const RecordID id, Transaction transaction ) : m_id( id )
{
	bool found { false };

	transaction << "SELECT title, creator, engine, last_played_r, total_playtime FROM records WHERE record_id = ?" << id
		>> [ this, &found ](
			   std::string title_in,
			   std::string creator_in,
			   std::string engine_in,
			   uint64_t last_played_in,
			   uint32_t total_playtime_in )
	{
		found = true;

		m_title = QString::fromStdString( std::move( title_in ) );
		m_creator = QString::fromStdString( std::move( creator_in ) );
		m_engine = QString::fromStdString( std::move( engine_in ) );
		m_last_played = last_played_in;
		m_total_playtime = total_playtime_in;
	};

	if ( !found ) throw InvalidRecordID( id );

	std::vector< GameMetadata > metadata;

	transaction
			<< "SELECT version, game_path, exec_path, in_place, last_played, version_playtime FROM game_metadata WHERE record_id = ?"
			<< id
		>> [ &metadata ](
			   std::string version,
			   std::string game_path,
			   std::string exec_path,
			   bool in_place,
			   uint64_t last_played,
			   uint32_t version_playtime )
	{
		metadata.emplace_back(
			QString::fromStdString( std::move( version ) ),
			std::move( game_path ),
			std::move( exec_path ),
			in_place,
			last_played,
			version_playtime );
	};

	const std::filesystem::path image_path { getSettings< QString >( "paths/images", "./data/images" ).toStdString() };

	transaction << "SELECT type, path FROM images WHERE record_id = ? " << id >>
		[ this, &image_path ]( const uint16_t type, std::string path )
	{
		switch ( static_cast< PreviewType >( type ) )
		{
			case PREVIEW_PREVIEW:
				{
					m_previews.emplace_back( image_path / path );
				}
			case PREVIEW_BANNER:
				{
					m_banner = image_path / path;
				}
			case PREVIEW_UNKNOWN:
				break;
		}
	};
}

RecordID RecordData::getID() const
{
	return m_id;
}

const QString& RecordData::getTitle() const
{
	return m_title;
}

const QString& RecordData::getCreator() const
{
	return m_creator;
}

const QString& RecordData::getEngine() const
{
	return m_engine;
}

uint64_t RecordData::getLastPlayed() const
{
	return m_last_played;
}

uint32_t RecordData::getTotalPlaytime() const
{
	return m_total_playtime;
}

const std::vector< GameMetadata >& RecordData::getVersions()
{
	return m_versions;
}

const std::filesystem::path& RecordData::getBannerPath() const
{
	return m_banner;
}

QPixmap RecordData::getBanner() const
{
	return QPixmap { QString::fromStdString( m_banner.string() ) };
}

const std::vector< std::filesystem::path >& RecordData::getPreviewPaths() const
{
	return m_previews;
}

std::vector< QPixmap > RecordData::getPreviews() const
{
	std::vector< QPixmap > images;

	for ( const auto& link : m_previews )
	{
		images.emplace_back( QString::fromStdString( link.string() ) );
	}

	return images;
}

void RecordData::setTitle( QString new_title, Transaction transaction )
{
	m_title = std::move( new_title );

	transaction << "UPDATE records SET title = ? WHERE record_id = ? " << new_title.toStdString() << m_id;

	emit dataChanged();
	emit titleChanged( m_title );
}

void RecordData::setCreator( QString new_creator, Transaction transaction )
{
	m_creator = std::move( new_creator );

	transaction << "UPDATE records SET creator = ? WHERE record_id = ?" << new_creator.toStdString() << m_id;

	emit dataChanged();
	emit creatorChanged( m_creator );
}

void RecordData::setEngine( QString new_engine, Transaction transaction )
{
	m_engine = std::move( new_engine );

	transaction << "UPDATE records SET engine = ? WHERE record_id = ?" << new_engine.toStdString() << m_id;

	emit dataChanged();
	emit engineChanged( m_engine );
}

void RecordData::setLastPlayed( const uint64_t time, Transaction transaction )
{
	m_last_played = time;

	transaction << "UPDATE records SET last_played_r = ? WHERE record_id = ?" << time << m_id;

	emit dataChanged();
	emit lastPlayedChanged( time );
}

void RecordData::setTotalPlaytime( const uint32_t time, Transaction transaction )
{
	m_total_playtime = time;

	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time << m_id;

	emit dataChanged();
	emit totalPlaytimeChanged( time );
}

void RecordData::addVersion( const GameMetadata& version, Transaction transaction )
{
	//Check if version is already added
	auto itter = std::find( m_versions.begin(), m_versions.end(), version );
	if ( itter != m_versions.end() ) return;

	m_versions.emplace_back( version );

	transaction
		<< "INSERT INTO game_metadata (record_id, version, game_path, exec_path, in_place, last_played, version_playtime) VALUES (?, ?, ?, ?, ?, ?, ?)"
		<< m_id << version.m_version << version.m_game_path.string() << version.m_exec_path.string()
		<< version.m_in_place << version.m_last_played << version.m_total_playtime;

	emit dataChanged();
	emit versionsChanged( m_versions );
}

void RecordData::removeVersion( const GameMetadata& version, Transaction transaction )
{
	auto itter = std::find( m_versions.begin(), m_versions.end(), version );
	if ( itter == m_versions.end() )
		return;
	else
		m_versions.erase( itter );

	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ? AND game_path = ? AND exec_path = ?"
				<< m_id << version.m_version << version.m_game_path.string() << version.m_exec_path.string();

	emit dataChanged();
	emit versionsChanged( m_versions );
}

void RecordData::sync( Transaction transaction )
{
	new ( this ) RecordData( m_id, transaction );
}

RecordData::RecordData(
	QString title,
	QString creator,
	QString engine,
	const std::uint64_t last_played,
	const std::uint32_t total_playtime,
	std::vector< GameMetadata > versions,
	std::filesystem::path banner,
	std::vector< std::filesystem::path > previews,
	Transaction transaction ) :
  m_title( std::move( title ) ),
  m_creator( std::move( creator ) ),
  m_engine( std::move( engine ) ),
  m_last_played( last_played ),
  m_total_playtime( total_playtime ),
  m_versions( std::move( versions ) ),
  m_banner( std::move( banner ) ),
  m_previews( std::move( previews ) )
{
	try
	{
		RecordID record_id { 0 };
		transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << m_title
					<< m_creator << m_engine
			>> [ & ]( const RecordID id ) { record_id = id; };

		if ( record_id != 0 )
		{
			transaction.abort();
			throw RecordAlreadyExists( Record( record_id ) );
		}

		transaction
				<< "INSERT INTO records (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, ?, ?) RETURNING record_id"
				<< m_title << m_creator << m_engine << m_last_played << m_total_playtime
			>> [ & ]( const RecordID id ) { m_id = id; };

		//Handle banner stuff
		transaction << "INSERT INTO images (record_id, type, path) VALUES (?, ?, ?)" << m_id << IMAGE_BANNER
					<< m_banner.string();

		for ( const auto& preview : m_previews )
			transaction << "INSERT INTO images (record_id, type, path) VALUES (?, ?, ?)" << m_id << IMAGE_PREVIEW
						<< preview.string();
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::error( "{}", e.get_sql() );
		std::rethrow_exception( std::current_exception() );
	}
}
