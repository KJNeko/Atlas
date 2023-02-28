//
// Created by kj16609 on 1/15/23.
//

#include <QPixmapCache>

#include <tracy/Tracy.hpp>

#include <h95/database/database.hpp>
#include <h95/config.hpp>
#include <h95/Importer.hpp>

#include <h95/database/Record.hpp>

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
	const GameMetadata& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews )
{
	ZoneScoped;
	RecordID id { 0 };

	spdlog::debug(
		"Adding record with following values: title={}, creator={}, engine={}, version={}, game_path={}, exec_path={}, banner={}, {} previews",
		title.toStdString(),
		creator.toStdString(),
		engine.toStdString(),
		metadata.m_version.toStdString(),
		metadata.m_game_path.string(),
		metadata.m_exec_path.string(),
		banner.string(),
		previews.size() );

	{
		ZoneScopedN( "Query Check" );
		spdlog::debug( "Checking if game already exists in record." );

		database::db_ref() << "SELECT record_id FROM records WHERE title = ? AND creator = ? AND engine = ?"
						   << title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [&]( const RecordID record_id )
		{
			id = record_id;
		};

		if ( id != 0 ) { spdlog::debug( "Game was found with id {}! Adding version instead", id ); }
	}

	if ( id == 0 )
	{
		ZoneScopedN( "Query" );
		database::db_ref() << "INSERT INTO records (title, creator, engine) VALUES (?, ?, ?) RETURNING record_id"
						   << title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [&]( const RecordID record_id )
		{
			id = record_id;
		};
		spdlog::info( "Record successfully imported with id = {}", id );

		std::vector< std::pair< std::string, PreviewType > > image_paths;
		if ( !banner.empty() ) image_paths.emplace_back( banner.string(), PREVIEW_BANNER );
		for ( const auto& preview : previews ) image_paths.emplace_back( preview.string(), PREVIEW_PREVIEW );

		for ( const auto& [path, type] : image_paths )
			database::db_ref() << "INSERT INTO previews (record_id, type, path) VALUES (?, ?, ?)" << id
							   << static_cast< uint8_t >( type ) << path;
	}

	return { id, title, creator, engine, { GameMetadata::insert( id, metadata ) }, banner, previews };
}

Record Record::select( const RecordID id )
{
	ZoneScoped;
	QString title;
	QString creator;
	QString engine;

	{
		ZoneScopedN( "Query inital" );
		database::db_ref() << "SELECT title, creator, engine FROM records WHERE record_id = ?" << id >>
			[&]( const std::string& title_in, const std::string& creator_in, const std::string& engine_in )
		{
			title = QString::fromStdString( title_in );
			creator = QString::fromStdString( creator_in );
			engine = QString::fromStdString( engine_in );
		};
	}

	std::filesystem::path banner_path { ":invalid_banner.jpg" };
	std::vector< std::filesystem::path > preview_paths;
	preview_paths.reserve( 32 );

	{
		ZoneScopedN( "Query previews" );
		database::db_ref() << "SELECT path, type FROM previews WHERE record_id = ?" << id >>
			[&banner_path, &preview_paths]( const std::string& path, const uint8_t type )
		{
			ZoneScopedN( "Process row" );
			switch ( static_cast< PreviewType >( type ) )
			{
				default:
					[[fallthrough]];
				case PREVIEW_UNKNOWN:
					break;
				case PREVIEW_BANNER:
					banner_path = std::filesystem::canonical( std::move( path ) );
					break;
				case PREVIEW_PREVIEW:
					preview_paths.emplace_back( std::filesystem::canonical( std::move( path ) ) );
					break;
			}
		};
	}

	return { id, title, creator, engine, GameMetadata::select( id ), banner_path, preview_paths };
}

