//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"
#include <QSqlQuery>
#include <QJsonArray>
#include <iostream>
#include <QSqlError>

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
	const GameMetadata& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews )
{
	QSqlQuery query;
	query.prepare(
		"INSERT INTO records (title, creator, version) VALUES (:title, :creator, :version) RETURNING record_id" );

	query.bindValue( ":title", title );
	query.bindValue( ":creator", creator );
	query.bindValue( ":version", version );
	query.exec();
	query.first();
	RecordID record_id { query.value(0).value<RecordID>() };

	std::vector< std::pair< QString, QString > > image_paths;
	if ( !banner.empty() ) image_paths.emplace_back( QString::fromStdString( banner.string() ), "banner" );
	for ( const auto& preview : previews )
		image_paths.emplace_back( QString::fromStdString( preview.string() ), "preview" );

	QSqlQuery banner_query;
	banner_query.prepare( "INSERT INTO previews (record_id, type, path) VALUES (:id, :type, :path)" );
	for ( const auto& [path, type] : image_paths )
	{
		banner_query.bindValue(":id", record_id);
		banner_query.bindValue( ":type", type );
		banner_query.bindValue( ":path", path );
		banner_query.exec();
	}

	return { record_id, title, creator, version, GameMetadata::insert( record_id, metadata ), banner, previews };
}

Record Record::select( const RecordID id )
{
	QSqlQuery query;
	query.prepare( "SELECT title, creator, version FROM records WHERE record_id = :id" );
	query.bindValue( ":id", id );
	query.exec();
	query.first();

	QSqlQuery banner_query;
	banner_query.prepare( "SELECT path, type FROM previews WHERE record_id = :id" );

	banner_query.bindValue( ":id", id );
	banner_query.exec();

	std::filesystem::path banner_path { ":invalid_banner.jpg" };
	std::vector< std::filesystem::path > preview_paths;

	while ( banner_query.next() )
	{
		const auto path {banner_query.value(0).toString()};
		const auto type {banner_query.value(1).toString()};

		if ( type == "banner" )
			banner_path = path.toStdString();
		else
			preview_paths.emplace_back( path.toStdString() );
	}

	return { id, query.value("title").toString(), query.value("creator").toString(), query.value("version").toString(), GameMetadata::select( id ), banner_path, preview_paths };
}
