



#ifndef ATLASGAMEMANAGER_ATLASUPDATER_HPP
#define ATLASGAMEMANAGER_ATLASUPDATER_HPP

#include <QNetworkReply>
#include <QThread>

class QNetworkReply;

#include <filesystem>

namespace atlas
{
    //Manage Updates for Atlas
    class AtlasUpdater final :  public QObject
    {
        Q_OBJECT

        QThread m_thread {};
		QNetworkAccessManager m_manager {};


        public:
        //AtlasUpdater(QObject* parent, std::shared_ptr<gui_settings> gui_settings);
        void check_for_updates(bool isManual);
		AtlasUpdater();

        private:

		  long int converToEpoch( QString time );
		  long int converToShortEpoch( QString time );
		  void handleJsonResponse( QNetworkReply* reply, bool isManual );
		  void handleManifestError( QNetworkReply::NetworkError error, QNetworkReply* reply );
		  void downloadUpdate( QString url );
		  void saveFile( QNetworkReply* reply );
	};
    
    void initUpdateHandler(bool isManual);
	void shutdownUpdateHandler();
}

#endif //ATLASGAMEMANAGER_ATLASUPDATER_HPP
