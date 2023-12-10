//
// Created by kj16609 on 7/21/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP
#define ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP

#include <QDialog>

class Notification;

QT_BEGIN_NAMESPACE

namespace Ui
{
	class NotificationManagerUI;
}

QT_END_NAMESPACE

class Notification;

class NotificationManagerUI final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( NotificationManagerUI )

	std::uint64_t active_notifications { 0 };

	std::vector< Notification* > notifications() const;

  public:

	void setHeight();

	void addNotification( Notification* notif );

	void resizeEvent( QResizeEvent* ) override;
	void moveEvent( QMoveEvent* event ) override;

	explicit NotificationManagerUI( QWidget* parent = nullptr );
	~NotificationManagerUI() override;

  private:

	Ui::NotificationManagerUI* ui;

  private slots:
	void deleteNotification( Notification* ptr );
	void on_btnClearNotifications_pressed();
	void on_btnMinimize_pressed();

  signals:
	void requestMove();
};

#endif //ATLASGAMEMANAGER_NOTIFICATIONMANAGERUI_HPP
