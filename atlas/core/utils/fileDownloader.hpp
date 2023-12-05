#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class FileDownloader final : public QObject
{
	Q_OBJECT

  public:

	explicit FileDownloader( QUrl imageUrl, QObject* parent = nullptr );
	~FileDownloader();
	QByteArray downloadedData() const;

  signals:
	void downloaded();

  private slots:
	void fileDownloaded( QNetworkReply* pReply );

  private:

	QNetworkAccessManager m_WebCtrl {};
	QByteArray m_DownloadedData {};
};

#endif // FILEDOWNLOADER_H