//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"
#include <QJsonArray>
#include <iostream>
#include "h95/database/database.hpp"

#ifdef NDEBUG
bool Record::operator==( const Record& other ) const
{
	return other.m_id == this->m_id;
}
#endif

Record Record::create(
	const QString& title,
	const QString& creator,
	const QString& version,
	const QString& engine,
	const GameMetadata& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews )
{

	RecordID id { 0 };
	database::db_ref() << "INSERT INTO records (title, creator, version, engine) VALUES (?, ?, ?, ?) RETURNING record_id"
					   << title.toStdString() << creator.toStdString() << version.toStdString() << engine.toStdString()
		>> [&]( const RecordID record_id )
	{
		id = record_id;
	};

	std::vector< std::pair< std::string, std::string > > image_paths;
	if ( !banner.empty() ) image_paths.emplace_back( banner.string(), "banner" );
	for ( const auto& preview : previews ) image_paths.emplace_back( preview.string(), "preview" );

	for ( const auto& [path, type] : image_paths )
		database::db_ref() << "INSERT INTO previews (record_id, type, path) VALUES (?, ?, ?)" << id << type << path;

	return { id, title, creator, version, engine, GameMetadata::insert( id, metadata ), banner, previews };
}

Record Record::select( const RecordID id )
{
	QString title;
	QString creator;
	QString version;
	QString engine;

	database::db_ref() << "SELECT title, creator, version, engine FROM records WHERE record_id = ?" << id >>
		[&]( const std::string title_in,
			 const std::string creator_in,
			 const std::string version_in,
			 const std::string engine_in )
	{
		title = QString::fromStdString( title_in );
		creator = QString::fromStdString( creator_in );
		version = QString::fromStdString( version_in );
		engine = QString::fromStdString( engine_in );
	};

	std::filesystem::path banner_path { ":invalid_banner.jpg" };
	std::vector< std::filesystem::path > preview_paths;

	database::db_ref() << "SELECT path, type FROM previews WHERE record_id = ?" << id >>
		[&]( const std::string path, const std::string type )
	{
		if ( type == "banner" )
			banner_path = path;
		else
			preview_paths.emplace_back( path );
	};

	return { id, title, creator, version, engine, GameMetadata::select( id ), banner_path, preview_paths };
}
