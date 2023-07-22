//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP

#include <QDialog>

#include "ProgressNotification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class NotificationManagerUI;
}

QT_END_NAMESPACE

class Notification;

class NotificationManagerUI : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( NotificationManagerUI )

	std::vector< Notification* > active_notifications {};

  public:

	void addNotification( Notification* notif );

	void resizeEvent( QResizeEvent* ) override;
	void moveEvent( QMoveEvent* event ) override;

	explicit NotificationManagerUI( QWidget* parent = nullptr );
	~NotificationManagerUI() override;

  private:

	void reorderChildren();

	Ui::NotificationManagerUI* ui;

  private slots:
	void deleteNotification( Notification* ptr );

  signals:
	void requestMove();
};

#endif //ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP
