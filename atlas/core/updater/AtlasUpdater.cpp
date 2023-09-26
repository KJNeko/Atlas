#include "AtlasUpdater.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "core/logging/logging.hpp"

#define REPO "https://api.github.com/repos/KJNeko/Atlas"

//#define REMOTE_BACKUP "https://futa.zip/atlas"

namespace atlas
{
	namespace internal
	{
		inline static AtlasUpdater* uManager { nullptr };
	}
    void initUpdateHandler()
	{
		if ( internal::uManager == nullptr ) internal::uManager = new updadat_manager();

		internal::uManager->check_for_updates();
	}

	void shutdownUpdateHandler()
	{
		delete internal::uManager;
	}

	AtlasUpdater::AtlasUpdater()
	{
		m_manager.moveToThread( &m_thread );
		moveToThread( &m_thread );
		m_thread.start();
	}

    void AtlasUpdater::check_for_updates()
    {
      
    }
}