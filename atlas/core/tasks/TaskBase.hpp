//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_TASK_HPP
#define ATLASGAMEMANAGER_TASK_HPP

#include <string>

#include <QObject>

class TaskBase : public QObject
{
  protected:

	//! Run should return after each progress step. It should NEVER do more then one. Will stop calling once progress == maxProgress
	virtual void run() = 0;

	//! User readable status
	virtual std::string userStatus() = 0;

	//! Returns a user readable status for the current progress 'tick'
	virtual std::string statusProgress() = 0;

	//! Current progress of this task
	virtual std::size_t progress() = 0;

	//! Max progress of this task
	virtual std::size_t maxProgress() = 0;

	friend class TaskManager;
};

#endif //ATLASGAMEMANAGER_TASK_HPP
