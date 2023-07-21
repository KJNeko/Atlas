//
// Created by kj16609 on 7/21/23.
//

#include "notifications.hpp"

#include <QFuture>
#include <QPromise>
#include <QThread>
#include <QTimer>

namespace atlas::notifications
{
	namespace internal
	{
		inline static NotificationManagerUI* notification_manager { nullptr };
	}

	void initNotifications( QWidget* parent )
	{
		internal::notification_manager = new NotificationManagerUI( parent );
		internal::notification_manager->show();
	}

	NotificationManagerUI& handle()
	{
		if ( internal::notification_manager == nullptr )
			throw std::runtime_error( "Init notification handler before accessing handle" );
		return *internal::notification_manager;
	}

	void createMessage( QString message )
	{
		handle().createMessage( std::move( message ) );
	}

	Signaler< ProgressNotification > createProgressMessage( QString str )
	{
		return handle().createProgressMessage( std::move( str ) );
	}

} // namespace atlas::notifications

void NotificationManagerUI::createMessage( QString message )
{
	using namespace atlas::notifications;

	if ( this->thread() == QThread::currentThread() )
	{
		auto* ptr { new MessageNotification( std::move( message ), internal::notification_manager ) };
		ptr->show();
		internal::notification_manager->addNotification( ptr );
	}
	else
	{
		QTimer* timer { new QTimer() };
		timer->moveToThread( this->thread() );
		timer->setSingleShot( true );

		QObject::connect(
			timer,
			&QTimer::timeout,
			[ message, timer ]()
			{
				auto* ptr { new MessageNotification( std::move( message ), internal::notification_manager ) };
				ptr->show();
				internal::notification_manager->addNotification( ptr );
				timer->deleteLater();
			} );

		QMetaObject::invokeMethod( timer, "start", Qt::QueuedConnection, Q_ARG( int, 0 ) );
	}
}

atlas::notifications::Signaler< ProgressNotification > NotificationManagerUI::createProgressMessage( QString str )
{
	using namespace atlas::notifications;

	if ( this->thread() == QThread::currentThread() )
	{
		auto* ptr { new ProgressNotification( internal::notification_manager ) };
		auto signaler { ptr->getSignaler() };
		ptr->show();
		signaler->setMessage( str );
		internal::notification_manager->addNotification( ptr );
		return signaler;
	}
	else
	{
		QTimer* timer { new QTimer() };
		timer->moveToThread( this->thread() );
		timer->setSingleShot( true );

		std::promise< Signaler< ProgressNotification > > promise {};
		std::future< Signaler< ProgressNotification > > future { promise.get_future() };

		QObject::connect(
			timer,
			&QTimer::timeout,
			[ str, timer, &promise ]()
			{
				auto* ptr { new ProgressNotification( internal::notification_manager ) };
				auto signaler { ptr->getSignaler() };
				ptr->show();
				signaler->setMessage( str );
				internal::notification_manager->addNotification( ptr );
				promise.set_value( std::move( signaler ) );
			} );

		QMetaObject::invokeMethod( timer, "start", Qt::QueuedConnection, Q_ARG( int, 0 ) );

		return future.get();
	}
}
