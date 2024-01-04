//
// Created by kj16609 on 7/18/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_POOLS_HPP
#define ATLASGAMEMANAGER_POOLS_HPP

#include <QThreadPool>

namespace atlas::threading
{

	struct ThreadPools
	{
		QThreadPool image_loaders {};
		QThreadPool image_importers {};
		QThreadPool importers {};
		QThreadPool pre_importers {};

		ThreadPools() { reloadConfig(); }

		void reloadConfig();
	};

	ThreadPools& globalPools();
} // namespace atlas::threading

#endif //ATLASGAMEMANAGER_POOLS_HPP
