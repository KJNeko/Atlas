#include "fileDownloader.hpp"

#include "core/database/record/game/Game.hpp"

FileDownloader::FileDownloader( QUrl imageUrl, atlas::records::Game game, QObject* parent ) :
  QObject( parent ),
  m_game( game )
{
	if ( !game.valid() ) return; // Do fuck all

	connect( &m_WebCtrl, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded );

	/*
		connect(
			&m_WebCtrl,
			SIGNAL( finished( QNetworkReply* ) ),
			this,
			SLOT( fileDownloaded( QNetworkReply*, record_id ) ) );
		 */

	QNetworkRequest request( imageUrl );
	m_WebCtrl.get( request );
}

FileDownloader::~FileDownloader()
{}

void FileDownloader::fileDownloaded( QNetworkReply* pReply )
{
	m_DownloadedData = pReply->readAll();
	//emit a signal
	pReply->deleteLater();
	emit downloaded( this, m_game );
}

QByteArray FileDownloader::downloadedData() const
{
	return m_DownloadedData;
}