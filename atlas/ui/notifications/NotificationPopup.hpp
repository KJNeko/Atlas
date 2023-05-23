//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_TASKPOPUP_HPP
#define ATLASGAMEMANAGER_TASKPOPUP_HPP

#include <future>
#include <memory>

#include <QDialog>
#include <QThread>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class NotificationPopup;
}

QT_END_NAMESPACE

template < typename T >
concept has_signaler = requires( T t ) {
	{
		t.getSignaler()
	} -> std::same_as< std::unique_ptr< typename T::Signaler > >;
};

template < typename T >
concept has_void_signaler = requires( T t ) {
	{
		t.getSignaler()
	} -> std::same_as< void >;
};

template < typename T > concept is_notification = has_signaler< T >;

class NotificationPopup final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( NotificationPopup )

  public:

	explicit NotificationPopup( QWidget* parent = nullptr );
	~NotificationPopup() override;

	void resizeEvent( QResizeEvent* event ) override;

	void addMessage( QWidget* notification );

	void removeMessage( QWidget* notification );
	void expand();

	template < typename T >
		requires( is_notification< T > && !has_void_signaler< T > )
	std::unique_ptr< typename T::Signaler > createNotification( const QString name, const bool reveal = false )
	{
		//All of this should run on the main thread

		using Signaler = std::unique_ptr< typename T::Signaler >;

		if ( this->thread() == QThread::currentThread() )
		{
			T* notif { new T( name ) };
			Signaler signaler { notif->getSignaler() };
			addMessage( notif );
			return signaler;
		}
		else
		{
			//This is the hackiest solution i've ever fucking found. But apparently it works
			QTimer* timer { new QTimer() };
			timer->moveToThread( this->thread() );
			timer->setSingleShot( true );

			std::promise< Signaler > promise;
			std::future< Signaler > notif_future { promise.get_future() };

			QObject::connect(
				timer,
				&QTimer::timeout,
				[ name, &promise, timer, this, reveal ]()
				{
					if ( reveal ) expand();
					T* notif { new T( name ) };
					addMessage( notif );
					promise.set_value( notif->getSignaler() );

					timer->deleteLater();
				} );

			QMetaObject::invokeMethod( timer, "start", Qt::QueuedConnection, Q_ARG( int, 0 ) );

			return notif_future.get();
		}
	}

	template < typename T >
		requires has_void_signaler< T >
	void createNotification( const QString name, const bool reveal = false )
	{
		if ( this->thread() == QThread::currentThread() )
		{
			if ( reveal ) expand();

			addMessage( new T( name ) );
			return;
		}
		else
		{
			QTimer* timer { new QTimer() };
			timer->moveToThread( this->thread() );
			timer->setSingleShot( true );

			QObject::connect(
				timer,
				&QTimer::timeout,
				[ name, timer, this, reveal ]()
				{
					if ( reveal ) expand();
					T* notif { new T( name ) };
					addMessage( notif );

					timer->deleteLater();
				} );

			QMetaObject::invokeMethod( timer, "start", Qt::QueuedConnection, Q_ARG( int, 0 ) );
		}
	}

  signals:
	void popupResized();

  private:

	Ui::NotificationPopup* ui;

  signals:

  private slots:
	void on_btnHideShow_clicked();
};

void initNotificationPopup( QWidget* parent );
NotificationPopup* getNotificationPopup();

#endif //ATLASGAMEMANAGER_TASKPOPUP_HPP
