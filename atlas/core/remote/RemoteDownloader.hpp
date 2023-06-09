//
// Created by kj16609 on 6/6/23.
//

#ifndef ATLASGAMEMANAGER_REMOTEDOWNLOADER_HPP
#define ATLASGAMEMANAGER_REMOTEDOWNLOADER_HPP

#include <filesystem>

#include <QJsonObject>
#include <QTimer>

#define REMOTE "https://atlas-gamesdb.com/"

//#define REMOTE_BACKUP "https://futa.zip/atlas/"

class RemoteDownloader : public QObject
{
	Q_OBJECT

	QTimer check_timer {};
	std::size_t m_last_check { 0 };

  public:

	RemoteDownloader();

  private:

	void parse( const std::filesystem::path path );

  public slots:
	void checkRemote();

  signals:
	void processData( QJsonObject data );
};

#endif //ATLASGAMEMANAGER_REMOTEDOWNLOADER_HPP
