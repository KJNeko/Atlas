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

	QFuture< void > m_runner_future {};
	QFutureWatcher< void > m_watcher {};

	void mainRunner( QPromise< void >& promise, const std::filesystem::path base, const QString regex );

  public:

	std::atomic< uint64_t > directories_left { 0 };

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
	//! Emitted when the prescanner has reached the point of waiting for the other threads to finish their work.
	void prescanWaiting();
};

#endif //ATLASGAMEMANAGER_GAMESCANNER_HPP