RecordID Record::search( const QString& title, const QString& creator, const QString& engine, const RecordID anti_id )
{
	RecordID record_id { 0 };

	{
		ZoneScopedN( "Query" );
		database::db_ref()
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
	const auto banner_path_str { QString::fromStdString( m_banner.string() ) };
	const std::filesystem::path banner_path { banner_path_str.toStdString() };

	QPixmap banner { ":/invalid_banner.jpg" };
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
	const auto banner_path_str { QString::fromStdString( m_banner.string() ) };
	const std::filesystem::path banner_path { banner_path_str.toStdString() };

	const auto key { banner_path_str + QString::number( banner_width ) + QString::number( banner_width ) };

	QPixmap resized_banner { ":/invalid_banner.jpg" };
	if ( !QPixmapCache::find( key, &resized_banner ) && std::filesystem::exists( banner_path ) )
	{
		QPixmap banner { getBanner() };

		banner = banner.scaledToHeight( banner_height, Qt::SmoothTransformation );
		if ( banner.width() > banner_width ) banner = banner.scaledToWidth( banner_width, Qt::SmoothTransformation );

		QPixmapCache::insert( key, banner );
		return banner;
	}

	return resized_banner;
}

void Record::update( const RecordID id, Record& record )
try
{
	ZoneScoped;

	const auto original { Record::select( id ) };

	{
		ZoneScopedN( "Update basic info" );
		database::db_ref() << "UPDATE records SET title = ?, creator = ?, engine = ? WHERE record_id = ?"
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
				(void) GameMetadata::insert( id, version );
		}

		//Check if we need to remove any.
		for ( const auto& version : original.m_versions )
		{
			if ( std::find( record.m_versions.begin(), record.m_versions.end(), version ) == record.m_versions.end() )
				database::db_ref() << "DELETE FROM game_metadata WHERE record_id = ? AND version = ?" << id
								   << version.m_version.toStdString();
			else
				continue;
		}
	}

	{
		ZoneScopedN( "Update images" );

		const auto preview_path { std::filesystem::canonical(
			getSettings< QString >( "paths/h95_banners", "./data/previews" ).toStdString() ) };
		const auto banner_path { std::filesystem::canonical(
			getSettings< QString >( "paths/h95_previews", "./data/banners" ).toStdString() ) };

		if ( !record.m_banner.string().starts_with( banner_path.string() ) )
			record.m_banner = importBanner( record.m_banner );

		if ( original.m_banner != record.m_banner )
			database::db_ref() << "UPDATE previews SET path = ? WHERE type = ? AND record_id = ?"
							   << record.m_banner.string() << PREVIEW_BANNER << record.m_id;

		for ( auto& preview : record.m_previews )
		{
			if ( !preview.string().starts_with( preview_path.string() ) ) preview = importPreview( preview );
		}

		//Remove all previews that are not in the list
		database::db_ref() << "SELECT path FROM previews WHERE type = ? AND record_id = ?" << PREVIEW_PREVIEW
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
				//Delete the item from the database if it's not found in the memory list.
				database::db_ref() << "DELETE FROM previews WHERE type = ? AND record_id = ? AND path = ?"
								   << PREVIEW_PREVIEW << record.m_id << path;
			}
		};

		//Add previews that are not in the db list
		for ( const auto& path : record.m_previews )
		{
			bool found { false };
			//Search for if it's in the database
			database::db_ref() << "SELECT path FROM previews WHERE type = ? AND record_id = ? AND path = ?"
							   << PREVIEW_PREVIEW << record.m_id << path.string()
				>> [&]( [[maybe_unused]] const std::string& str )
			{
				found = true;
			};

			//If it's not in the database then add it.
			if ( !found )
			{
				database::db_ref() << "INSERT INTO previews (record_id, type, path) VALUES (?, ?, ?)" << record.m_id
								   << PREVIEW_PREVIEW << path.string();
			}
		}
	}
}
catch ( std::exception& e )
{
	spdlog::critical( "Failed to update record properly: {}", e.what() );
}

void Record::erase( const RecordID id )
try
{
	ZoneScoped;

	database::db_ref() << "DELETE FROM previews WHERE record_id = ?" << id;
	database::db_ref() << "DELETE FROM game_metadata WHERE record_id = ?" << id;
	database::db_ref() << "DELETE FROM records WHERE record_id = ?" << id;

	//TODO: Clean orphans/empty folders
}
catch ( std::exception& e )
{
	spdlog::error( "Failed to delete record due to {}", e.what() );
}
