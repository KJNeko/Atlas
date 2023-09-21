//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageNotification.h" resolved

#include "MessageNotification.hpp"

#include <moc_MessageNotification.cpp>

#include <QTimer>

#include "ui_MessageNotification.h"

MessageNotification::
	MessageNotification( const MessageLevel level, QString user_message, QString full_message, QWidget* parent ) :
  Notification( parent ),
  m_full_message( std::move( full_message ) ),
  ui( new Ui::MessageNotification )
{
	ui->setupUi( this );
	ui->label->setText( std::move( user_message ) );

	using enum MessageLevel;

	switch ( level )
	{
		default:
			[[fallthrough]];
		case ATLAS_DEBUG:
			ui->lblType->setText( "Debug" );
			break;
		case ATLAS_INFO_SELFCLOSE:
			{
				//Setup timer to self close this notification after 10 seconds
				QTimer* timer { new QTimer() };
				timer->setSingleShot( true );

				connect( timer, &QTimer::timeout, this, &MessageNotification::selfClose );
				connect( timer, &QTimer::timeout, timer, &QTimer::deleteLater );

				using namespace std::chrono_literals;
				timer->setInterval( 10s );

				timer->start();
			}
			[[fallthrough]];
		case ATLAS_INFO:
			ui->lblType->setText( "Info" );
			break;
		case ATLAS_WARNING:
			ui->lblType->setText( "Warning" );
			break;
		case ATLAS_ERROR:
			ui->lblType->setText( "Error" );
			break;
		case ATLAS_CRITICAL:
			ui->lblType->setText( "Critical" );
			break;
	}
}

MessageNotification::~MessageNotification()
{
	delete ui;
}
