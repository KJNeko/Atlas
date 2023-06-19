//
// Created by kj16609 on 6/6/23.
//

#include "AtlasRemote.hpp"

#include <fstream>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/database/Database.hpp"
#include "core/logging.hpp"
#include "core/utils/regex/regex.hpp"
#include "extract.hpp"
#include "ui/notifications/NotificationPopup.hpp"
#include "ui/notifications/ProgressMessage.hpp"

#define REMOTE "https://atlas-gamesdb.com/"

//#define REMOTE_BACKUP "https://futa.zip/atlas"

namespace atlas
{

	namespace internal
	{
		inline static AtlasRemote* remote { nullptr };
	}

	void initRemoteHandler()
	{
		if ( internal::remote == nullptr ) internal::remote = new AtlasRemote();

		internal::remote->triggerCheckRemote();
	}

	void shutdownRemoteHandler()
	{
		delete internal::remote;
	}

	AtlasRemote& atlasRemote()
	{
		if ( internal::remote == nullptr ) throw std::runtime_error( "AtlasRemote has not been initialized!" );

		return *internal::remote;
	}

	AtlasRemote::AtlasRemote()
	{
		m_manager.moveToThread( &m_thread );
		moveToThread( &m_thread );
		m_thread.start();

		connect( this, &AtlasRemote::checkRemoteSignal, this, &AtlasRemote::updateRemoteList );
		connect( this, &AtlasRemote::downloadRemoteSignal, this, &AtlasRemote::downloadRemote );
		connect( this, &AtlasRemote::downloadUpdate, this, &AtlasRemote::downloadUpdateSlot );
	}

	void AtlasRemote::triggerCheckRemote()
	{
		emit checkRemoteSignal();
	}

	void AtlasRemote::updateRemoteList()
	{
		ZoneScoped;
		const QString path { REMOTE "api/updates" };
		spdlog::info( "Checking remote for updates at {}", path.toStdString() );
		QNetworkRequest request { QUrl { path } };
		auto* reply { m_manager.get( request ) };

		connect(
			reply,
			&QNetworkReply::finished,
			this,
			[ =, this ]() { handleJsonResponse( reply ); },
			Qt::SingleShotConnection );
	}

	void AtlasRemote::handleJsonResponse( QNetworkReply* reply )
	try
	{
		ZoneScoped;
		spdlog::debug( "Handling json response from {}", reply->url().path().toStdString() );
		//Read the json

		if ( reply->error() != QNetworkReply::NoError )
		{
			spdlog::warn(
				"Failed to handle json response from {}. Server returned code {}",
				reply->url().path().toStdString(),
				reply->errorString().toStdString() );
			return;
		}

		const QByteArray response_data { reply->readAll() };
		const QJsonDocument doc { QJsonDocument::fromJson( response_data ) };
		if ( !doc.isArray() )
		{
			spdlog::warn(
				"Failed to handle json response from {}. The json response was not an array!",
				reply->url().path().toStdString() );
			spdlog::warn( "{}", response_data.toStdString() );
		}
		const QJsonArray array = doc.array();
		/*
			[
			  {
			    "date": 1686679308,
			    "name": "1686679308.update",
			    "md5": "c199a08bde792f0674cab838dd900d3a"
			  }
			]
		 */

		//Start checking the updates. Add them to the updates table if they are not already there.
		const auto updates = []() -> std::vector< std::pair< std::uint64_t, std::uint64_t > >
		{
			std::vector< std::pair< std::uint64_t, std::uint64_t > > tmp_updates;
			Transaction t { Autocommit };
			t << "SELECT update_time, processed_time FROM updates" >>
				[ &tmp_updates ]( const std::uint64_t update_time, const std::uint64_t processed_time )
			{ tmp_updates.emplace_back( update_time, processed_time ); };
			return tmp_updates;
		}();

		//Check if the update is already in the database
		for ( const auto& data : array )
		{
			qDebug() << data;
			const auto& obj { data.toObject() };
			if ( !obj.contains( "date" ) || !obj.contains( "name" ) || !obj.contains( "md5" ) )
			{
				spdlog::warn(
					"Failed to handle json response from {}. Json object possibly malformed!",
					reply->url().path().toStdString() );
				delete reply;
				return;
			}

			const auto& date { obj[ "date" ].toInteger() };
			const auto& md5_str { obj[ "md5" ].toString() };
			const auto& md5 { QByteArray::fromHex( md5_str.toUtf8() ) };
			std::vector< std::byte > md5_data_c {};
			md5_data_c.resize( static_cast< unsigned long >( md5.size() ) );
			memcpy( md5_data_c.data(), md5.data(), static_cast< size_t >( md5.size() ) );

			const auto it = std::find_if(
				updates.begin(),
				updates.end(),
				[ date ]( const auto& pair ) { return pair.first == static_cast< std::uint64_t >( date ); } );

			Transaction t { Autocommit };
			if ( it == updates.end() )
			{
				spdlog::debug( "Adding update {} to database", date );
				//Add it to the database
				t << "INSERT INTO updates (update_time, processed_time, md5) VALUES (?, ?, ?)" << date << 0
				  << md5_data_c;
			}

			t.commit();
		}
		triggerDownloadRemote();
		reply->deleteLater();
	}
	catch ( const std::exception& e )
	{
		spdlog::error(
			"Failed to handle json response from {}. Exception: {}", reply->url().path().toStdString(), e.what() );
	}

