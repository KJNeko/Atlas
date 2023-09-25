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
  QDialog( parent, Qt::Tool | Qt::FramelessWindowHint ),
  ui( new Ui::NotificationManagerUI )
{
	ui->setupUi( this );
	this->resize( this->minimumSize() );
	//Max height of window is 600;
	this->setMaximumHeight( 550 );
	ui->scrollArea->setMaximumHeight( 550 );
	//spdlog::info( "Max size: {}", NotificationManagerUI::maximumHeight() );
}

NotificationManagerUI::~NotificationManagerUI()
{
	delete ui;
}

std::vector< Notification* > NotificationManagerUI::notifications() const
{
	std::vector< Notification* > ptrs;

	for ( auto* ptr : ui->notifications->children() )
	{
		if ( ptr->objectName() != "verticalLayout" )
		{
			ptrs.emplace_back( dynamic_cast< Notification* >( ptr ) );
		}
	}

	return ptrs;
}

void NotificationManagerUI::addNotification( Notification* notif )
{
	//Get current count of notifications

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
	ui->label->setText( QString( "%1 notifications" ).arg( ++active_notifications ) );
	ui->btnHideNotifications->setVisible(true );
	setHeight();
}

void NotificationManagerUI::setHeight()
{
	int height { 0 };
	int scroll_Area { 0 };
	height += ui->NotificationFrame->height();
	height += NotificationManagerUI::layout()->contentsMargins().bottom() + NotificationManagerUI::layout()->contentsMargins().top();

	if ( ui->scrollArea->isVisible())
	{
		spdlog::info( "Children size: {}", ui->notifications->children().size() );
		//setFixedHeight( ui->NotificationFrame->height() );
		if(ui->notifications->children().size() >1 )
		{
			for ( auto* notif : notifications() )
			{
				scroll_Area += notif->sizeHint().height() + ui->notifications->layout()->spacing();
			}
				scroll_Area += NotificationManagerUI::layout()->contentsMargins().bottom() + NotificationManagerUI::layout()->contentsMargins().top();
			scroll_Area += 9; //This is to fix padding that was added in qss for horizontal bar. Do not remove. Will work with all other QSS
		}
		else{
			height += 2;
		}
		
	}
	ui->scrollArea->setFixedHeight(scroll_Area );
	setFixedHeight( height + scroll_Area );
	emit requestMove();
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

	ui->label->setText( QString( "%1 notifications" ).arg( --active_notifications ) );
	this->setHidden( notifications().size() == 0 );
	setHeight();
}

//Buttons
void NotificationManagerUI::on_btnHideNotifications_pressed()
{
	// Close anything that ***CAN*** be closed.

	auto children { ui->notifications->children() };
	spdlog::info( "Attempting to close {} notifications", children.size() - 1 );
	for ( auto* child : children )
	{
		spdlog::info( "Children: {}", child->objectName() );
		if ( child->objectName() == "MessageNotification" )
		{
			Notification* notif { dynamic_cast< Notification* >( child ) };
			notif->setParent( nullptr );
			notif->close();
			notif->deleteLater();			
		}
	}

	ui->label->setText( QString( "%1 notifications" ).arg( --active_notifications ) );
	ui->scrollArea->setFixedHeight( 0 );
	ui->btnHideNotifications->setVisible( false );
	setHeight();
}

void NotificationManagerUI::on_btnMinimize_pressed()
{
	//Set to inverse
	ui->scrollArea->setVisible( !ui->scrollArea->isVisible() );
	setHeight();
}
