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
}

NotificationManagerUI::~NotificationManagerUI()
{
	delete ui;
}

void NotificationManagerUI::addNotification( Notification* notif )
{
	connect( notif, &Notification::selfClosePtr, this, &NotificationManagerUI::deleteNotification );
	active_notifications.emplace_back( notif );
	reorderChildren();
	ui->label->setText( QString( "%1 notifications" ).arg( active_notifications.size() ) );
}

void NotificationManagerUI::reorderChildren()
{
	// No children so we should hide ourselves
	if ( active_notifications.size() == 0 )
		this->hide();
	else
		this->show();

	QPoint g_pos { this->mapToGlobal( this->rect().topLeft() ) };

	for ( Notification* child : active_notifications )
	{
		//Force child to be as small as possible
		child->resize( child->sizeHint() );
		child->updateGeometry();

		// Offset spacing
		g_pos -= QPoint( 0, 4 );

		// Offset vertical size
		g_pos -= QPoint( 0, child->height() );

		child->move( g_pos );
	}
}

void NotificationManagerUI::moveEvent( QMoveEvent* event )
{
	QWidget::moveEvent( event );
	reorderChildren();
}

void NotificationManagerUI::deleteNotification( Notification* ptr )
{
	if ( auto itter = std::find( active_notifications.begin(), active_notifications.end(), ptr );
	     itter != active_notifications.end() )
	{
		Notification* ptr_in { *itter };
		ptr_in->close();
		ptr_in->deleteLater();
		active_notifications.erase( itter );
	}
	else
		spdlog::warn( "Tried to delete a notification that didn't exist in the list!" );

	reorderChildren();
	ui->label->setText( QString( "%1 notifications" ).arg( active_notifications.size() ) );
}
