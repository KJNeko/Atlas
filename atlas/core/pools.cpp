//
// Created by kj16609 on 7/18/23.
//

#include "pools.hpp"

namespace internal
{
	inline static ThreadPools global_pools;
}

void ThreadPools::reloadConfig()
{
	image_loaders.setMaxThreadCount( 2 );
	image_importers.setMaxThreadCount( 4 );
}

ThreadPools& globalPools()
{
	return internal::global_pools;
}
