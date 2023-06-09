//
// Created by kj16609 on 6/6/23.
//

#include "AtlasRemote.hpp"

#include "RemoteDownloader.hpp"

namespace atlas
{

	namespace internal
	{
		inline static AtlasRemote* remote { nullptr };

	}

	void initRemoteHandler()
	{
		if ( internal::remote == nullptr ) internal::remote = new AtlasRemote();

		//Start scanner
		RemoteDownloader downloader;
		downloader.checkRemote();
	}

	void shutdownRemoteHandler()
	{
		delete internal::remote;
	}

	AtlasRemote& atlasRemote()
	{
		if ( internal::remote == nullptr ) throw std::runtime_error( "AtlasRemote has not been initialized!" );

		return *internal::remote;
	}
} // namespace atlas