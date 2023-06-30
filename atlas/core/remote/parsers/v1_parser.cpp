//
// Created by kj16609 on 6/28/23.
//

#include <QJsonArray>

#include "core/database/Transaction.hpp"
#include "core/remote/parsers/parser.hpp"

namespace remote::parsers::v1
{
	enum DataSet
	{
		SetAtlas,
		SetF95,
		InvalidSet
	};

	DataSet nameToSet( const QString str )
	{
		if ( str == "atlas" ) return SetAtlas;
		if ( str == "f95_zone" ) return SetF95;

		return InvalidSet;
	}

#define KEY_CHECK( key )                                                                                               \
	if ( !obj.contains( key ) ) return false;

	bool validateAtlasKeys( const QJsonObject& obj )
	{
		KEY_CHECK( "atlas_id" )
		KEY_CHECK( "id_name" )
		KEY_CHECK( "short_name" )
		KEY_CHECK( "title" )
		KEY_CHECK( "original_name" )
		KEY_CHECK( "category" )
		KEY_CHECK( "engine" )
		KEY_CHECK( "status" )
		KEY_CHECK( "version" )
		KEY_CHECK( "developer" )
		KEY_CHECK( "creator" )
		KEY_CHECK( "overview" )
		KEY_CHECK( "censored" )
		KEY_CHECK( "language" )
		KEY_CHECK( "translations" )
		KEY_CHECK( "genre" )
		KEY_CHECK( "tags" )
		KEY_CHECK( "voice" )
		KEY_CHECK( "os" )
		KEY_CHECK( "release_date" )
		KEY_CHECK( "length" )
		KEY_CHECK( "banner" )
		KEY_CHECK( "banner_wide" )
		KEY_CHECK( "cover" )
		KEY_CHECK( "logo" )
		KEY_CHECK( "wallpaper" )
		KEY_CHECK( "previews" )
		KEY_CHECK( "last_db_update" )

		return true;
	}

