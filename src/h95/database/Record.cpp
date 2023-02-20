//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"
#include <QJsonArray>
#include <iostream>
#include <tracy/Tracy.hpp>
#include <QPixmapCache>
#include "h95/database/database.hpp"
#include "h95/config.hpp"

#ifdef NDEBUG
bool Record::operator==( const Record& other ) const
{
	return other.m_id == this->m_id;
}
#endif

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
		metadata.version.toStdString(),
		metadata.game_path.string(),
		metadata.exec_path.string(),
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
		ZoneScopedN( "Query Inital" );
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
					banner_path = std::move( path );
					break;
				case PREVIEW_PREVIEW:
					preview_paths.emplace_back( std::move( path ) );
					break;
			}
		};
	}

	return { id, title, creator, engine, GameMetadata::select( id ), banner_path, preview_paths };
}


QPixmap Record::getBanner() const
{
	ZoneScoped;
	const auto banner_path_str { QString::fromStdString( m_banner ) };
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
	const auto banner_path_str { QString::fromStdString( m_banner ) };
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
