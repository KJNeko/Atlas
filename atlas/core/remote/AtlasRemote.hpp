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
	class AtlasRemote : public QObject
	{
		Q_OBJECT

		QThread m_thread {};
		QNetworkAccessManager m_manager {};

	  public:

		AtlasRemote();

		void triggerCheckRemote();
		void triggerDownloadRemote();

	  signals:
		//! Emitted when the remote should be checked.
		void checkRemoteSignal();
		void downloadRemoteSignal();
		//! Emitted when the remote has an update to download/process
		void updateAvailable();
		//! Emitted when an update has been downloaded and is able to be processed.
		void updateDownloaded( const std::filesystem::path path );
		//! Emitted when an update has finished processing.
		void updateComplete( const std::filesystem::path path );

		void downloadUpdate( const std::uint64_t update_time );

	  private slots:
		//! Updates the local DB with the updates available
		void updateRemoteList();
		void processUpdateFile( const std::uint64_t update_time );
		void downloadUpdateSlot( const std::uint64_t update_time );
		void handleJsonResponse( QNetworkReply* reply );
		void handleDownloader( QNetworkReply* reply );
		void downloadRemote();
	};

	void initRemoteHandler();
	void shutdownRemoteHandler();

	AtlasRemote& atlasRemote();
} // namespace atlas

#endif //ATLASGAMEMANAGER_ATLASREMOTE_HPP
