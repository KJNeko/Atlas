//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NotificationManagerUI.h" resolved

#include "NotificationManagerUI.hpp"

#include <moc_NotificationManagerUI.cpp>

#include "core/logging.hpp"
#include "ui/notifications/Notification.hpp"
#include "ui_NotificationManagerUI.h"

NotificationManagerUI::NotificationManagerUI( QWidget* parent ) :
  QDialog( parent, Qt::Dialog | Qt::FramelessWindowHint ),
  ui( new Ui::NotificationManagerUI )
{
	ui->setupUi( this );
	this->resize( this->minimumSize() );
	this->updateGeometry();
}

NotificationManagerUI::~NotificationManagerUI()
{
	delete ui;
}

QSize NotificationManagerUI::sizeHint() const
{
	const auto min_size { minimumSize() };
	QSize size { min_size.width(), 0 };
}

void NotificationManagerUI::addNotification( Notification* notif )
{
	connect( notif, &Notification::selfClosePtr, this, &NotificationManagerUI::deleteNotification );
	ui->notifications->layout()->addWidget( notif );
	this->resize( this->minimumSize() );
	this->updateGeometry();
	ui->label->setText( QString( "%1 notifications" ).arg( ++active_notifications ) );
}

void NotificationManagerUI::resizeEvent( QResizeEvent* event )
{
	emit requestMove();
	QDialog::resizeEvent( event );
}

void NotificationManagerUI::moveEvent( QMoveEvent* event )
{
	QWidget::moveEvent( event );
}

void NotificationManagerUI::deleteNotification( Notification* ptr )
{
	ptr->setParent( nullptr );
	ptr->close();
	ptr->deleteLater();
	this->resize( this->minimumSize() );

	ui->label->setText( QString( "%1 notifications" ).arg( --active_notifications ) );
}
