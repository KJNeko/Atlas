#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "core/Types.hpp"
#include "core/database/record/game/Game.hpp"

class FileDownloader final : public QObject
{
	Q_OBJECT

	atlas::records::Game m_game;

  public:

	explicit FileDownloader( QUrl imageUrl, atlas::records::Game record_id, QObject* parent = nullptr );
	~FileDownloader();
	QByteArray downloadedData() const;

  signals:
	void downloaded( const FileDownloader* fdownloader, atlas::records::Game );

  private slots:
	void fileDownloaded( QNetworkReply* pReply );

  private:

	QNetworkAccessManager m_WebCtrl {};
	QByteArray m_DownloadedData {};
};

#endif // FILEDOWNLOADER_H