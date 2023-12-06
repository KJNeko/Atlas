#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "core/Types.hpp"

class FileDownloader final : public QObject
{
	Q_OBJECT

  public:

	explicit FileDownloader( QUrl imageUrl, QObject* parent = nullptr, RecordID record_id = 0 );
	~FileDownloader();
	QByteArray downloadedData() const;

  signals:
	void downloaded( const FileDownloader* fdownloader, const RecordID id );

  private slots:
	void fileDownloaded( QNetworkReply* pReply, const FileDownloader* fdownloader, RecordID record_id );

  private:

	QNetworkAccessManager m_WebCtrl {};
	QByteArray m_DownloadedData {};
};

#endif // FILEDOWNLOADER_H