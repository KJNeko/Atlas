//
// Created by kj16609 on 7/18/23.
//

#include "pools.hpp"

#include "config.hpp"

namespace internal
{
	inline static ThreadPools global_pools;
}

void ThreadPools::reloadConfig()
{
	image_loaders.setMaxThreadCount( config::threads::image_loader_threads::get() );
	image_importers.setMaxThreadCount( config::threads::image_import_threads::get() );
	importers.setMaxThreadCount( config::threads::import_threads::get() );
	pre_importers.setMaxThreadCount( config::threads::import_pre_loader_threads::get() );
}

ThreadPools& globalPools()
{
	return internal::global_pools;
}
