//
// Created by kj16609 on 12/9/23.
//

#pragma once

#include <QNetworkAccessManager>
#include <QPromise>
#include <QTimer>

#include <queue>

#include "core/Types.hpp"
#include "ui/notifications/ProgressNotification.hpp"

namespace atlas::network
{

	class Downloader final : public QObject
	{
		Q_OBJECT

		QUrl m_url {};
		QPromise< QImage >* promise {};
		QNetworkReply* m_reply { nullptr };
		std::unique_ptr< ProgressSignaler > m_signaler { nullptr };

		Q_DISABLE_COPY_MOVE( Downloader )

	  public:

		Downloader() = default;

		QFuture< QImage > getImage( const QUrl& url );

		void run( QNetworkAccessManager& manager );

	  private slots:
		void downloadFinished();
		void downloadProgress( qint64 bytesReceived, qint64 bytesTotal );
	};

	class NetworkManager : public QObject
	{
		Q_OBJECT

		std::unique_ptr< QNetworkAccessManager > m_access_manager {};

		std::queue< std::unique_ptr< Downloader > > download_queue;

		QTimer timer {};

		Q_DISABLE_COPY_MOVE( NetworkManager )

	  public:

		NetworkManager();

		QNetworkAccessManager& accessManager() { return *m_access_manager; }

		static NetworkManager& instance();

		QFuture< QImage > getImage( const QUrl& url );

	  private slots:
		void onTimeout();
		void setup();
	};

} // namespace atlas::network