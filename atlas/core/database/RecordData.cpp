//
// Created by kj16609 on 1/15/23.
//

#include "RecordData.hpp"

#include <iostream>

#include <QPixmapCache>

#include "Record.hpp"
#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/database/GameMetadata.hpp"
#include "core/imageManager.hpp"

RecordData::RecordData( const RecordID id, Transaction transaction ) : m_id( id )
{
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

const QString RecordData::getTitle( Transaction transaction ) const
try
{
	std::string title;
	transaction << "SELECT title FROM records WHERE record_id = ?" << m_id >> title;
	return QString::fromStdString( title );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getTitle(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return QString( "ATLAS_ERROR" );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getTitle(): {}", m_id, e.what() );
	return QString( "ATLAS_ERROR" );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getTitle(): unknown exception", m_id );
	return QString( "ATLAS_ERROR" );
}

const QString RecordData::getCreator( Transaction transaction ) const
try
{
	std::string creator;
	transaction << "SELECT creator FROM records WHERE record_id = ?" << m_id >> creator;
	return QString::fromStdString( creator );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getCreator(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return QString( "ATLAS_ERROR" );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getCreator(): {}", m_id, e.what() );
	return QString( "ATLAS_ERROR" );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getCreator(): unknown exception", m_id );
	return QString( "ATLAS_ERROR" );
}

const QString RecordData::getEngine( Transaction transaction ) const
try
{
	std::string engine;
	transaction << "SELECT engine FROM records WHERE record_id = ?" << m_id >> engine;
	return QString::fromStdString( engine );
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getEngine(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return QString( "ATLAS_ERROR" );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getEngine(): {}", m_id, e.what() );
	return QString( "ATLAS_ERROR" );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getEngine(): unknown exception", m_id );
	return QString( "ATLAS_ERROR" );
}

uint64_t RecordData::getLastPlayed( Transaction transaction ) const
try
{
	uint64_t last_played;
	transaction << "SELECT last_played_r FROM records WHERE record_id = ?" << m_id >> last_played;
	return last_played;
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getLastPlayed(): {}", m_id, e.what() );
	return 0;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getLastPlayed(): unknown exception", m_id );
	return 0;
}

uint32_t RecordData::getTotalPlaytime( Transaction transaction ) const
try
{
	uint32_t total_playtime;
	transaction << "SELECT total_playtime FROM records WHERE record_id = ?" << m_id >> total_playtime;
	return total_playtime;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getTotalPlaytime(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return 0;
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getTotalPlaytime(): {}", m_id, e.what() );
	return 0;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getTotalPlaytime(): unknown exception", m_id );
	return 0;
}

std::optional< GameMetadata > RecordData::getVersion( const QString version_name, Transaction transaction )
try
{
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

const std::filesystem::path RecordData::getBannerPath( const BannerType type, Transaction transaction ) const
try
{
	std::string banner_path;
	transaction << "SELECT path FROM banners WHERE record_id = ? AND type = ? limit 1" << m_id
				<< static_cast< int >( type )
		>> banner_path;

	return { banner_path };
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::getBannerPath({}): {} [{},{}]",
		m_id,
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getBannerPath({}): {}", m_id, static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	//Hacky as fuck but it should work for 'solving' banners when one doesn't exist.
	if ( type > Error )
		return getBannerPath( static_cast< BannerType >( type - 1 ), transaction );
	else
		return {};
}

QPixmap RecordData::getBanner( const BannerType type, Transaction transaction ) const
try
{
	const auto path { getBannerPath( type, transaction ) };

	if ( path.empty() )
		return {};
	else
		return QPixmap { QString::fromStdString( path.string() ) };
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::getBanner({}): {} [{},{}]",
		m_id,
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getBanner({}): {}", m_id, static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getBanner({}): unknown exception", m_id, static_cast< int >( type ) );
	return {};
}

QPixmap RecordData::getBanner(
	const int width,
	const int height,
	const SCALE_TYPE aspect_ratio_mode,
	const BannerType type,
	Transaction transaction ) const
try
{
	const auto key { QString::fromStdString( getBannerPath( type, transaction ).filename().string() )
		             + QString::number( width ) + "x" + QString::number( height )
		             + QString::number( static_cast< unsigned int >( aspect_ratio_mode ) ) };

	//spdlog::info( key );

	QPixmap banner;
	if ( QPixmapCache::find( key, &banner ) )
		return banner;
	else
	{
		banner = getBanner( type, transaction );
		if ( banner.isNull() )
		{
			spdlog::warn(
				"Failed to get image for banner in record: {}, title: {}",
				m_id,
				getTitle( transaction ).toStdString() );
			return {};
		}
		else
		{
			auto banner_scaled {
				banner.scaled( width, height, Qt::AspectRatioMode( aspect_ratio_mode ), Qt::SmoothTransformation )

			};
			if ( aspect_ratio_mode == KEEP_ASPECT_RATIO_BY_EXPANDING )
			{
				//Rmove image outside of boundry and center image
				const int new_x { width == banner_scaled.width() ? 0 : ( banner_scaled.width() - width ) / 2 };
				const int new_y { height == banner_scaled.height() ? 0 : ( banner_scaled.height() - height ) / 2 };
				banner_scaled = banner_scaled.copy( new_x, new_y, width, height );
			}

			if ( !QPixmapCache::insert( key, banner_scaled ) )
				spdlog::warn( "failed to insert banner into cache with key: {}", key );
			return banner_scaled;
		}
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::getBanner({}): {} [{},{}]",
		m_id,
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getBanner({}): {}", m_id, static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getBanner({}): unknown exception", m_id, static_cast< int >( type ) );
	return {};
}

const std::vector< std::filesystem::path > RecordData::getPreviewPaths( Transaction transaction ) const
try
{
	std::vector< std::string > preview_strs;
	transaction << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_id >>
		[ & ]( const std::string str ) { preview_strs.emplace_back( str ); };

	std::vector< std::filesystem::path > previews;

	for ( const auto& preview : preview_strs ) previews.emplace_back( preview );

	return previews;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getPreviewPaths(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getPreviewPaths(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getPreviewPaths(): unknown exception", m_id );
	return {};
}

std::vector< QPixmap > RecordData::getPreviews( Transaction transaction ) const
try
{
	std::vector< QPixmap > images;

	for ( const auto& link : getPreviewPaths( transaction ) )
	{
		images.emplace_back( QString::fromStdString( link.string() ) );
	}

	return images;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::getPreviews(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::getPreviews(): {}", m_id, e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::getPreviews(): unknown exception", m_id );
	return {};
}

void RecordData::setTitle( QString new_title, Transaction transaction )
try
{
	transaction << "UPDATE records SET title = ? WHERE record_id = ? " << new_title.toStdString() << m_id;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::setTitle({}): {} [{},{}]",
		m_id,
		new_title.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setTitle({}): unknown exception", m_id, new_title.toStdString() );
}

void RecordData::setCreator( QString new_creator, Transaction transaction )
try
{
	transaction << "UPDATE records SET creator = ? WHERE record_id = ?" << new_creator.toStdString() << m_id;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setCreator({}): unknown exception", m_id, new_creator.toStdString() );
}

void RecordData::setEngine( QString new_engine, Transaction transaction )
try
{
	transaction << "UPDATE records SET engine = ? WHERE record_id = ?" << new_engine.toStdString() << m_id;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::setEngine({}): {} [{},{}]",
		m_id,
		new_engine.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setEngine({}): unknown exception", m_id, new_engine.toStdString() );
}

void RecordData::setLastPlayed( const uint64_t time, Transaction transaction )
try
{
	transaction << "UPDATE records SET last_played_r = ? WHERE record_id = ?" << time << m_id;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setLastPlayed({}): unknown exception", m_id, time );
}

void RecordData::addPlaytime( const std::uint32_t time, Transaction transaction )
try
{
	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time + getTotalPlaytime( transaction )
				<< m_id;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::addPlaytime({}): unknown exception", m_id, time );
}

void RecordData::setTotalPlaytime( const uint32_t time, Transaction transaction )
try
{
	transaction << "UPDATE records SET total_playtime = ? WHERE record_id = ?" << time << m_id;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::
		error( "{}->RecordData::setTotalPlaytime({}): {} [{},{}]", m_id, time, e.what(), e.get_code(), e.get_sql() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setTotalPlaytime({}): unknown exception", m_id, time );
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
	spdlog::debug(
		"RecordData::addVersion() - Adding version {} to record {}:{}",
		version.toStdString(),
		m_id,
		getTitle( transaction ).toStdString() );
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
	const auto active_versions { getVersions( transaction ) };

	auto itter { std::find( active_versions.begin(), active_versions.end(), version ) };
	if ( itter == active_versions.end() ) return;

	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND version = ?" << m_id
				<< version.getVersionName().toStdString();
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

void RecordData::setBanner( const std::filesystem::path& path, const BannerType type, Transaction transaction )
try
{
	spdlog::debug( "Setting banner to {} for record_id {}", path, m_id );

	//Move banner to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	//Check if it exists
	if ( new_path == getBannerPath( type, transaction ) )
	{
		transaction << "UPDATE banners SET record_id = ? AND path = ? AND type = ?" << m_id << new_path.string()
					<< static_cast< int >( type );
	}
	else
	{
		transaction << "INSERT INTO banners (record_id, path, type) VALUES (?, ?, ?)" << m_id << new_path.string()
					<< static_cast< int >( type );
	}
}
catch ( sqlite::errors::constraint_unique& e )
{
	//In this case we want to just eat the unique constraint. Since it just means the banner was set to the same thing as before.
	return;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::
		error( "{}->RecordData::setBanner({}): {} [{},{}]", m_id, path.string(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::setBanner({}): {}", m_id, path.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::setBanner({}): unknown exception", m_id, path.string() );
}

void RecordData::addPreview( const std::filesystem::path& path, Transaction transaction )
try
{
	spdlog::debug( "RecordData::addPreview({:ce})", path );
	//Move preview to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	transaction << "INSERT INTO previews (record_id, path, position) VALUES (?, ?, ?)" << m_id << new_path.string()
				<< 256;
}
catch ( sqlite::errors::constraint_unique& e )
{
	//Eat unique constraint since it just means the preview was already added
	return;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::
		error( "{}->RecordData::addPreview({}): {} [{},{}]", m_id, path.string(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::addPreview({}): {}", m_id, path.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::addPreview({}): unknown exception", m_id, path.string() );
}

void RecordData::reorderPreviews( const std::vector< std::filesystem::path >& paths, Transaction transaction )
try
{
	const auto active_previews { getPreviewPaths( transaction ) };

	//Set all previews to max order
	constexpr std::size_t max_order { 256 };
	// max_order is just used to prevent strange previews that somehow were not in this list from being set to 0
	transaction << "UPDATE previews SET position = ? WHERE record_id = ?" << max_order << m_id;

	//Update all previews
	for ( std::size_t i = 0; i < paths.size(); ++i )
	{
		const auto& path { paths[ i ] };
		if ( std::find( active_previews.begin(), active_previews.end(), path ) != active_previews.end() )
		{
			//Update it
			transaction << "UPDATE previews SET position = ? WHERE record_id = ? AND path = ?" << i << m_id
						<< path.string();
		}
		else
		{
			//Path is not something that has been added?
			spdlog::warn( "Invalid path found in preview reorder list. Ignoring. Path was {:ce}", path );
			continue;
		}
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::reorderPreviews(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::reorderPreviews(): {}", m_id, e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::reorderPreviews(): unknown exception", m_id );
}

void RecordData::sync( Transaction transaction )
try
{
	new ( this ) RecordData( m_id, transaction );
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error( "{}->RecordData::sync(): {} [{},{}]", m_id, e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::sync(): {}", m_id, e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::sync(): unknown exception", m_id );
}

RecordData::RecordData( QString title, QString creator, QString engine, Transaction transaction )
try
{
	RecordID record_id { 0 };
	transaction << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?" << title.toStdString()
				<< creator.toStdString() << engine.toStdString()
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
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): {} [{},{}]",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): {}",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		e.what() );
}
catch ( ... )
{
	spdlog::error(
		"RecordData::RecordData({}, {}, {}): unknown exception",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString() );
}

QPixmap RecordData::
	getBanner( const QSize size, const SCALE_TYPE aspect_ratio_mode, const BannerType type, Transaction transaction )
		const
{
	return getBanner( size.width(), size.height(), aspect_ratio_mode, type, transaction );
}

QString RecordData::getDesc( Transaction transaction ) const
try
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

void RecordData::removePreview( const std::filesystem::path& preview, Transaction trans )
try
{
	trans << "DELETE FROM previews WHERE record_id = ? AND path = ?" << m_id << preview.string();
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::removePreview({}): {} [{},{}]", m_id, preview.string(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::removePreview({}): {}", m_id, preview.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::removePreview({}): unknown exception", m_id, preview.string() );
}

RecordID recordID( const QString& title, const QString& creator, const QString& engine, Transaction transaction )
try
{
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