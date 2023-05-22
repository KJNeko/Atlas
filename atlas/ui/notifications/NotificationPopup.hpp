//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_TASKPOPUP_HPP
#define ATLASGAMEMANAGER_TASKPOPUP_HPP

#include <future>
#include <memory>

#include <QDialog>
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
		if ( reveal ) expand();

		//All of this should run on a separate thread
		auto* notif { new T( name ) };
		auto signaler { notif->getSignaler() };
		addMessage( notif );

		std::promise< std::unique_ptr< typename T::Signaler > > promise;
		std::future< std::unique_ptr< typename T::Signaler > > future { promise.get_future() };
		promise.set_value( std::move( signaler ) );

		return future.get();
	}

	template < typename T >
		requires has_void_signaler< T >
	void createNotification( const QString name, const bool reveal = false )
	{
		if ( reveal ) expand();

		addMessage( new T( name ) );
		return;
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
