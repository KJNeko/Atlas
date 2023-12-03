//
// Created by kj16609 on 6/6/23.
//

#include "AtlasRemote.hpp"

#include <moc_AtlasRemote.cpp>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <tracy/TracyC.h>

#include <fstream>

#include "core/database/RapidTransaction.hpp"
#include "core/logging/logging.hpp"
#include "core/notifications/notifications.hpp"
#include "core/remote/parsers/parser.hpp"
#include "core/utils/regex/regex.hpp"
#include "extract.hpp"

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
		if ( internal::remote == nullptr ) throw AtlasException( "AtlasRemote has not been initialized!" );

		return *internal::remote;
	}

	AtlasRemote::AtlasRemote()
	{
		m_manager.moveToThread( &m_thread );
		moveToThread( &m_thread );
		m_thread.start();

		connect( this, &AtlasRemote::checkRemoteSignal, this, &AtlasRemote::check );
		connect( this, &AtlasRemote::triggerDownloadFor, this, &AtlasRemote::downloadUpdate );
		connect( this, &AtlasRemote::triggerParseFor, this, &AtlasRemote::processUpdateFile );
	}

	void AtlasRemote::triggerCheckRemote()
	{
		emit checkRemoteSignal();
	}

	void AtlasRemote::check()
	{
		downloadManifest();
	}

	void AtlasRemote::downloadManifest()
	{
		ZoneScoped;
		const QString path { REMOTE "api/updates" };
		atlas::logging::info( "Checking remote for updates at {}", path.toStdString() );
		QNetworkRequest request { QUrl { path } };
		request.setTransferTimeout( 5000 );
		auto* reply { m_manager.get( request ) };

		connect(
			reply,
			&QNetworkReply::finished,
			this,
			[ =, this ]() { handleJsonResponse( reply ); },
			Qt::SingleShotConnection );

		connect(
			reply,
			&QNetworkReply::errorOccurred,
			this,
			[ =, this ]( const QNetworkReply::NetworkError& error ) { handleManifestError( error, reply ); },
			Qt::SingleShotConnection );
	}

	void AtlasRemote::handleDownloader( QNetworkReply* reply )
	{
		atlas::logging::debug( "Handling download from {}", reply->url().path().toStdString() );
		ZoneScoped;

		if ( reply->error() != QNetworkReply::NoError )
		{
			atlas::logging::warn(
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

	void AtlasRemote::downloadUpdate( const std::uint64_t update_time )
	{
		ZoneScoped;
		const auto update_path { format_ns::format( REMOTE "packages/{}.update", update_time ) };
		QNetworkRequest request { QUrl { QString::fromStdString( update_path ) } };
		auto* reply { m_manager.get( request ) };

		connect(
			reply,
			&QNetworkReply::finished,
			this,
			[ =, this ]() { handleDownloader( reply ); },
			Qt::SingleShotConnection );

		connect(
			reply,
			&QNetworkReply::errorOccurred,
			this,
			[ =, this ]( const QNetworkReply::NetworkError error ) { handleDownloadError( error, reply ); },
			Qt::SingleShotConnection );
	}

	void AtlasRemote::handleJsonResponse( QNetworkReply* reply )
	try
	{
		ZoneScoped;
		atlas::logging::debug( "Handling json response from {}", reply->url().path().toStdString() );
		//Read the json

		if ( reply->error() != QNetworkReply::NoError )
		{
			logging::warn(
				"Failed to handle json response from {}. Server returned code {}",
				reply->url().path().toStdString(),
				reply->errorString().toStdString() );
			return;
		}

		const QByteArray response_data { reply->readAll() };
		atlas::logging::debug( "Response returned {} bytes", response_data.size() );
		const QJsonDocument doc { QJsonDocument::fromJson( response_data ) };

		if ( doc.isNull() )
		{
			logging::warn(
				"Failed to handle json response from {}. The json response was null!",
				reply->url().path().toStdString() );
			return;
		}

		if ( !doc.isArray() )
		{
			logging::warn(
				"Failed to handle json response from {}. The json response was not an array!",
				reply->url().path().toStdString() );
			logging::warn( "{}", response_data.toStdString() );
			return;
		}
		const QJsonArray& array = doc.array();

		//Check if the update is already in the database
		for ( const auto& data : array )
		{
			const auto& obj { data.toObject() };
			if ( !obj.contains( "date" ) || !obj.contains( "name" ) || !obj.contains( "md5" ) )
			{
				logging::warn(
					"Failed to handle json response from {}. Json object possibly malformed!",
					reply->url().path().toStdString() );
				delete reply;
				return;
			}

			const std::uint64_t update_time { static_cast< std::uint64_t >( obj[ "date" ].toInteger() ) };

			atlas::logging::debug( "Processing update for timestamp: {}", update_time );

			//Blacklist records that are broken/invalid/old
			if ( update_time == 1686886200 || update_time == 1687918793 ) continue;

			const auto& md5_str { obj[ "md5" ].toString() };
			const auto& md5 { QByteArray::fromHex( md5_str.toUtf8() ) };
			std::vector< std::byte > md5_data_c {};
			md5_data_c.resize( static_cast< unsigned long >( md5.size() ) );
			memcpy( md5_data_c.data(), md5.data(), static_cast< size_t >( md5.size() ) );

			const auto updates { getUpdatesList() };
			const auto it { std::find_if(
				updates.begin(),
				updates.end(),
				[ update_time ]( const auto& pair )
				{ return pair.first == static_cast< std::uint64_t >( update_time ); } ) };

			RapidTransaction t {};
			if ( it == updates.end() )
			{
				atlas::logging::debug( "Adding update {} to database", update_time );
				//Add it to the database
				t << "INSERT INTO updates (update_time, processed_time, md5) VALUES (?, ?, ?)" << update_time << 0
				  << md5_data_c;
				triggerDownloadFor( update_time );
			}
		}
		reply->deleteLater();

		processPendingUpdates();
	}
	catch ( const std::exception& e )
	{
		atlas::logging::error(
			"Failed to handle json response from {}. Exception: {}", reply->url().path().toStdString(), e.what() );
	}

	std::vector< std::pair< std::uint64_t, std::uint64_t > > AtlasRemote::getUpdatesList() const
	{
		std::vector< std::pair< std::uint64_t, std::uint64_t > > tmp_updates;
		RapidTransaction t {};
		t << "SELECT update_time, processed_time FROM updates ORDER BY update_time ASC" >>
			[ &tmp_updates ]( const std::uint64_t update_time, const std::uint64_t processed_time )
		{ tmp_updates.emplace_back( update_time, processed_time ); };
		return tmp_updates;
	}

	std::uint64_t AtlasRemote::getNextUpdateTime() const
	{
		const auto updates { getUpdatesList() };

		for ( const auto& [ update_time, processed_time ] : updates )
		{
			atlas::logging::debug( "Update time: {}, processed time: {}", update_time, processed_time );
			if ( processed_time != 0 ) continue;
			return update_time;
		}

		return 0;
	}

	void parse( const std::vector< char > file_data )
	{
		ZoneScoped;
		const QByteArray array { file_data.data(), static_cast< qsizetype >( file_data.size() ) };

		const QJsonObject& json { QJsonDocument::fromJson( array ).object() };

		//Figure out what version we are trying to parse.

		if ( !json.contains( "min_ver" ) )
		{
			throw AtlasException( "Failed to parse update file. Missing min_ver" );
		}

		const std::uint64_t version { static_cast< std::uint64_t >( json[ "min_ver" ].toInteger() ) };

		using namespace remote::parsers;

		if ( version > MAX_REMOTE_VERSION )
		{
			atlas::logging::
				error( "Failed to parse update file! Version was {}. Our max is {}", version, MAX_REMOTE_VERSION );
			return;
		}

		switch ( version )
		{
			default:
				{
					atlas::logging::error(
						"Failed to parse update file! Version was {}. Our max is {}!", version, MAX_REMOTE_VERSION );
					return;
				}
			case 0:
				{
					v0::processJson( json );
					break;
				}
		}
	}

	void AtlasRemote::processUpdateFile( const std::uint64_t update_time )
	{
		ZoneScoped;
		atlas::logging::info( "Processing update for time {}", update_time );
		//Check if the file exists
		const std::filesystem::path local_update_archive_path {
			format_ns::format( "./data/updates/{}.update", update_time )
		};

		if ( !std::filesystem::exists( local_update_archive_path ) )
		{
			atlas::logging::warn( "Update {} doesn't exist. Can't process.", update_time );
			return;
		}

		//Check if the file is already processed
		const auto processed_time { [ &update_time ]()
			                        {
										std::uint64_t tmp_processed_time { 0 };
										RapidTransaction t {};
										t << "SELECT processed_time FROM updates WHERE update_time = ?" << update_time
											>> [ &tmp_processed_time ]( const std::uint64_t processed_time_i ) noexcept
										{ tmp_processed_time = processed_time_i; };
										return tmp_processed_time;
									}() };

		if ( processed_time != 0 )
		{
			atlas::logging::warn( "Update {} is already processed.", update_time );
			return;
		}

		//Ensure that we are updating IN ORDER.
		const auto next_update { getNextUpdateTime() };
		//getNextUpdateTime() can return 0 but.....If it returns zero then i've VERY concerned how we got here?
		if ( next_update == 0 )
		{
			atlas::logging::critical( "What the fuck? HOW are you zero?!?" );
			std::abort(); //Just abort the entire program at this point.
		}

		if ( update_time != next_update )
		{
			//We are about to update out of order. Abort.
			return;
		}

		atlas::logging::debug( "Processing file {:ce}", local_update_archive_path );
		try
		{
			auto data { atlas::extract( local_update_archive_path ) };
#ifndef NDEBUG
			if ( std::ofstream ofs( local_update_archive_path.string() + ".decoded" ); ofs )
			{
				ofs.write( data.data(), data.size() );
			}
#endif

			atlas::parse( std::move( data ) );
			markComplete( update_time );

			//Check if the next update file is ready to go
			const auto next_time { getNextUpdateTime() };
			if ( next_time != 0
			     && std::filesystem::exists( format_ns::format( "./data/updates/{}.update", next_time ) ) )
			{
				//Trigger it
				processPendingUpdates();
			}
		}
		catch ( const std::exception& e )
		{
			atlas::logging::error( "Failed to process update file {}", update_time );
		}
	}

	void AtlasRemote::processPendingUpdates()
	try
	{
		atlas::logging::debug( "Starting to process pending updates" );
		auto update_time { getNextUpdateTime() };

		while ( update_time != 0 )
		{
			const std::filesystem::path path { format_ns::format( "./data/updates/{}.update", update_time ) };
			atlas::logging::debug( "Processing update: {}", path );
			if ( !std::filesystem::exists( path ) ) return;

			processUpdateFile( update_time );

			markComplete( update_time );
			update_time = getNextUpdateTime();
		}
	}
	catch ( std::exception& e )
	{
		atlas::logging::warn( "Failed to process updates: {}", e.what() );
	}

	void AtlasRemote::markComplete( const std::uint64_t update_time, const bool yes )
	{
		const std::uint64_t update_now {
			static_cast< uint64_t >( std::chrono::duration_cast<
										 std::chrono::seconds >( std::chrono::system_clock::now().time_since_epoch() )
			                             .count() )
		};

		RapidTransaction() << "UPDATE updates SET processed_time = ? WHERE update_time = ?" << ( yes ? update_now : 0 )
						   << update_time;

		atlas::logging::info( "Processed update for time {} at {}", update_time, update_now );
	}

	void AtlasRemote::handleManifestError( QNetworkReply::NetworkError error, QNetworkReply* reply )
	{
		reply->deleteLater();
		atlas::logging::error( "Failed to download manifest from remote: {}", static_cast< int >( error ) );
		switch ( error )
		{
			default:
				[[fallthrough]];
			case QNetworkReply::NoError:
				break;
			case QNetworkReply::ConnectionRefusedError:
				break;
			case QNetworkReply::RemoteHostClosedError:
				break;
			case QNetworkReply::HostNotFoundError:
				break;
			case QNetworkReply::TimeoutError:
				atlas::logging::error( "Timed out" );
				break;
			case QNetworkReply::OperationCanceledError:
				break;
			case QNetworkReply::SslHandshakeFailedError:
				break;
			case QNetworkReply::TemporaryNetworkFailureError:
				break;
			case QNetworkReply::NetworkSessionFailedError:
				break;
			case QNetworkReply::BackgroundRequestNotAllowedError:
				break;
			case QNetworkReply::TooManyRedirectsError:
				break;
			case QNetworkReply::InsecureRedirectError:
				break;
			case QNetworkReply::UnknownNetworkError:
				break;
			case QNetworkReply::ProxyConnectionRefusedError:
				break;
			case QNetworkReply::ProxyConnectionClosedError:
				break;
			case QNetworkReply::ProxyNotFoundError:
				break;
			case QNetworkReply::ProxyTimeoutError:
				break;
			case QNetworkReply::ProxyAuthenticationRequiredError:
				break;
			case QNetworkReply::UnknownProxyError:
				break;
			case QNetworkReply::ContentAccessDenied:
				break;
			case QNetworkReply::ContentOperationNotPermittedError:
				break;
			case QNetworkReply::ContentNotFoundError:
				break;
			case QNetworkReply::AuthenticationRequiredError:
				break;
			case QNetworkReply::ContentReSendError:
				break;
			case QNetworkReply::ContentConflictError:
				break;
			case QNetworkReply::ContentGoneError:
				break;
			case QNetworkReply::UnknownContentError:
				break;
			case QNetworkReply::ProtocolUnknownError:
				break;
			case QNetworkReply::ProtocolInvalidOperationError:
				break;
			case QNetworkReply::ProtocolFailure:
				break;
			case QNetworkReply::InternalServerError:
				break;
			case QNetworkReply::OperationNotImplementedError:
				break;
			case QNetworkReply::ServiceUnavailableError:
				break;
			case QNetworkReply::UnknownServerError:
				break;
		}
	}

	void AtlasRemote::handleDownloadError( QNetworkReply::NetworkError error, QNetworkReply* reply )
	{
		reply->deleteLater();
		logging::error( "Failed to download file from remote: {}", static_cast< int >( error ) );
		switch ( error )
		{
			default:
				[[fallthrough]];
			case QNetworkReply::NoError:
				break;
			case QNetworkReply::ConnectionRefusedError:
				break;
			case QNetworkReply::RemoteHostClosedError:
				break;
			case QNetworkReply::HostNotFoundError:
				break;
			case QNetworkReply::TimeoutError:
				logging::error( "Timed out" );
				break;
			case QNetworkReply::OperationCanceledError:
				break;
			case QNetworkReply::SslHandshakeFailedError:
				break;
			case QNetworkReply::TemporaryNetworkFailureError:
				break;
			case QNetworkReply::NetworkSessionFailedError:
				break;
			case QNetworkReply::BackgroundRequestNotAllowedError:
				break;
			case QNetworkReply::TooManyRedirectsError:
				break;
			case QNetworkReply::InsecureRedirectError:
				break;
			case QNetworkReply::UnknownNetworkError:
				break;
			case QNetworkReply::ProxyConnectionRefusedError:
				break;
			case QNetworkReply::ProxyConnectionClosedError:
				break;
			case QNetworkReply::ProxyNotFoundError:
				break;
			case QNetworkReply::ProxyTimeoutError:
				break;
			case QNetworkReply::ProxyAuthenticationRequiredError:
				break;
			case QNetworkReply::UnknownProxyError:
				break;
			case QNetworkReply::ContentAccessDenied:
				break;
			case QNetworkReply::ContentOperationNotPermittedError:
				break;
			case QNetworkReply::ContentNotFoundError:
				break;
			case QNetworkReply::AuthenticationRequiredError:
				break;
			case QNetworkReply::ContentReSendError:
				break;
			case QNetworkReply::ContentConflictError:
				break;
			case QNetworkReply::ContentGoneError:
				break;
			case QNetworkReply::UnknownContentError:
				break;
			case QNetworkReply::ProtocolUnknownError:
				break;
			case QNetworkReply::ProtocolInvalidOperationError:
				break;
			case QNetworkReply::ProtocolFailure:
				break;
			case QNetworkReply::InternalServerError:
				break;
			case QNetworkReply::OperationNotImplementedError:
				break;
			case QNetworkReply::ServiceUnavailableError:
				break;
			case QNetworkReply::UnknownServerError:
				break;
		}
	}

} // namespace atlas