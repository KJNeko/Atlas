//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"

#include <iostream>

#include <QPixmapCache>

#include <tracy/Tracy.hpp>

#include "atlas/config.hpp"
#include "atlas/database/Database.hpp"
#include "atlas/imageManager.hpp"

enum PreviewType
{
	PREVIEW_UNKNOWN = 0,
	PREVIEW_BANNER,
	PREVIEW_PREVIEW
};

RecordData::RecordData( const RecordID id, Transaction transaction ) : m_id( id )
{
	ZoneScoped;
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

	transaction
			<< "SELECT version, game_path, exec_path, in_place, last_played, version_playtime, folder_size FROM game_metadata WHERE record_id = ?"
			<< id
		>> [ this, id ](
			   std::string version,
			   std::string game_path,
			   std::string exec_path,
			   bool in_place,
			   uint64_t last_played,
			   uint32_t version_playtime,
			   uint64_t folder_size )
	{
		m_versions.emplace_back(
			*this,
			QString::fromStdString( std::move( version ) ),
			std::move( game_path ),
			std::move( exec_path ),
			in_place,
			last_played,
			version_playtime,
			folder_size );
	};

	const std::filesystem::path image_path { config::paths::images::getPath() };

	transaction << "SELECT type, path FROM images WHERE record_id = ? " << id >>
		[ this, &image_path ]( const uint16_t type, std::string path )
	{
		switch ( static_cast< PreviewType >( type ) )
		{
			case PREVIEW_PREVIEW:
				{
					m_previews.emplace_back( image_path / path );
					break;
				}
			case PREVIEW_BANNER:
				{
					m_banner = image_path / path;
					break;
				}
			case PREVIEW_UNKNOWN:
				[[fallthrough]];
			default:
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

GameMetadata& RecordData::getVersion( const QString version_name )
{
	ZoneScoped;

	const auto idx { std::find_if(
		m_versions.begin(),
		m_versions.end(),
		[ &version_name ]( const GameMetadata& version ) { return version.getVersionName() == version_name; } ) };

	if ( idx == m_versions.end() )
		throw std::runtime_error( fmt::format( "getVersion: Version {} not found", version_name ) );
	else
		return *idx;
}

GameMetadata& RecordData::getLatestVersion()
{
	return m_versions.at( m_versions.size() - 1 );
}

std::vector< GameMetadata >& RecordData::getVersions()
{
	return m_versions;
}

const std::filesystem::path& RecordData::getBannerPath() const
{
	return m_banner;
}

QPixmap RecordData::getBanner() const
{
	ZoneScoped;
	if ( !std::filesystem::exists( m_banner ) )
		return {};
	else
		return QPixmap { QString::fromStdString( m_banner.string() ) };
}

QPixmap RecordData::getBanner( int width, int height, bool expanding ) const
{
	ZoneScoped;
	const auto key { QString::fromStdString( m_banner.filename().string() ) + QString::number( width ) + "x"
		             + QString::number( height ) };

	QPixmap banner;
	if ( QPixmapCache::find( key, &banner ) )
		return banner;
	else
	{
		banner = getBanner();
		if ( banner.isNull() )
			return {};
		else
		{
			banner = banner.scaled(
				width,
				height,
				expanding ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio,
				Qt::SmoothTransformation );
			if ( !QPixmapCache::insert( key, banner ) )
				spdlog::warn( "failed to insert banner into cache with key: {}", key );
			return banner;
		}
	}
	return {};
}

const std::vector< std::filesystem::path >& RecordData::getPreviewPaths() const
{
	return m_previews;
}

std::vector< QPixmap > RecordData::getPreviews() const
{
	ZoneScoped;
	std::vector< QPixmap > images;

	for ( const auto& link : m_previews )
	{
		images.emplace_back( QString::fromStdString( link.string() ) );
	}

	return images;
}

void RecordData::setTitle( QString new_title, Transaction transaction )
{
	ZoneScoped;
	m_title = std::move( new_title );

	transaction << "UPDATE records SET title = ? WHERE record_id = ? " << new_title.toStdString() << m_id;

	emit dataChanged();
	emit titleChanged( m_title );
}

void RecordData::setCreator( QString new_creator, Transaction transaction )
{
	ZoneScoped;
	m_creator = std::move( new_creator );

	transaction << "UPDATE records SET creator = ? WHERE record_id = ?" << new_creator.toStdString() << m_id;

	emit dataChanged();
	emit creatorChanged( m_creator );
}

void RecordData::setEngine( QString new_engine, Transaction transaction )
{
	ZoneScoped;
	m_engine = std::move( new_engine );

	transaction << "UPDATE records SET engine = ? WHERE record_id = ?" << new_engine.toStdString() << m_id;

	emit dataChanged();
	emit engineChanged( m_engine );
}

void RecordData::setLastPlayed( const uint64_t time, Transaction transaction )
{
	ZoneScoped;
	m_last_played = time;

	transaction << "UPDATE records SET last_played_r = ? WHERE record_id = ?" << time << m_id;

	emit dataChanged();
	emit lastPlayedChanged( time );
}

void RecordData::addPlaytime( const std::uint32_t time, Transaction transaction )
{
	ZoneScoped;

	m_total_playtime += time;

	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << m_total_playtime << m_id;
}

void RecordData::setTotalPlaytime( const uint32_t time, Transaction transaction )
{
	ZoneScoped;
	m_total_playtime = time;

	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time << m_id;

	emit dataChanged();
	emit totalPlaytimeChanged( time );
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
	spdlog::info( "Adding version {} to record {}:{}", version.toStdString(), m_id, m_title.toStdString() );
	//Check if version is already added
	auto itter { std::find_if(
		m_versions.begin(),
		m_versions.end(),
		[ &version ]( const GameMetadata& other ) { return version == other.getVersionName(); } ) };
	if ( itter != m_versions.end() ) return;

	m_versions.emplace_back( *this, version, game_path, exec_path, in_place, 0, 0, folder_size );

	transaction
		<< "INSERT INTO game_metadata (record_id, version, game_path, exec_path, in_place, last_played, version_playtime, folder_size) VALUES (?, ?, ?, ?, ?, 0, 0, ?)"
		<< m_id << version.toStdString() << game_path.string() << exec_path.string() << in_place << folder_size;

	emit dataChanged();
	emit versionsChanged( m_versions );
}
catch ( std::exception& e )
{
	spdlog::error( "An exception was throw in addVersion: {}", e.what() );
	std::rethrow_exception( std::current_exception() );
}
catch ( ... )
{
	spdlog::error( "An unknown exception was thrown in addVersion!" );
	std::rethrow_exception( std::current_exception() );
}

void RecordData::removeVersion( const GameMetadata& version, Transaction transaction )
{
	ZoneScoped;
	auto itter { std::find( m_versions.begin(), m_versions.end(), version ) };
	if ( itter == m_versions.end() ) return;

	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ? AND game_path = ? AND exec_path = ?"
				<< m_id << version.getVersionName().toStdString() << version.getPath().string()
				<< version.getExecPath().string();

	m_versions.erase( itter );

	emit dataChanged();
	emit versionsChanged( m_versions );
}

void RecordData::setBanner( const std::filesystem::path& path, Transaction transaction )
{
	ZoneScoped;
	spdlog::info( "Setting baner to {}", path );

	//Move banner to image folder
	m_banner = imageManager::importImage( path );

	//Check if it exists
	bool found { false };
	transaction << "SELECT path FROM images WHERE record_id = ? AND type = ?" << m_id << PREVIEW_BANNER >> [ &found ]()
	{ found = true; };

	if ( found )
		transaction << "UPDATE images SET path = ? WHERE record_id = ? AND type = ?" << m_banner.string() << m_id
					<< PREVIEW_BANNER;
	else
		transaction << "INSERT INTO images (record_id, path, type) VALUES (?, ?, ?)" << m_id << m_banner.string()
					<< PREVIEW_BANNER;

	emit dataChanged();
	emit bannerChanged( getBanner() );
}

void RecordData::addPreview( const std::filesystem::path& path, Transaction transaction )
{
	ZoneScoped;
	//Move preview to image folder
	m_previews.emplace_back( imageManager::importImage( path ) );

	try
	{
		transaction << "INSERT INTO images (record_id, preview_path, type) VALUES (?, ?, ?)" << m_id
					<< m_previews.back().string() << PREVIEW_PREVIEW;
	}
	catch ( const std::exception& e )
	{
		spdlog::error( "Failed to add preview to database: {}", e.what() );
		//Undo adding to m_previews
		m_previews.pop_back();
	}

	emit dataChanged();
	emit previewsChanged( getPreviews() );
}

void RecordData::sync( Transaction transaction )
{
	ZoneScoped;
	new ( this ) RecordData( m_id, transaction );
}

RecordData::RecordData( QString title, QString creator, QString engine, Transaction transaction ) :
  m_title( std::move( title ) ),
  m_creator( std::move( creator ) ),
  m_engine( std::move( engine ) )
{
	ZoneScoped;
	try
	{
		RecordID record_id { 0 };
		transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
					<< m_title.toStdString() << m_creator.toStdString() << m_engine.toStdString()
			>> [ & ]( const RecordID id ) { record_id = id; };

		if ( record_id != 0 )
		{
			transaction.abort();
			throw RecordAlreadyExists( Record( record_id, transaction ) );
		}

		transaction
				<< "INSERT INTO records (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, ?, ?) RETURNING record_id"
				<< m_title.toStdString() << m_creator.toStdString() << m_engine.toStdString() << m_last_played
				<< m_total_playtime
			>> [ & ]( const RecordID id ) { m_id = id; };

		//Handle banner stuff
		if ( !m_banner.empty() )
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

RecordID recordID( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
{
	RecordID record_id { 0 };

	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << title.toStdString()
				<< creator.toStdString() << engine.toStdString()
		>> [ &record_id ]( [[maybe_unused]] const RecordID id ) { record_id = id; };

	return record_id;
}

bool recordExists( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
{
	return recordID( title, creator, engine, transaction );
}

//! imports a new record and returns it. Will return an existing record if the record already exists
Record importRecord( QString title, QString creator, QString engine, Transaction transaction )
{
	if ( recordExists( title, creator, engine, transaction ) )
		throw RecordAlreadyExists( Record(
			recordID( std::move( title ), std::move( creator ), std::move( engine ), transaction ), transaction ) );

	try
	{
		return Record( std::move( title ), std::move( creator ), std::move( engine ), transaction );
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Failed to import record: {}", e.what() );
		std::rethrow_exception( std::current_exception() );
	}
	catch ( ... )
	{
		spdlog::error( "Failed to import record: Unknown error" );
		std::rethrow_exception( std::current_exception() );
	}
}