	void AtlasRemote::downloadUpdateSlot( const std::uint64_t update_time )
	{
		ZoneScoped;
		const auto update_path { fmt::format( REMOTE "packages/{}.update", update_time ) };
		QNetworkRequest request { QUrl { QString::fromStdString( update_path ) } };
		auto* reply { m_manager.get( request ) };

		connect(
			reply,
			&QNetworkReply::finished,
			this,
			[ =, this ]() { handleDownloader( reply ); },
			Qt::SingleShotConnection );
	}

	void AtlasRemote::handleDownloader( QNetworkReply* reply )
	{
		spdlog::debug( "Handling downloader from {}", reply->url().path().toStdString() );
		ZoneScoped;

		if ( reply->error() != QNetworkReply::NoError )
		{
			spdlog::warn(
				"Failed to handle downloader from {}. Server returned code {}",
				reply->url().path().toStdString(),
				reply->errorString().toStdString() );
			return;
		}

		const QString file_name { reply->url().fileName() };
		const std::filesystem::path dest_folder { "./data/updates/" };
		std::filesystem::create_directories( dest_folder );
		if ( std::ofstream ofs( dest_folder / file_name.toStdString(), std::ios::binary ); ofs )
		{
			while ( !reply->atEnd() )
			{
				std::array< std::byte, 4096 > buffer;
				const auto read_bytes { reply->read( reinterpret_cast< char* >( buffer.data() ), buffer.size() ) };
				ofs.write( reinterpret_cast< char* >( buffer.data() ), read_bytes );

				if ( read_bytes == 0 ) break;
			}
		}

		//Signal to process update file
		emit processUpdateFile( static_cast< uint64_t >( file_name.left( file_name.lastIndexOf( '.' ) )
		                                                     .toLongLong() ) );
		reply->deleteLater();
	}

	void AtlasRemote::triggerDownloadRemote()
	{
		emit downloadRemoteSignal();
	}

	void AtlasRemote::downloadRemote()
	{
		ZoneScoped;
		//Check what updates are available
		const auto updates = []()
		{
			std::vector< std::tuple< std::uint64_t, std::uint64_t, std::vector< std::byte > > > tmp_updates;
			Transaction t { Autocommit };
			t << "SELECT update_time, processed_time, md5 FROM updates" >>
				[ &tmp_updates ](
					const std::uint64_t update_time, const std::uint64_t processed_time, std::vector< std::byte > md5 )
			{ tmp_updates.emplace_back( update_time, processed_time, std::move( md5 ) ); };
			return tmp_updates;
		}();

		for ( const auto& [ update_time, processed_time, md5 ] : updates )
		{
			//If processed time is not zero then it's been processed at some point.
			if ( processed_time != 0 ) continue;

			//Check if the file exists
			const std::filesystem::path local_update_archive_path {
				fmt::format( "./data/updates/{}.update", update_time )
			};
			if ( std::filesystem::exists( local_update_archive_path ) )
			{
				//Check if the md5 matches
				const auto local_md5 { [ &local_update_archive_path ]() -> std::vector< std::byte >
					                   {
										   QCryptographicHash hash { QCryptographicHash::Md5 };
										   QFile file { local_update_archive_path };
										   file.open( QIODevice::ReadOnly );
										   hash.addData( &file );
										   const auto data { hash.result() };

										   std::vector< std::byte > data_c {};
										   data_c.resize( static_cast< unsigned long >( data.size() ) );
										   memcpy( data_c.data(), data.data(), static_cast< size_t >( data.size() ) );

										   return data_c;
									   }() };

				if ( local_md5 == md5 )
				{
					//File exists and is correct. Can be processed now.
					emit processUpdateFile( update_time );
				}
				else
				{
					//local hex
					const auto local_hex {
						QByteArrayView( local_md5.data(), local_md5.size() ).toByteArray().toHex().toStdString()
					};
					const auto md5_hex { QByteArrayView( md5.data(), md5.size() ).toByteArray().toHex().toStdString() };

					spdlog::warn(
						"Update {} exists but md5 {} vs {} doesn't match. Downloading again.",
						update_time,
						local_hex,
						md5_hex );
					//Download the file from remote.
					std::filesystem::remove( fmt::format( "./data/updates/{}.update", update_time ) );
					emit downloadUpdate( update_time );
				}
			}
			else //File doesn't exist
			{
				//Download the file from remote.
				emit downloadUpdate( update_time );
			}
		}
	}

