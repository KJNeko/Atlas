//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP
#define ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP

#include <QFuture>
#include <QFutureWatcher>
#include <QMetaObject>

#include "Notification.hpp"
#include "NotificationSignalers.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class ProgressNotification;
}

QT_END_NAMESPACE

class ProgressNotification : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressNotification )

  public:

	explicit ProgressNotification( QWidget* parent = nullptr );
	~ProgressNotification() override;

	friend class ProgressSignaler;

  private:

	Ui::ProgressNotification* ui;
};

class ProgressSignaler : public atlas::notifications::NotificationSignaler
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressSignaler )

  public:

	ProgressSignaler();
	ProgressSignaler( QString str );

	void setMax( int i );
	void setProgress( int i );
	void setSubMessage( const QString str );
	void setMessage( const QString str );

	~ProgressSignaler();

  private:

	void hookSignaler( ProgressNotification* notif );

  public slots:

	void triggerSelfClose() { emit selfClose(); }

  signals:
	void progressChanged( int i );
	void maxChanged( int i );
	void messageChanged( const QString str );
	void subMessageChanged( const QString str );
	void selfClose();
};

template < typename T >
class FutureWatcherSignaler final : public QFuture< T >
{
	Q_DISABLE_COPY_MOVE( FutureWatcherSignaler )

	using Future = QFuture< T >;
	using Watcher = QFutureWatcher< T >;

	Future m_future;
	Watcher m_watcher {};
	ProgressSignaler m_signaler;

	std::vector< QMetaObject::Connection > m_signals;

  public:

	FutureWatcherSignaler() = delete;

	FutureWatcherSignaler( QFuture< T > future ) : m_future( std::move( future ) ), m_signaler()
	{
		m_signals.emplace_back( connect(
			&m_watcher,
			&Watcher::progressValueChanged,
			[ this ]( const int progress ) { m_signaler.setProgress( progress ); } ) );

		m_signals.emplace_back( connect(
			&m_watcher,
			&Watcher::progressRangeChanged,
			[ this ]( [[maybe_unused]] const int min, const int max ) { m_signaler.setMax( max ); } ) );

		m_signals.emplace_back( connect(
			&m_watcher,
			&Watcher::finished,
			[ this ]()
			{
				m_signaler.setSubMessage( "Finished" );
				m_signaler.triggerSelfClose();
			} ) );

		m_signals.emplace_back(
			connect( &m_watcher, &Watcher::started, [ this ]() { m_signaler.setSubMessage( "Started" ); } ) );

		m_watcher.setFuture( m_future );
	}

	~FutureWatcherSignaler()
	{
		for ( auto& signal : m_signals )
		{
			m_watcher.disconnect( signal );
		}
	}
};

#endif //ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP
