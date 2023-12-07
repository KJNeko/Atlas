#include "fileDownloader.hpp"

#include "core/database/record/game/Game.hpp"

FileDownloader::FileDownloader( QUrl imageUrl, atlas::records::Game game, QObject* parent ) :
  QObject( parent ),
  m_game( game )
{
	if ( !game.valid() ) throw AtlasException( "Invalid game given to FileDownloader" );
	if ( imageUrl.isEmpty() ) throw AtlasException( "Empty URL given to FileDownloader" );

	connect( &m_WebCtrl, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded );

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