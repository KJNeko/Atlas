//
// Created by kj16609 on 1/15/23.
//

#include <QPixmapCache>

#include <tracy/Tracy.hpp>

#include <h95/database/Database.hpp>
#include <h95/config.hpp>

#include <h95/database/Record.hpp>
#include "h95/imageManager.hpp"

enum PreviewType
{
	PREVIEW_UNKNOWN = 0,
	PREVIEW_BANNER,
	PREVIEW_PREVIEW
};

Record Record::create(
	const QString& title,
	const QString& creator,
	const QString& engine,
	const std::vector< GameMetadata >& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews,
	Transaction& transaction )
{
	ZoneScoped;
	RecordID id { 0 };

	spdlog::debug(
		"Adding record with following values: title={}, creator={}, engine={}, banner={}, {} previews",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		banner.string(),
		previews.size() );

	{
		ZoneScopedN( "Query Check" );
		spdlog::debug( "Checking if game already exists in record." );

		transaction.ref() << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
						  << title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [&]( const RecordID record_id )
		{
			id = record_id;
		};

		if ( id != 0 )
		{
			auto record { Record::select( id, transaction ) };
			transaction.abort();
			throw RecordAlreadyExists( std::move( record ) );
		}
	}

	{
		ZoneScopedN( "Query" );

		spdlog::debug( "Inserting new record" );

		transaction.ref() << "INSERT INTO records (title, creator, engine) VALUES (?, ?, ?) RETURNING record_id"
						  << title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [&]( const RecordID record_id )
		{
			id = record_id;
		};

		spdlog::debug( "Record successfully imported with id = {}", id );

		std::vector< std::pair< std::string, PreviewType > > image_paths;
		if ( !banner.empty() && ( std::filesystem::exists( banner ) || banner.string().starts_with( ':' ) ) )
			image_paths.emplace_back( banner.string(), PREVIEW_BANNER );
		for ( const auto& preview : previews )
			if ( std::filesystem::exists( banner ) ) image_paths.emplace_back( preview.string(), PREVIEW_PREVIEW );

		spdlog::debug( "Inserting {} images for record id {}", previews.size(), id );

		for ( const auto& [path, type] : image_paths )
			transaction.ref() << "INSERT INTO images (record_id, type, path) VALUES (?, ?, ?)" << id
							  << static_cast< uint8_t >( type ) << path;
	}

	std::vector< GameMetadata > metadata_inserted;
	for ( const auto& m_data : metadata )
	{
		metadata_inserted.emplace_back( GameMetadata::insert( id, m_data, transaction ) );
	}

	Record record { id, title, creator, engine, std::move( metadata_inserted ), banner, std::move( previews ) };

	return record;
}

Record Record::select( const RecordID id, Transaction& transaction )
{
	ZoneScoped;
	QString title;
	QString creator;
	QString engine;
	bool record_found { false };

	if ( id == 0 )
	{
		transaction.abort();
		throw InvalidRecordID( id );
	}

	spdlog::debug( "Selecting record {} from database", id );

	{
		ZoneScopedN( "Query inital" );
		transaction.ref() << "SELECT title, creator, engine FROM records WHERE record_id = ?" << id >>
			[&]( const std::string& title_in, const std::string& creator_in, const std::string& engine_in )
		{
			title = QString::fromStdString( title_in );
			creator = QString::fromStdString( creator_in );
			engine = QString::fromStdString( engine_in );
			record_found = true;
		};
	}

	if ( !record_found )
	{
		transaction.abort();
		throw InvalidRecordID( id );
	}

	std::filesystem::path banner_path { ":/banner/placeholder.jpg" };
	std::vector< std::filesystem::path > preview_paths;

	{
		ZoneScopedN( "Query previews" );

		spdlog::debug( "Selecting previews and banner from DB for record {}", id );

		transaction.ref() << "SELECT path, type FROM images WHERE record_id = ?" << id >>
			[&banner_path, &preview_paths]( const std::string& path, const uint8_t type )
		{
			ZoneScopedN( "Process row" );

			if ( !std::filesystem::exists( path ) && !path.starts_with( ':' ) )
			{
				spdlog::warn( "Path {} was expected to exist but does not", path );
				return;
			}

			switch ( static_cast< PreviewType >( type ) )
			{
				default:
					[[fallthrough]];
				case PREVIEW_UNKNOWN:
					break;
				case PREVIEW_BANNER:
					banner_path = std::move( path );
					break;
				case PREVIEW_PREVIEW:
					preview_paths.emplace_back( std::move( path ) );
					break;
			}
		};
	}

	return { id, title, creator, engine, GameMetadata::select( id, transaction ), banner_path, preview_paths };
}

RecordID Record::search(
	const QString& title,
	const QString& creator,
	const QString& engine,
	const RecordID anti_id,
	Transaction& transaction )
{
	RecordID record_id { 0 };

	{
		ZoneScopedN( "Query" );

		spdlog::debug(
			"Searching for {} {} {} ! {}",
			title.toStdString(),
			creator.toStdString(),
			engine.toStdString(),
			anti_id );

		transaction.ref()
				<< "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ? AND NOT record_id = ?"
				<< title.toStdString() << creator.toStdString() << engine.toStdString() << anti_id
			>> [&]( const RecordID id )
		{
			record_id = id;
		};
	}

	return record_id;
}