	bool idExists( const std::string table_name, const std::uint64_t id, Transaction& Transaction )
	{
		ZoneScoped;
		bool exists = false;
		Transaction << fmt::format( "SELECT EXISTS(SELECT 1 FROM {} WHERE id = {} LIMIT 1)", table_name, id ) >>
			[ &exists ]( const bool e ) { exists = e; };
		return exists;
	}

	void updateData(
		const std::string& table_name, const std::uint64_t id, const QJsonObject& obj, Transaction& transaction )
	{
		const auto keys { obj.keys() };
		std::string query { fmt::format( "UPDATE {} SET ", table_name ) };

		bool first { true };

		for ( int i = 0; i < keys.size(); ++i )
		{
			const auto key { keys[ i ] };
			const auto value { obj[ key ] };

			switch ( value.type() )
			{
				case QJsonValue::Bool:
					if ( first )
						first = false;
					else
						query += ", ";
					query += fmt::format( "{} = {}", key.toStdString(), value.toBool() );
					break;
				case QJsonValue::Double:
					if ( first )
						first = false;
					else
						query += ", ";
					query += fmt::format( "{} = {}", key.toStdString(), value.toDouble() );
					break;
				case QJsonValue::String:
					if ( first )
						first = false;
					else
						query += ", ";
					query += fmt::format(
						"{} = '{}'", key.toStdString(), value.toString().replace( '\'', "\'\'" ).toStdString() );
					//If not last key and (next index is not above max and next object is not null or undefined or object or array)
					break;
				case QJsonValue::Array:
					[[fallthrough]];
				case QJsonValue::Object:
					[[fallthrough]];
				case QJsonValue::Undefined:
					[[fallthrough]];
				case QJsonValue::Null:
					[[fallthrough]];
				default:
					break;
			}
		}

		query += fmt::format( " WHERE id = {}", id );
		transaction << query;
	}

