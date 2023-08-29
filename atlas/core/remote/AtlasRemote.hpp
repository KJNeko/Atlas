//
// Created by kj16609 on 6/6/23.
//

#ifndef ATLASGAMEMANAGER_ATLASREMOTE_HPP
#define ATLASGAMEMANAGER_ATLASREMOTE_HPP

#include <QNetworkReply>
#include <QThread>

class QNetworkReply;

#include <filesystem>

namespace atlas
{
	//! Manages all remote connections to the Atlas remote server
	class AtlasRemote final : public QObject
	{
		Q_OBJECT

		QThread m_thread {};
		QNetworkAccessManager m_manager {};

		void downloadManifest();
		void downloadUpdates();
		void processPendingUpdates();
		void markComplete( const std::uint64_t update_time, const bool yes = true );

		//! Returns a list of all updates.
		std::vector< std::pair< std::uint64_t, std::uint64_t > > getUpdatesList() const;

		//! Returns the next update to process. Returns 0 if no updates to process.
		std::uint64_t getNextUpdateTime() const;

	  public:

		AtlasRemote();

		void triggerCheckRemote();

	  signals:
		//! Emitted when the remote should be checked.
		void checkRemoteSignal();
		//! Asks the remote to download the timestamp file
		void triggerDownloadFor( const std::uint64_t timestamp );
		void triggerParseFor( const std::uint64_t timestamp );

	  private slots:
		//! Updates the local DB with the updates available
		void processUpdateFile( const std::uint64_t update_time );

		void downloadUpdate( const std::uint64_t update_time );

		void handleManifestError( QNetworkReply::NetworkError error );

		void handleDownloadError( QNetworkReply::NetworkError error );

		//! Handles manifest requests from the server.
		void handleJsonResponse( QNetworkReply* reply );

		//! Handles download responses from the remote
		void handleDownloader( QNetworkReply* reply );

		//! Causes the remote to go through a full check. Asking for new updates and processing them.
		void check();
		void handleNetworkError( QNetworkReply::NetworkError );
	};

	void initRemoteHandler();
	void shutdownRemoteHandler();

	AtlasRemote& atlasRemote();
} // namespace atlas

#endif //ATLASGAMEMANAGER_ATLASREMOTE_HPP
