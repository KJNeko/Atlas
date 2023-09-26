



#ifndef ATLASGAMEMANAGER_UPDATE_MANAGER_HPP
#define ATLASGAMEMANAGER_UPDATE_MANAGER_HPP

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
        void check_for_updates();
		AtlasUpdater();

    };
    
    void initUpdateHandler();
	void shutdownUpdateHandler();
}

#endif //ATLASGAMEMANAGER_UPDATE_MANAGER_HPP
