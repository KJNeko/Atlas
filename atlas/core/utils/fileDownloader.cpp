#include "fileDownloader.hpp"

FileDownloader::FileDownloader( QUrl imageUrl, QObject* parent, RecordID record_id ) : QObject( parent )
{
	if ( record_id != 0 )
	{
		connect(
			&m_WebCtrl,
			SIGNAL( finished( QNetworkReply* ) ),
			this,
			SLOT( fileDownloaded( QNetworkReply*, record_id ) ) );

		QNetworkRequest request( imageUrl );
		m_WebCtrl.get( request );
	}
}

FileDownloader::~FileDownloader()
{}

void FileDownloader::fileDownloaded( QNetworkReply* pReply, const FileDownloader* fdownloader, RecordID record_id )
{
	m_DownloadedData = pReply->readAll();
	//emit a signal
	pReply->deleteLater();
	emit downloaded( fdownloader, record_id );
}

QByteArray FileDownloader::downloadedData() const
{
	return m_DownloadedData;
}