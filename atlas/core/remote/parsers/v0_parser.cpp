//
// Created by kj16609 on 6/28/23.
//

#include <QJsonArray>

#include "core/database/Transaction.hpp"
#include "core/database/record/tags.hpp"
#include "core/notifications/notifications.hpp"
#include "core/remote/parsers/parser.hpp"

namespace remote::parsers::v0
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

		if ( !obj.contains( "atlas_id" ) ) throw AtlasException( "Atlas data did not contain it's pkey!" );

		const auto pkey { obj[ "atlas_id" ].toInteger() };

		for ( const auto& key : keys )
		{
			const auto& data { obj[ key ] };

			switch ( data.type() )
			{
				case QJsonValue::Bool:
					trans << format_ns::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key )
						  << data.toBool() << pkey;
					break;
				case QJsonValue::Double:
					trans << format_ns::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key )
						  << data.toInteger() << pkey;
					break;
				case QJsonValue::String:
					trans << format_ns::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key )
						  << data.toString() << pkey;
					break;
				case QJsonValue::Null:
					trans << format_ns::format( "UPDATE atlas_data SET {} = ? WHERE atlas_id = ?", key ) << std::nullopt
						  << pkey;
					continue;
				case QJsonValue::Array:
					throw AtlasException( "Unexpected type when parsing atlas data: Was array" );
				case QJsonValue::Object:
					throw AtlasException( "Unexpected type when parsing atlas data: Was object" );
				case QJsonValue::Undefined:
					throw AtlasException( "Unexpected type when parsing atlas data: Was undefined" );
				default:
					throw AtlasException( "Unexpected type when parsing atlas data: Was default case" );
			}
		}
	}

	void insertAtlasData( const QJsonObject& obj, Transaction& trans )
	{
		constexpr std::string_view query { "INSERT INTO atlas_data ("
			                               "atlas_id, id_name, short_name, title, original_name, category, "
			                               "engine, status, version, developer, creator, overview, censored, "
			                               "language, translations, genre, voice, os, release_date, length, "
			                               "banner, banner_wide, cover, logo, wallpaper, last_db_update) "
			                               "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)" };

		trans << query << obj[ "atlas_id" ].toInteger() << obj[ "id_name" ].toString() << obj[ "short_name" ].toString()
			  << obj[ "title" ].toString() << obj[ "original_name" ].toString() << obj[ "category" ].toString()
			  << obj[ "engine" ].toString() << obj[ "status" ].toString() << obj[ "version" ].toString()
			  << obj[ "developer" ].toString() << obj[ "creator" ].toString() << obj[ "overview" ].toString()
			  << obj[ "censored" ].toString() << obj[ "language" ].toString() << obj[ "translations" ].toString()
			  << obj[ "genre" ].toString() << obj[ "voice" ].toString() << obj[ "os" ].toString()
			  << obj[ "release_date" ].toInteger() << obj[ "length" ].toString() << obj[ "banner" ].toString()
			  << obj[ "banner_wide" ].toString() << obj[ "cover" ].toString() << obj[ "logo" ].toString()
			  << obj[ "wallpaper" ].toString() << obj[ "last_db_update" ].toInteger();

		const AtlasID atlas_id { static_cast< AtlasID >( obj[ "atlas_id" ].toInteger() ) };

		if ( obj[ "tags" ].isString() )
		{
			//CSV mode
			const auto parsed { obj[ "tags" ].toString().split( ',' ) };

			for ( const auto& str : parsed )
			{
				const TagID tag_id { atlas::tags::createTag( str ) };

				trans << "INSERT INTO atlas_tags (tag_id, atlas_id) VALUES (?,?)" << tag_id << atlas_id;
			}
		}
		else if ( obj[ "tags" ].isArray() )
		{
			const QJsonArray tags = obj[ "tags" ].toArray();

			for ( const QJsonValue& tag : tags )
			{
				const TagID tag_id { atlas::tags::createTag( tag.toString() ) };

				trans << "INSERT INTO atlas_tags (tag_id, atlas_id) VALUES (?,?)" << tag_id << atlas_id;
			}
		}

		if ( obj[ "previews" ].isString() )
		{
			const auto parsed { obj[ "previews" ].toString().split( ',' ) };

			for ( auto str : parsed )
			{
				str = str.trimmed();

				trans << "INSERT INTO atlas_previews (atlas_id, preview_url) VALUES (?,?)" << atlas_id << str;
			}
		}
		else
		{
			const QJsonArray previews = obj[ "previews" ].toArray();

			for ( const QJsonValue& preview_url : previews )
			{
				trans << "INSERT INTO atlas_previews (atlas_id, preview_url) VALUES (?,?)" << atlas_id
					  << preview_url.toString();
			}
		}
	}

	bool atlasIDExists( Transaction& trans, const AtlasID atlas_id )
	{
		std::optional< AtlasID > id { std::nullopt };
		trans << "SELECT atlas_id FROM atlas_data WHERE atlas_id = ?" << atlas_id >> id;
		return id.has_value();
	}

	void parseAtlasArray( const QJsonArray& data, Transaction& trans )
	{
		ProgressSignaler signaler {};

		const int max { static_cast< int >( data.size() - 1 ) };
		signaler.setMax( max );
		int counter { 0 };
		for ( const auto& obj_data : data )
		{
			const auto& obj { obj_data.toObject() };

			if ( !obj.contains( ( "atlas_id" ) ) )
				throw AtlasException( "Expected atlas_id. Got nothing in obj for update" );

			if ( validateAtlasKeys( obj ) && !atlasIDExists( trans, obj[ "atlas_id" ].toInteger() ) )
				insertAtlasData( obj, trans );
			else if ( atlasIDExists( trans, obj[ "atlas_id" ].toInteger() ) )
				updateAtlasData( obj, trans );
			else
				throw AtlasException( "Failed to update record. Record incomplete and did not exist" );

			++counter;
			signaler.setProgress( counter );
			signaler.setSubMessage( QString( "%1/%2" ).arg( counter ).arg( max ) );
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

		if ( !obj.contains( "f95_id" ) ) throw AtlasException( "F95 data did not contain it's pkey!" );

		const auto pkey { obj[ "f95_id" ].toInteger() };

		for ( const auto& key : keys )
		{
			const auto& data { obj[ key ] };

			switch ( data.type() )
			{
				case QJsonValue::Null:
					trans << format_ns::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key )
						  << std::nullopt << pkey;
					break;
				case QJsonValue::Bool:
					trans << format_ns::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key )
						  << data.toBool() << pkey;
					break;
				case QJsonValue::Double:
					trans << format_ns::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key )
						  << data.toInteger() << pkey;
					break;
				case QJsonValue::String:
					trans << format_ns::format( "UPDATE f95_zone_data SET {} = ? WHERE f95_id = ?", key )
						  << data.toString() << pkey;
					break;
				case QJsonValue::Array:
					throw AtlasException( "Unexpected type when parsing f95 data: Was array" );
				case QJsonValue::Object:
					throw AtlasException( "Unexpected type when parsing f95 data: Was object" );
				case QJsonValue::Undefined:
					throw AtlasException( "Unexpected type when parsing f95 data: Was undefined" );
				default:
					throw AtlasException( "Unexpected type when parsing f95 data: Was default case" );
			}
		}
	}

	void insertF95Data( const QJsonObject& obj, Transaction& trans )
	{
		constexpr std::string_view query { "INSERT INTO f95_zone_data (f95_id, atlas_id, banner_url, site_url, "
			                               "last_thread_comment, thread_publish_date, last_record_update, views, "
			                               "likes, rating, replies) VALUES (?,?,?,?,?,?,?,?,?,?,?)" };

		trans << query << obj[ "f95_id" ].toInteger() << obj[ "atlas_id" ].toInteger() << obj[ "banner_url" ].toString()
			  << obj[ "site_url" ].toString() << obj[ "last_thread_comment" ].toInteger()
			  << obj[ "thread_publish_date" ].toInteger() << obj[ "last_record_update" ].toInteger()
			  << obj[ "views" ].toInteger() << obj[ "likes" ].toInteger() << obj[ "rating" ].toDouble()
			  << obj[ "replies" ].toInteger();

		const F95ID f95_id { static_cast< F95ID >( obj[ "f95_id" ].toInteger() ) };

		if ( obj[ "tags" ].isString() )
		{
			//CSV mode
			const auto parsed { obj[ "tags" ].toString().split( ',' ) };

			for ( const auto& str : parsed )
			{
				const TagID tag_id { atlas::tags::createTag( str ) };

				trans << "INSERT INTO f95_zone_tags (tag_id, f95_id) VALUES (?,?)" << tag_id << f95_id;
			}
		}
		else if ( obj[ "tags" ].isArray() )
		{
			const QJsonArray tags = obj[ "tags" ].toArray();

			for ( const QJsonValue& tag : tags )
			{
				const TagID tag_id { atlas::tags::createTag( tag.toString() ) };

				trans << "INSERT INTO f95_zone_tags (tag_id, f95_id) VALUES (?,?)" << tag_id << f95_id;
			}
		}

		if ( obj[ "screens" ].isString() )
		{
			const auto parsed { obj[ "screens" ].toString().split( ',' ) };

			for ( auto str : parsed )
			{
				str = str.trimmed();

				trans << "INSERT INTO f95_zone_screens (f95_id, screen_url) VALUES (?,?)" << f95_id << str;
			}
		}
		else
		{
			const QJsonArray previews = obj[ "screens" ].toArray();

			for ( const QJsonValue& preview_url : previews )
			{
				trans << "INSERT INTO f95_zone_screens (f95_id, screen_url) VALUES (?,?)" << f95_id
					  << preview_url.toString();
			}
		}
	}

	bool F95IDExists( const F95ID f95_id, Transaction& trans )
	{
		std::optional< F95ID > id;
		trans << "SELECT f95_id FROM f95_zone_data WHERE f95_id = ?" << f95_id >> id;
		return id.has_value();
	}

	void parseF95Array( const QJsonArray& data, Transaction& trans )
	{
		ProgressSignaler signaler { QString( "Processing update for F95 data" ) };
		const int max { static_cast< int >( data.size() - 1 ) };
		signaler.setMax( max );
		int counter { 0 };
		for ( const auto& obj_data : data )
		{
			const auto& obj { obj_data.toObject() };

			if ( validateF95Keys( obj ) && !F95IDExists( obj[ "f95_id" ].toInteger(), trans ) )
				insertF95Data( obj, trans );
			else if ( F95IDExists( obj[ "f95_id" ].toInteger(), trans ) )
				updateF95Data( obj, trans );
			else
				throw AtlasException( "Failed to update record. Record incomplete and did not exist" );

			++counter;
			signaler.setProgress( counter );
			signaler.setMessage( QString( "%1/%2" ).arg( counter ).arg( max ) );
		}
	}

	void processJson( const QJsonObject& json )
	{
		Transaction transaction {};
		try
		{
			const auto keys { json.keys() };
			for ( const auto& table_key : keys )
			{
				if ( table_key == "min_ver" ) continue;

				const auto set { nameToSet( table_key ) };
				if ( set == InvalidSet )
				{
					throw AtlasException( format_ns::format( "Unexpected data in set! Key = {}", table_key ) );
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
					case InvalidSet:
						[[fallthrough]];
					default:
						throw AtlasException( "Unexpected set!" );
				}
			}

			transaction.commit();
		}
		catch ( ... )
		{
			transaction.abort();
			std::rethrow_exception( std::current_exception() );
		}
	}

} // namespace remote::parsers::v0