	void insertData( const std::string& table_name, const QJsonObject& obj, Transaction& transaction )
	{
		const auto keys { obj.keys() };
		std::string query { fmt::format( "INSERT INTO {} (", table_name ) };

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

			if ( value.isArray() )
			{
				//In this case we use the supporting table
				const std::string supporting_table { fmt::format( "{}_data_{}", table_name, key.toStdString() ) };
				//Does the supporting table exist?
				int count;
				transaction << "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?;"
							<< supporting_table
					>> count;

				if ( count == 0 )
				{
					//Create a table for now.
					transaction << fmt::format(
						"CREATE TABLE {}_data_{} (id INTEGER PRIMARY KEY REFERENCES {} (id), value);",
						table_name,
						key.toStdString(),
						supporting_table );
				}

				//Insert the data into the supporting table
				transaction << fmt::format( "INSERT INTO {} (value) VALUES (?);", supporting_table )
							<< value.toString().toStdString();
			}
			else if ( value.isDouble() )
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

	void parse( const std::vector< char > file_data )
	{
		ZoneScoped;
		const QByteArray array { file_data.data(), static_cast< qsizetype >( file_data.size() ) };

		const QJsonObject json { QJsonDocument::fromJson( array ).object() };

		Transaction transaction { NoAutocommit };

		for ( const auto& table_key : json.keys() )
		{
			std::size_t row_counter { 0 };
			ZoneScopedN( "Process set" );
			const auto key_str { table_key.toStdString() };
			TracyMessage( key_str.c_str(), key_str.size() );
			const QJsonArray data = json[ table_key ].toArray();

			//Name of the table we intend to shove all this data into.
			const std::string table_name { table_key.endsWith( "_data" ) ? table_key.toStdString() :
				                                                           table_key.toStdString() + "_data" };

			//The query we will be building onto
			const std::string query_start { fmt::format( "INSERT INTO {} (", table_name ) };

			for ( const auto& obj_data : data )
			{
				ZoneScopedN( "Process object" );
				const auto obj { obj_data.toObject() };

				const auto keys { obj.keys() };

				std::string query { query_start };

				assert( !keys.empty() );
				assert( keys.contains( "id" ) );
				const std::uint64_t id { static_cast< std::uint64_t >( obj[ "id" ].toInteger() ) };

				//Check if table exists
				bool table_exists { false };
				transaction << "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?;" << table_name
					>> table_exists;

				if ( !table_exists )
				{
					//If the table doesn't exist then we make a temporary table where everything is turned into strings instead.
					//Later on the client (when doing it's startup) can properly check the tables and set them as needed. This should be a rare case.
					//Since this will only really ever happen if the database gets updated before the client is ready to parse (or if a client is out of date)
					std::string table_creation_query { fmt::format( "CREATE TABLE IF NOT EXISTS {} (", table_name ) };

					for ( const auto& key : keys )
					{
						const auto item_type { obj[ key ].type() };

						switch ( item_type )
						{
							case QJsonValue::Null:
								[[fallthrough]];
							case QJsonValue::Bool:
								[[fallthrough]];
							case QJsonValue::Double:
								[[fallthrough]];
							case QJsonValue::String:
								//Later on we can try to 'hint' types. For now this works.
								table_creation_query += fmt::format( "{}", key.toStdString() );
								break;
							case QJsonValue::Array:
								//Ignored since we make the table later on if required.
								break;
							case QJsonValue::Object:
								[[fallthrough]];
							case QJsonValue::Undefined:
								[[fallthrough]];
							default:
								break;
						}
					}

					table_creation_query += ");";

					transaction << table_creation_query;
				}

				if ( idExists( table_name, id, transaction ) )
					updateData( table_name, id, obj, transaction );
				else
					insertData( table_name, obj, transaction );

				++row_counter;

				if ( row_counter % 1000 == 0 ) spdlog::info( "Processed {} rows for table {}", row_counter, table_key );
			}
		}

		transaction.commit();
	}

	void AtlasRemote::processUpdateFile( const std::uint64_t update_time )
	try
	{
		ZoneScoped;
		spdlog::info( "Processing update for time {}", update_time );
		//Check if the file exists
		const std::filesystem::path local_update_archive_path {
			fmt::format( "./data/updates/{}.update", update_time )
		};

		if ( !std::filesystem::exists( local_update_archive_path ) )
		{
			spdlog::warn( "Update {} doesn't exist. Can't process.", update_time );
			return;
		}

		//Check if the file is already processed
		const auto processed_time { [ &update_time ]()
			                        {
										std::uint64_t tmp_processed_time { 0 };
										Transaction t { Autocommit };
										t << "SELECT processed_time FROM updates WHERE update_time = ?" << update_time
											>> [ &tmp_processed_time ]( const std::uint64_t processed_time_i )
										{ tmp_processed_time = processed_time_i; };
										return tmp_processed_time;
									}() };

		if ( processed_time != 0 )
		{
			spdlog::warn( "Update {} is already processed.", update_time );
			return;
		}

		spdlog::info( "Processing file {:ce}", local_update_archive_path );
		atlas::parse( atlas::extract( local_update_archive_path ) );

		Transaction transaction { NoAutocommit };
		transaction << "UPDATE updates SET processed_time = ? WHERE update_time = ?;"
					<< std::chrono::steady_clock::now().time_since_epoch().count() << update_time;
		transaction.commit();
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::error( "Error parsing update {}. {}: query: {}", update_time, e.what(), e.get_sql() );
	}

} // namespace atlas