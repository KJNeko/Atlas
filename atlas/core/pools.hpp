//
// Created by kj16609 on 7/18/23.
//

#ifndef ATLASGAMEMANAGER_POOLS_HPP
#define ATLASGAMEMANAGER_POOLS_HPP

#include <QThreadPool>

struct ThreadPools
{
	QThreadPool image_loaders {};
	QThreadPool image_importers {};

	ThreadPools() { reloadConfig(); }

	void reloadConfig();
};

ThreadPools& globalPools();

#endif //ATLASGAMEMANAGER_POOLS_HPP