	void updateAtlasData( const QJsonObject& obj, Transaction& trans )
	{
		const auto keys { obj.keys() };

		if ( !obj.contains( "atlas_id" ) ) throw std::runtime_error( "Atlas data did not contain it's pkey!" );

		const auto pkey { obj[ "atlas_id" ].toInteger() };

		for ( const auto& key : keys )
		{
			const auto& data { obj[ key ] };
			if ( data.isString() )
				trans << fmt::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key ) << data.toString()
					  << pkey;
			else if ( data.isDouble() )
				trans << fmt::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key ) << data.toInteger()
					  << pkey;
			else
				throw std::runtime_error( "Unexpected type when parsing atlas data!" );
		}
	}

	void insertAtlasData( const QJsonObject& obj, Transaction& trans )
	{
		constexpr std::string_view query { "INSERT INTO atlas_data ("
			                               "atlas_id, id_name, short_name, title, original_name, category, "
			                               "engine, status, version, developer, creator, overview, censored, "
			                               "language, translations, genre, tags, voice, os, release_date, length, "
			                               "banner, banner_wide, cover, logo, wallpaper, previews, last_db_update) "
			                               "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)" };

		trans << query << obj[ "atlas_id" ].toInteger() << obj[ "id_name" ].toString() << obj[ "short_name" ].toString()
			  << obj[ "title" ].toString() << obj[ "original_name" ].toString() << obj[ "category" ].toString()
			  << obj[ "engine" ].toString() << obj[ "status" ].toString() << obj[ "version" ].toString()
			  << obj[ "developer" ].toString() << obj[ "creator" ].toString() << obj[ "overview" ].toString()
			  << obj[ "censored" ].toString() << obj[ "language" ].toString() << obj[ "translations" ].toString()
			  << obj[ "genre" ].toString() << obj[ "tags" ].toString() << obj[ "voice" ].toString()
			  << obj[ "os" ].toString() << obj[ "release_date" ].toInteger() << obj[ "length" ].toString()
			  << obj[ "banner" ].toString() << obj[ "banner_wide" ].toString() << obj[ "cover" ].toString()
			  << obj[ "logo" ].toString() << obj[ "wallpaper" ].toString() << obj[ "previews" ].toString()
			  << obj[ "last_db_update" ].toInteger();
	}

	void parseAtlasArray( const QJsonArray& data, Transaction& trans )
	{
		for ( const auto& obj_data : data )
		{
			const auto& obj { obj_data.toObject() };

			if ( !validateAtlasKeys( obj ) )
				updateAtlasData( obj, trans ); //This is probably an update
			else
				insertAtlasData( obj, trans );
		}
	}

	bool validateF95Keys( const QJsonObject& obj )
	{
		KEY_CHECK( "f95_id" )
		KEY_CHECK( "atlas_id" )
		KEY_CHECK( "banner_url" )
		KEY_CHECK( "site_url" )
		KEY_CHECK( "last_thread_comment" )
		KEY_CHECK( "thread_publish_date" )
		KEY_CHECK( "views" )
		KEY_CHECK( "likes" )
		KEY_CHECK( "tags" )
		KEY_CHECK( "rating" )
		KEY_CHECK( "rating" )
		KEY_CHECK( "screens" )
		KEY_CHECK( "replies" )

		return true;
	}

	void updateF95Data( const QJsonObject& obj, Transaction& trans )
	{
		const auto keys { obj.keys() };

		if ( !obj.contains( "f95_id" ) ) throw std::runtime_error( "F95 data did not contain it's pkey!" );

		const auto pkey { obj[ "f95_id" ].toInteger() };

		for ( const auto& key : keys )
		{
			const auto& data { obj[ key ] };
			if ( data.isString() )
				trans << fmt::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key ) << data.toString()
					  << pkey;
			else if ( data.isDouble() )
				trans << fmt::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key ) << data.toInteger()
					  << pkey;
			else
				throw std::runtime_error( "Unexpected type when parsing f95zone data!" );
		}
	}

	void insertF95Data( const QJsonObject& obj, Transaction& trans )
	{
		constexpr std::string_view query {
			"INSERT INTO f95_zone_data (f95_id, atlas_id, banner_url, site_url, last_thread_comment, thread_publish_date, last_record_update, views, likes, tags, rating, screens, replies) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)"
		};

		trans << query << obj[ "f95_id" ].toInteger() << obj[ "atlas_id" ].toInteger() << obj[ "banner_url" ].toString()
			  << obj[ "site_url" ].toString() << obj[ "last_thread_comment" ].toInteger()
			  << obj[ "thread_publish_date" ].toInteger() << obj[ "last_record_update" ].toInteger()
			  << obj[ "views" ].toInteger() << obj[ "likes" ].toInteger() << obj[ "tags" ].toString()
			  << obj[ "rating" ].toDouble() << obj[ "screens" ].toString() << obj[ "replies" ].toInteger();
	}

	void parseF95Array( const QJsonArray& data, Transaction& trans )
	{
		for ( const auto& obj_data : data )
		{
			const auto& obj { obj_data.toObject() };

			if ( !validateF95Keys( obj ) )
				updateF95Data( obj, trans );
			else
				insertF95Data( obj, trans );
		}
	}

	void processJson( const QJsonObject& json )
	{
		Transaction transaction {};

		for ( const auto& table_key : json.keys() )
		{
			const auto set { nameToSet( table_key ) };
			if ( set == InvalidSet )
			{
				throw std::runtime_error( "Unexpected data in set!" );
			}

			ZoneScopedN( "Process set" );
			const auto& key_str { table_key.toStdString() };
			TracyMessage( key_str.c_str(), key_str.size() );
			const QJsonArray& data = json[ table_key ].toArray();

			switch ( set )
			{
				case SetAtlas:
					parseAtlasArray( data, transaction );
					break;
				case SetF95:
					parseF95Array( data, transaction );
					break;
				default:
					throw std::runtime_error( "Unexpected set!" );
			}
		}
	}

} // namespace remote::parsers::v1