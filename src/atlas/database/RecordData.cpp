//
// Created by kj16609 on 1/15/23.
//

#include "RecordData.hpp"

#include <iostream>

#include <QPixmapCache>

#include <tracy/Tracy.hpp>

#include "Record.hpp"
#include "atlas/config.hpp"
#include "atlas/database/Database.hpp"
#include "atlas/database/GameMetadata.hpp"
#include "atlas/imageManager.hpp"

RecordData::RecordData( const RecordID id, Transaction transaction ) : m_id( id )
{
	ZoneScoped;

	bool exists;
	transaction << "SELECT record_id FROM records WHERE record_id = ?" << m_id >> exists;

	if ( !exists ) throw std::runtime_error( "Record does not exist" );
}

RecordID RecordData::getID() const
{
	ZoneScoped;
	return m_id;
}

const QString RecordData::getTitle( Transaction transaction ) const
{
	ZoneScoped;
	std::string title;
	transaction << "SELECT title FROM records WHERE record_id = ?" << m_id >> title;
	return QString::fromStdString( title );
}

const QString RecordData::getCreator( Transaction transaction ) const
{
	ZoneScoped;
	std::string creator;
	transaction << "SELECT creator FROM records WHERE record_id = ?" << m_id >> creator;
	return QString::fromStdString( creator );
}

const QString RecordData::getEngine( Transaction transaction ) const
{
	ZoneScoped;
	std::string engine;
	transaction << "SELECT engine FROM records WHERE record_id = ?" << m_id >> engine;
	return QString::fromStdString( engine );
}

uint64_t RecordData::getLastPlayed( Transaction transaction ) const
{
	ZoneScoped;
	uint64_t last_played;
	transaction << "SELECT last_played_r FROM records WHERE record_id = ?" << m_id >> last_played;
	return last_played;
}

uint32_t RecordData::getTotalPlaytime( Transaction transaction ) const
{
	ZoneScoped;
	uint32_t total_playtime;
	transaction << "SELECT total_playtime FROM records WHERE record_id = ?" << m_id >> total_playtime;
	return total_playtime;
}

std::optional< GameMetadata > RecordData::getVersion( const QString version_name, Transaction transaction )
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

std::optional< GameMetadata > RecordData::getLatestVersion( Transaction transaction )
{
	ZoneScoped;
	const auto versions { getVersions( transaction ) };
	if ( versions.empty() )
		return std::nullopt;
	else
		return versions.at( 0 );
}

std::vector< GameMetadata > RecordData::getVersions( Transaction transaction )
{
	ZoneScoped;
	std::vector< GameMetadata > metadata;
	transaction << "SELECT version FROM game_metadata WHERE record_id = ? ORDER BY date_added DESC" << m_id >>
		[ this, &metadata, &transaction ]( std::string version )
	{ metadata.emplace_back( m_id, QString::fromStdString( std::move( version ) ), transaction ); };

	return metadata;
}

const std::filesystem::path RecordData::getBannerPath( Transaction transaction ) const
try
{
	ZoneScoped;
	std::string banner_path;
	transaction << "SELECT path FROM banners WHERE record_id = ?" << m_id >> banner_path;
	return { banner_path };
}
catch ( ... )
{
	return {};
}

QPixmap RecordData::getBanner( Transaction transaction ) const
{
	ZoneScoped;
	if ( !std::filesystem::exists( getBannerPath( transaction ) ) )
		return {};
	else
		return QPixmap { QString::fromStdString( getBannerPath( transaction ).string() ) };
}

QPixmap RecordData::getBanner( const int width, const int height, const bool expanding, Transaction transaction ) const
{
	ZoneScoped;
	const auto key { QString::fromStdString( getBannerPath( transaction ).filename().string() )
		             + QString::number( width ) + "x" + QString::number( height ) };

	QPixmap banner;
	if ( QPixmapCache::find( key, &banner ) )
		return banner;
	else
	{
		banner = getBanner( transaction );
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
}

const std::vector< std::filesystem::path > RecordData::getPreviewPaths( Transaction transaction ) const
try
{
	ZoneScoped;
	std::vector< std::string > preview_strs;
	transaction << "SELECT path FROM previews WHERE record_id = ?" << m_id >> [ & ]( const std::string str )
	{ preview_strs.emplace_back( str ); };

	std::vector< std::filesystem::path > previews;

	for ( const auto& preview : preview_strs ) previews.emplace_back( preview );

	return previews;
}
catch ( ... )
{
	return {};
}

std::vector< QPixmap > RecordData::getPreviews( Transaction transaction ) const
{
	ZoneScoped;
	std::vector< QPixmap > images;

	for ( const auto& link : getPreviewPaths( transaction ) )
	{
		images.emplace_back( QString::fromStdString( link.string() ) );
	}

	return images;
}

void RecordData::setTitle( QString new_title, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET title = ? WHERE record_id = ? " << new_title.toStdString() << m_id;
}

void RecordData::setCreator( QString new_creator, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET creator = ? WHERE record_id = ?" << new_creator.toStdString() << m_id;
}

void RecordData::setEngine( QString new_engine, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET engine = ? WHERE record_id = ?" << new_engine.toStdString() << m_id;
}

void RecordData::setLastPlayed( const uint64_t time, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET last_played_r = ? WHERE record_id = ?" << time << m_id;
}

void RecordData::addPlaytime( const std::uint32_t time, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time + getTotalPlaytime( transaction )
				<< m_id;
}

void RecordData::setTotalPlaytime( const uint32_t time, Transaction transaction )
{
	ZoneScoped;
	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time << m_id;
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
		"Adding version {} to record {}:{}", version.toStdString(), m_id, getTitle( transaction ).toStdString() );
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
	const auto active_versions { getVersions( transaction ) };

	auto itter { std::find( active_versions.begin(), active_versions.end(), version ) };
	if ( itter == active_versions.end() ) return;

	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ? AND game_path = ? AND exec_path = ?"
				<< m_id << version.getVersionName().toStdString() << version.getPath( transaction ).string()
				<< version.getExecPath( transaction ).string();
}

