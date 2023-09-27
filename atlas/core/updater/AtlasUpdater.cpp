#include "AtlasUpdater.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "core/logging/logging.hpp"

#define REPO "https://api.github.com/repos/KJNeko/Atlas"

//#define REMOTE_BACKUP "https://futa.zip/atlas"

namespace atlas
{
	namespace internal
	{
		inline static AtlasUpdater* uManager { nullptr };
	}
    void initUpdateHandler()
	{
		if ( internal::uManager == nullptr ) internal::uManager = new AtlasUpdater();

		internal::uManager->check_for_updates();
	}

	void shutdownUpdateHandler()
	{
		delete internal::uManager;
	}

	AtlasUpdater::AtlasUpdater()
	{
		/*m_manager.moveToThread( &m_thread );
		moveToThread( &m_thread );
		m_thread.start();*/
	}

    void AtlasUpdater::check_for_updates()
    {
      	QUrl url("https://api.github.com/repos/KJNeko/Atlas/releases");
		qInfo() << url.toString();
		atlas::logging::info( "Checking github for updates at {}", url.toString() );
		QNetworkRequest request { url};
		request.setTransferTimeout( 2000 );
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

	void AtlasUpdater::handleJsonResponse( QNetworkReply* reply )
	try
	{
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

		const QString response_data { reply->readAll() };
		//qInfo() << response_data;
		QJsonDocument jsonResponse  { QJsonDocument::fromJson( response_data.toUtf8() ) };

		

		if ( !jsonResponse.isArray() )
		{
			logging::warn(
				"Failed to handle json response from {}. The json response was not an array!",
				reply->url().path().toStdString() );
			logging::warn( "{}", response_data.toStdString() );
		}
		else{
			QJsonObject jsonObject = jsonResponse.object();
			const QJsonArray& array = jsonResponse.array();

			for ( const auto& data : array )
			{
				const auto& obj { data.toObject() };
				qInfo() << obj[ "tag_name" ].toString();
				qInfo() << obj[ "created_at" ].toString();
				for ( const auto& assets : obj["assets"].toArray())
				{
					const auto& asset { assets.toObject() };
					qInfo() << asset[ "browser_download_url" ].toString();
				}
			}
		}
		reply->deleteLater();

	}
	catch ( const std::exception& e )
	{
		atlas::logging::error(
			"Failed to handle json response from {}. Exception: {}", reply->url().path().toStdString(), e.what() );
	}
	void AtlasUpdater::handleManifestError( QNetworkReply::NetworkError error, QNetworkReply* reply )
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
}