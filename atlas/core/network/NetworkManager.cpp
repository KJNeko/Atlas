//
// Created by kj16609 on 12/9/23.
//

#include "NetworkManager.hpp"

#include <QFuture>
#include <QNetworkReply>
#include <QThread>

#include "core/exceptions.hpp"
#include "core/logging/logging.hpp"

namespace atlas::network
{
	namespace internal
	{
		inline static std::unique_ptr< NetworkManager > s_network_manager { nullptr };
		inline static std::unique_ptr< QThread > s_network_thread { nullptr };
	} // namespace internal

	NetworkManager& NetworkManager::instance()
	{
		if ( !internal::s_network_manager )
		{
			internal::s_network_thread = std::make_unique< QThread >();
			internal::s_network_thread->start();
			internal::s_network_manager = std::make_unique< NetworkManager >();
			internal::s_network_manager->moveToThread( internal::s_network_thread.get() );
			QMetaObject::invokeMethod( internal::s_network_manager.get(), "setup", Qt::QueuedConnection );
		}
		return *internal::s_network_manager;
	}

	void NetworkManager::setup()
	{
		atlas::logging::debug( "Network manager setup" );
		m_access_manager = std::make_unique< QNetworkAccessManager >();
		timer.setInterval( 500 );
		timer.setSingleShot( false );
		timer.start();
		connect( &timer, &QTimer::timeout, this, &NetworkManager::onTimeout );
	}

	NetworkManager::NetworkManager()
	{
		atlas::logging::debug( "Network manager created" );
		timer.moveToThread( internal::s_network_thread.get() );
	}

	void NetworkManager::onTimeout()
	{
		if ( download_queue.empty() ) return;

		Downloader* downloader { download_queue.front().release() };
		download_queue.pop();

		if ( !downloader ) throw AtlasException( "Downloader is null" );

		downloader->run( *m_access_manager );
	}

	QFuture< QImage > NetworkManager::getImage( const QUrl& url )
	{
		std::unique_ptr< Downloader > downloader { std::make_unique< Downloader >() };
		QFuture< QImage > image_future { downloader->getImage( url ) };
		downloader->moveToThread( internal::s_network_thread.get() );
		download_queue.emplace( std::move( downloader ) );
		return image_future;
	}

	QFuture< QImage > Downloader::getImage( const QUrl& url )
	{
		m_url = url;
		m_signaler = std::make_unique< ProgressSignaler >();
		m_signaler->setMessage( "Downloading image" );
		m_signaler->setSubMessage( url.toString() );
		m_signaler->setProgress( 0 );
		m_signaler->setMax( 0 );

		promise = new QPromise< QImage >();

		promise->start();

		return promise->future();
	}

	void Downloader::downloadFinished()
	try
	{
		if ( m_reply == nullptr )
		{
			atlas::logging::error( "Reply is null for url: {}", m_url );
			promise->setException( std::make_exception_ptr<
								   AtlasException >( format_ns::format( "Reply is null for url: {}", m_url ) ) );
			promise->finish();
			this->deleteLater();
			return;
		}

		QNetworkReply* reply { m_reply };
		atlas::logging::debug( "Finished downloading image from url: \"{}\"", reply->url().toString() );
		if ( reply->error() != QNetworkReply::NoError )
		{
			atlas::logging::error(
				"Failed to download image from url: \"{}\". Error: {}", reply->url().toString(), reply->errorString() );
			promise->setException( std::make_exception_ptr< AtlasException >( reply->errorString().toStdString() ) );
			promise->finish();
			reply->deleteLater();
			this->deleteLater();
			return;
		}
		else
		{
			const auto image_data { reply->readAll() };

			if ( image_data.size() == 0 )
			{
				atlas::logging::error( "Recieved zero bytes from \"{}\"", reply->url() );
				promise->setException( std::make_exception_ptr< AtlasException >( "Recieved zero bytes" ) );
				promise->finish();
			}

			QImage pixmap {};
			pixmap.loadFromData( image_data );

			if ( pixmap.isNull() )
			{
				atlas::logging::error( "Failed to load image from url: \"{}\"", reply->url().toString() );
				promise->setException( std::make_exception_ptr< AtlasException >( "Failed to load image" ) );
				promise->finish();
				reply->deleteLater();
				this->deleteLater();
				return;
			}

			atlas::logging::debug( "Finished loading image from url: \"{}\"", reply->url().toString() );

			promise->addResult( std::move( pixmap ) );
			promise->finish();
			reply->deleteLater();
			this->deleteLater();
			return;
		}
	}
	catch ( ... )
	{
		// Catch literaly anything to ensure we don't leak memory.
		this->deleteLater();
	}

	void Downloader::run( QNetworkAccessManager& manager )
	{
		QNetworkRequest request { m_url };
		m_reply = manager.get( request );
		connect( m_reply, &QNetworkReply::finished, this, &Downloader::downloadFinished );
		connect( m_reply, &QNetworkReply::downloadProgress, this, &Downloader::downloadProgress );
	}

	void Downloader::downloadProgress( qint64 bytesReceived, qint64 bytesTotal )
	{
		m_signaler->setProgress( bytesReceived );
		m_signaler->setMax( bytesTotal );
	}

} // namespace atlas::network