QPixmap Record::getBanner() const
{
	ZoneScoped;

	spdlog::debug( "Getting banner for {}", m_id );

	const auto banner_path_str { QString::fromStdString( m_banner.string() ) };
	const std::filesystem::path banner_path { banner_path_str.toStdString() };

	QPixmap banner { ":/banner/placeholder.jpg" };
	if ( !QPixmapCache::find( banner_path_str, &banner ) && std::filesystem::exists( banner_path ) )
	{
		banner = QPixmap( banner_path_str );
		QPixmapCache::insert( banner_path_str, banner );
		return banner;
	}

	return banner;
}

QPixmap Record::getBanner( const int banner_width, const int banner_height ) const
{
	ZoneScopedN( "getBannerResized" );

	spdlog::debug( "Getting banner for id {} resized to {}x{}", m_id, banner_width, banner_height );

	const auto banner_path_str { QString::fromStdString( m_banner.string() ) };
	const std::filesystem::path banner_path { banner_path_str.toStdString() };

	const auto key { banner_path_str + QString::number( banner_width ) + QString::number( banner_width ) };

	QPixmap resized_banner { ":/invalid_banner.jpg" };
	if ( !QPixmapCache::find( key, &resized_banner ) && (std::filesystem::exists( banner_path ) || banner_path.string().starts_with(':')) )
	{
		QPixmap banner { getBanner() };

		banner = banner.scaledToHeight( banner_height, Qt::SmoothTransformation );
		if ( banner.width() > banner_width ) banner = banner.scaledToWidth( banner_width, Qt::SmoothTransformation );

		QPixmapCache::insert( key, banner );
		return banner;
	}

	return resized_banner;
}

void Record::update( const RecordID id, Record& record, Transaction& transaction )
try
{
	ZoneScoped;

	spdlog::debug( "Updating record {}", id );

	const auto original { Record::select( id, transaction ) };

	{
		ZoneScopedN( "Update basic info" );
		transaction.ref() << "UPDATE records SET title = ?, creator = ?, engine = ? WHERE record_id = ?"
						  << record.m_title.toStdString() << record.m_creator.toStdString()
						  << record.m_engine.toStdString() << record.m_id;
	}

	{
		ZoneScopedN( "Update versions" );

		std::vector< GameMetadata > to_remove;

		for ( const auto& version : record.m_versions )
		{
			//Check if the version is already in the list
			if ( std::find( original.m_versions.begin(), original.m_versions.end(), version )
				 != original.m_versions.end() )
				continue;
			else
				//Not in list. Need to add.
				(void) GameMetadata::insert( id, version, transaction );
		}

		//Check if we need to remove any.
		for ( const auto& version : original.m_versions )
		{
			if ( std::find( record.m_versions.begin(), record.m_versions.end(), version ) == record.m_versions.end() )
				transaction.ref() << "DELETE FROM game_metadata WHERE record_id = ? AND version = ?" << id
								  << version.m_version.toStdString();
			else
				continue;
		}
	}

	{
		ZoneScopedN( "Update images" );

		const auto preview_path { imageManager::getPreviewPath() };
		const auto banner_path { imageManager::getBannerPath() };

		if ( !record.m_banner.string().starts_with( banner_path.string() ) )
			record.m_banner = imageManager::importBanner( record.m_banner );

		if ( original.m_banner != record.m_banner )
			transaction.ref() << "UPDATE images SET path = ? WHERE type = ? AND record_id = ?"
							  << record.m_banner.string() << PREVIEW_BANNER << record.m_id;

		for ( auto& preview : record.m_previews )
		{
			if ( !preview.string().starts_with( preview_path.string() ) )
				preview = imageManager::importPreview( preview );
		}

		//Remove all previews that are not in the list
		transaction.ref() << "SELECT path FROM images WHERE type = ? AND record_id = ?" << PREVIEW_PREVIEW
						  << record.m_id
			>> [&]( const std::string& path )
		{
			//Try to find the path inside of the list in memory.
			if ( std::find_if(
					 record.m_previews.begin(),
					 record.m_previews.end(),
					 [&]( const std::filesystem::path& path_fs ) -> bool { return path_fs.string() == path; } )
				 == record.m_previews.end() )
			{
				spdlog::debug( "Deleting image {} from record {}", path, record.m_id );

				//Delete the item from the database if it's not found in the memory list.
				transaction.ref() << "DELETE FROM images WHERE type = ? AND record_id = ? AND path = ?"
								  << PREVIEW_PREVIEW << record.m_id << path;
			}
		};

		//Add previews that are not in the db list
		for ( const auto& path : record.m_previews )
		{
			bool found { false };
			//Search for if it's in the database
			transaction.ref() << "SELECT path FROM images WHERE type = ? AND record_id = ? AND path = ?"
							  << PREVIEW_PREVIEW << record.m_id << path.string()
				>> [&]( [[maybe_unused]] const std::string& str )
			{
				found = true;
			};

			//If it's not in the database then add it.
			if ( !found )
			{
				spdlog::debug( "Adding image {} to record {}", path.string(), record.m_id );

				transaction.ref() << "INSERT INTO images (record_id, type, path) VALUES (?, ?, ?)" << record.m_id
								  << PREVIEW_PREVIEW << path.string();
			}
		}
	}
}
catch ( std::exception& e )
{
	spdlog::critical( "Failed to update record properly: {}", e.what() );
}

void Record::erase( const RecordID id, Transaction& transaction )
try
{
	ZoneScoped;

	transaction.ref() << "DELETE FROM images WHERE record_id = ?" << id;
	transaction.ref() << "DELETE FROM game_metadata WHERE record_id = ?" << id;
	transaction.ref() << "DELETE FROM records WHERE record_id = ?" << id;

	//TODO: Clean orphans/empty folders
}
catch ( std::exception& e )
{
	spdlog::error( "Failed to delete record due to {}", e.what() );
}
