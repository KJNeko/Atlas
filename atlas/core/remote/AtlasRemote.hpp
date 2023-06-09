//
// Created by kj16609 on 6/6/23.
//

#ifndef ATLASGAMEMANAGER_ATLASREMOTE_HPP
#define ATLASGAMEMANAGER_ATLASREMOTE_HPP

#include <QThread>

namespace atlas
{

	//! Manages all remote connections to the Atlas remote server
	class AtlasRemote
	{
		QThread m_thread {};
	};

	void initRemoteHandler();
	void shutdownRemoteHandler();

	AtlasRemote& atlasRemote();
} // namespace atlas

#endif //ATLASGAMEMANAGER_ATLASREMOTE_HPP
