//
// Created by kj16609 on 6/6/23.
//

#include "RemoteDownloader.hpp"

#include <filesystem>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

#include <tracy/Tracy.hpp>

#include "core/database/Database.hpp"
#include "core/logging.hpp"

RemoteDownloader::RemoteDownloader()
{}

void RemoteDownloader::checkRemote()
{
	QNetworkAccessManager manager;

	if ( m_last_check == 0 )
	{
		//Never checked before! Gotta process base.update

		//Check if it exists.
		//If it does, download it.

		if ( !std::filesystem::exists( "./data/updates/base.update" ) )
		{
			//Download it. (REMOTE + "packages/base.update")

			QUrl url { REMOTE "packages/base.update" };
			QNetworkRequest request { url };
			manager.get( request );
		}

		//TODO: Extract the file to working location

		parse( "./data/updates/base" );
	}
}

void RemoteDownloader::parse( const std::filesystem::path path )
try
{
	spdlog::info( "Parsing {:ce}", path );
	ZoneScoped;
	QFile file { QString::fromStdString( path.string() ) };
	file.open( QIODevice::ReadOnly );
	const auto file_data { file.readAll() };

	const QJsonObject json { QJsonDocument::fromJson( file_data ).object() };

	Transaction transaction { NoAutocommit };

	for ( const auto& table_key : json.keys() )
	{
		ZoneScopedN( "Process set" );
		const auto key_str { table_key.toStdString() };
		TracyMessage( key_str.c_str(), key_str.size() );
		const QJsonArray data = json[ table_key ].toArray();
		for ( const auto& obj_data : data )
		{
			ZoneScopedN( "Process object" );
			const auto obj { obj_data.toObject() };

			const auto keys { obj.keys() };

			std::string query { fmt::format( "INSERT INTO {}_data (", table_key.toStdString() ) };
			for ( int i = 0; i < keys.size(); ++i )
			{
				const auto key { keys[ i ] };
				query += fmt::format( "{}", key.toStdString() );
				if ( i != keys.size() - 1 ) query += fmt::format( "," );
			}
			query += ") VALUES (";
			for ( int i = 0; i < keys.size(); ++i )
			{
				const auto key { keys[ i ] };
				const auto value { obj[ key ] };
				if ( value.isDouble() )
					query += fmt::format( "{}", value.toInteger() );
				else if ( value.isNull() )
					query += fmt::format( "NULL" );
				else
					query += fmt::format( "\'{}\'", value.toString().replace( '\'', "\'\'" ).toStdString() );
				if ( i != keys.size() - 1 ) query += fmt::format( "," );
			}

			query += ");";

			transaction << query;
		}
	}

	transaction.commit();
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "Error parsing {:ce}: {}: query: {}", path, e.what(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "Error parsing {:ce}: {}", path, e.what() );
}
catch ( ... )
{
	spdlog::error( "Error parsing {:ce}: Unknown error", path );
}
