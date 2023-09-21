//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevNotification.h" resolved

#include "DevNotification.hpp"

#include <moc_DevNotification.cpp>

#include <QClipboard>
#include <QGuiApplication>

#include "ui_DevNotification.h"

DevNotification::DevNotification( const MessageLevel level, QString msg, QString full_message, QWidget* parent ) :
  Notification( parent ),
  m_full_message( std::move( full_message ) ),
  ui( new Ui::DevNotification )
{
	ui->setupUi( this );

	ui->errorData->setText( std::move( msg ) );

	switch ( level )
	{
		default:
			[[fallthrough]];
		case MessageLevel::ATLAS_DEBUG:
			[[fallthrough]];
		case MessageLevel::ATLAS_INFO_SELFCLOSE:
			[[fallthrough]];
		case MessageLevel::ATLAS_INFO:
			[[fallthrough]];
		case MessageLevel::ATLAS_WARNING:
			{
				ui->errorType->setText( "Warning" );
				break;
			}
		case MessageLevel::ATLAS_ERROR:
			{
				ui->errorType->setText( "Error" );
				break;
			}
		case MessageLevel::ATLAS_CRITICAL:
			{
				ui->errorType->setText( "Critical" );
				break;
			}
	}
}

DevNotification::~DevNotification()
{
	delete ui;
}

void DevNotification::on_copyError_pressed()
{
	QClipboard* clip { QGuiApplication::clipboard() };
	clip->setText( m_full_message );
}
