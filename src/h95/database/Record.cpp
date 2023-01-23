//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"
#include <QJsonArray>
#include <iostream>
#include <tracy/Tracy.hpp>
#include "h95/database/database.hpp"

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
	const QString& version,
	const QString& engine,
	const GameMetadata& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews )
{
	ZoneScoped;
	RecordID id { 0 };

	{
		ZoneScopedN( "Query" );
		database::db_ref()
				<< "INSERT INTO records (title, creator, version, engine) VALUES (?, ?, ?, ?) RETURNING record_id"
				<< title.toStdString() << creator.toStdString() << version.toStdString() << engine.toStdString()
			>> [&]( const RecordID record_id )
		{
			id = record_id;
		};
	}

	std::vector< std::pair< std::string, PreviewType > > image_paths;
	if ( !banner.empty() ) image_paths.emplace_back( banner.string(), PREVIEW_BANNER );
	for ( const auto& preview : previews ) image_paths.emplace_back( preview.string(), PREVIEW_PREVIEW);

	for ( const auto& [path, type] : image_paths )
		database::db_ref() << "INSERT INTO previews (record_id, type, path) VALUES (?, ?, ?)" << id << static_cast<uint8_t>(type) << path;

	return { id, title, creator, version, engine, GameMetadata::insert( id, metadata ), banner, previews };
}

Record Record::select( const RecordID id )
{
	ZoneScoped;
	QString title;
	QString creator;
	QString version;
	QString engine;

	{
		ZoneScopedN( "Query Inital" );
		database::db_ref() << "SELECT title, creator, version, engine FROM records WHERE record_id = ?" << id >>
			[&]( const std::string& title_in,
				 const std::string& creator_in,
				 const std::string& version_in,
				 const std::string& engine_in )
		{
			title = QString::fromStdString( title_in );
			creator = QString::fromStdString( creator_in );
			version = QString::fromStdString( version_in );
			engine = QString::fromStdString( engine_in );
		};
	}


	std::filesystem::path banner_path { ":invalid_banner.jpg" };
	std::vector< std::filesystem::path > preview_paths;
	preview_paths.reserve(32);

	{
		ZoneScopedN( "Query previews" );
		database::db_ref() << "SELECT path, type FROM previews WHERE record_id = ?" << id >>
			[&banner_path, &preview_paths]( std::string&& path, const uint8_t type )
		{
			ZoneScopedN("Process row");
			switch(static_cast<PreviewType>(type))
			{
				default: [[fallthrough]];
				case PREVIEW_UNKNOWN:
					break;
				case PREVIEW_BANNER:
					banner_path = std::move(path);
					break;
				case PREVIEW_PREVIEW:
					preview_paths.emplace_back(std::move(path));
					break;
			}
		};
	}

	return { id, title, creator, version, engine, GameMetadata::select( id ), banner_path, preview_paths };
}
