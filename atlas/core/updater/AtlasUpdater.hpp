



#ifndef ATLASGAMEMANAGER_ATLASUPDATER_HPP
#define ATLASGAMEMANAGER_ATLASUPDATER_HPP

#include <QNetworkReply>
#include <QThread>
#include <QProgressDialog>

class QNetworkReply;

#include <filesystem>
#include <ui/dialog/UpdateDialog.h>

namespace atlas
{
    //Manage Updates for Atlas
    class AtlasUpdater final :  public QObject
    {
		//Q_DISABLE_COPY_MOVE( AtlasUpdater )
        Q_OBJECT

        //QThread m_thread {};
		QNetworkAccessManager m_manager {};

        public:
		AtlasUpdater();
		AtlasUpdater(const AtlasUpdater&) = delete;
		AtlasUpdater& operator=(const AtlasUpdater&) = delete;
		~AtlasUpdater();
        //AtlasUpdater(QObject* parent, std::shared_ptr<gui_settings> gui_settings);
        void check_for_updates(bool isManual);


        private:

		std::uint64_t converToEpoch( QString time );
		std::uint64_t converToShortEpoch( QString time );
		void handleJsonResponse( QNetworkReply* reply, bool isManual );
		  void handleManifestError( QNetworkReply::NetworkError error, QNetworkReply* reply );
		  void downloadUpdate( QString url );
		  void saveFile( QNetworkReply* reply );
		  int updateMessageBox( QString message, QString windowTitle, bool includeChangelog );
		  void downloadProgress( int ist, int max );
		  UpdateDialog* ud = new UpdateDialog();
	};
    
    void initUpdateHandler(bool isManual);
	void shutdownUpdateHandler();
}

#endif //ATLASGAMEMANAGER_ATLASUPDATER_HPP
