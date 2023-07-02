//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GAMESCANNER_HPP
#define ATLASGAMEMANAGER_GAMESCANNER_HPP

#include <QFuture>
#include <QFutureWatcher>
#include <QThreadPool>

#include "GameImportData.hpp"

class GameScanner final : public QObject
{
	Q_OBJECT

	QThreadPool m_thread_pool {};
	QFuture< void > m_runner_future {};
	QFutureWatcher< void > m_watcher {};

	void mainRunner( QPromise< void >& promise, const std::filesystem::path base, const QString regex );

  public:

	void start( const std::filesystem::path path, const QString regex );
	void wait();

	~GameScanner() override;

  public slots:
	void pause();
	void resume();
	void abort();
	bool isRunning();
	bool isPaused();

	void emitComplete() { emit scanComplete(); }

  signals:
	void scanComplete();
	void foundGame( const GameImportData data );
};

#endif //ATLASGAMEMANAGER_GAMESCANNER_HPP
