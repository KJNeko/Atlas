//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NotificationManagerUI.h" resolved

#include "NotificationManagerUI.hpp"

#include <moc_NotificationManagerUI.cpp>

#include "core/logging.hpp"
#include "ui/notifications/Notification.hpp"
#include "ui_NotificationManagerUI.h"
#include "core/config.hpp"

NotificationManagerUI::NotificationManagerUI( QWidget* parent ) :
  QDialog( parent, Qt::Dialog | Qt::FramelessWindowHint ),
  ui( new Ui::NotificationManagerUI )
{
	ui->setupUi( this );
	this->resize( this->minimumSize() );
	//Max height of window is 600;
	this->setMaximumHeight( 550 );
	ui->scrollArea->setMaximumHeight( 550 );
	spdlog::info( "Max size: {}", NotificationManagerUI::maximumHeight() );
}

NotificationManagerUI::~NotificationManagerUI()
{
	delete ui;
}

void NotificationManagerUI::addNotification( Notification* notif )
{
	//Get current count of notifications

	notificationWidgetHeight += notificationWidgetHeight == 0 ? notif->height() + ui->notifications->height() : notif->height();
	//spdlog::info("TOTAL height{}", notificationWidgetHeight);
	//int count { ui->notifications->layout()->count() };
	//int height { notif->height() };
	//spdlog::info( "ADD Notification Height :{}", notif->height() );
	//spdlog::info( "ADD total items:{} item_height:{}", count, (notif->height() * (count) ) );
	connect( notif, &Notification::selfClosePtr, this, &NotificationManagerUI::deleteNotification );
	ui->notifications->layout()->addWidget( notif );

	//ui->scrollArea->setMinimumHeight( notificationWidgetHeight );
	//ui->scrollArea->setFixedSize(ui->scrollArea->width(), notificationWidgetHeight);	
	//ui->scrollArea->resize(ui->scrollArea->width(), notificationWidgetHeight);
	//ui->notifications->resize(ui->scrollArea->width(), notificationWidgetHeight);
	this->resize( this->width(),  notificationWidgetHeight);

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
	notificationWidgetHeight -= ptr->height();
	spdlog::info("TOTAL height{}", notificationWidgetHeight);
	//ui->scrollArea->setMinimumHeight( notificationWidgetHeight );
	
	//ui->scrollArea->resize(ui->scrollArea->width(), notificationWidgetHeight);
	//ui->notifications->resize(ui->scrollArea->width(), notificationWidgetHeight);
	if(ui->notifications->layout()->count() <= 2){
		this->resize( this->minimumSize() );
	}
	else{
		this->resize( this->width(), notificationWidgetHeight);
	}

	ptr->setParent( nullptr );
	ptr->close();
	ptr->deleteLater();

	ui->label->setText( QString( "%1 notifications" ).arg( --active_notifications ) );
}

//Buttons
void NotificationManagerUI::on_btnClose_pressed(){

}
void NotificationManagerUI::on_btnHideShow_pressed(){
	//Set to inverse
	this->setVisible( !this->isVisible() );
}
