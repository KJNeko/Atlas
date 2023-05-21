//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_TASKMANAGER_HPP
#define ATLASGAMEMANAGER_TASKMANAGER_HPP

#include <queue>

#include <QFuture>
#include <QtConcurrent>

#include "TaskBase.hpp"

class TaskManager : public QObject
{
	QList< TaskBase* > tasks;

	template < typename T, typename Task >
	QFuture< T > enqueue( Task task )
	{
		auto future = QtConcurrent::run( task );
		return future;
	}
};

#endif //ATLASGAMEMANAGER_TASKMANAGER_HPP