void RecordData::setBanner( const std::filesystem::path& path, Transaction transaction )
{
	ZoneScoped;
	spdlog::debug( "Setting banner to {} for record_id {}", path, m_id );

	//Move banner to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	//Check if it exists
	if ( new_path == getBannerPath( transaction ) )
	{
		transaction << "UPDATE banner SET path = ? WHERE record_id = ?" << new_path.string() << m_id;
	}
	else
	{
		transaction << "INSERT INTO banners (record_id, path) VALUES (?, ?)" << m_id << new_path.string();
	}
}

void RecordData::addPreview( const std::filesystem::path& path, Transaction transaction )
{
	ZoneScoped;
	//Move preview to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	const auto active_previews { getPreviewPaths( transaction ) };

	//Check if it exists
	if ( std::find( active_previews.begin(), active_previews.end(), new_path ) != active_previews.end() )
	{
		spdlog::debug( "Preview already exists, not adding" );
		return;
	}
	else
	{
		transaction << "INSERT INTO previews (record_id, path) VALUES (?, ?)" << m_id << new_path.string();
	}
}

void RecordData::sync( Transaction transaction )
{
	ZoneScoped;
	new ( this ) RecordData( m_id, transaction );
}

RecordData::RecordData( QString title, QString creator, QString engine, Transaction transaction )
{
	ZoneScoped;
	try
	{
		RecordID record_id { 0 };
		transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
					<< title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [ & ]( const RecordID id ) { record_id = id; };

		if ( record_id != 0 )
		{
			transaction.abort();
			throw RecordAlreadyExists( Record( record_id ) );
		}

		transaction
				<< "INSERT INTO records (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, 0, 0) RETURNING record_id"
				<< title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [ & ]( const RecordID id ) { m_id = id; };
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::error( "{}", e.get_sql() );
		std::rethrow_exception( std::current_exception() );
	}
}

QPixmap RecordData::getBanner( const QSize size, const bool expanding, Transaction transaction ) const
{
	ZoneScoped;
	return getBanner( size.width(), size.height(), expanding, transaction );
}

QString RecordData::getDesc( Transaction transaction ) const
{
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

void RecordData::setDesc( const QString& str, Transaction transaction )
{
	bool found { false };
	transaction << "SELECT count(*) FROM game_notes WHERE record_id = ?" << m_id >> found;

	if ( found )
		transaction << "UPDATE game_notes SET notes = ? WHERE record_id = ?" << str.toStdString() << m_id;
	else
		transaction << "INSERT INTO game_notes (record_id, notes) VALUES (?, ?)" << m_id << str.toStdString();
}

std::vector< QString > RecordData::getAllTags( Transaction transaction ) const
{
	std::vector< QString > tags;
	transaction << "SELECT tag FROM full_tags WHERE record_id = ?" << m_id >> [ &tags ]( const std::string& str )
	{ tags.emplace_back( QString::fromStdString( str ) ); };
	return tags;
}

std::vector< QString > RecordData::getUserTags( Transaction transaction ) const
{
	std::vector< QString > tags;
	transaction << "SELECT tag FROM tag_mappings NATURAL JOIN tags WHERE record_id = ?" << m_id >>
		[ & ]( const std::string str ) { tags.emplace_back( QString::fromStdString( str ) ); };

	return tags;
}

std::size_t strToTagID( const QString str, Transaction transaction = Transaction( Autocommit ) )
try
{
	std::size_t id { 0 };
	transaction << "SELECT tag_id FROM tags WHERE tag = ?" << str.toStdString() >> id;
	return id;
}
catch ( [[maybe_unused]] sqlite::exceptions::no_rows& e )
{
	return 0;
}

void RecordData::addUserTag( const QString str, Transaction transaction )
try
{
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
catch ( sqlite::exceptions::constraint_unique& e )
{
	spdlog::warn( "addTag: Violated unique constraint: \"{}\"", e.get_sql() );
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::warn( "addTag: Failed to add tag to {}: {}", m_id, e.errstr() );
}

void RecordData::removeUserTag( const QString str, Transaction transaction )
{
	const auto tag_id { strToTagID( str, transaction ) };
	if ( tag_id == 0 )
		return;
	else
	{
		spdlog::warn( "Removing user tag {} from {} with id {}", str, m_id, tag_id );
		transaction << "DELETE FROM tag_mappings WHERE record_id = ? AND tag_id = ?;" << m_id << tag_id;
	}
}

RecordID recordID( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
{
	ZoneScoped;
	RecordID record_id { 0 };

	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << title.toStdString()
				<< creator.toStdString() << engine.toStdString()
		>> [ &record_id ]( [[maybe_unused]] const RecordID id ) { record_id = id; };

	return record_id;
}

bool recordExists( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
{
	ZoneScoped;
	return recordID( title, creator, engine, transaction );
}
