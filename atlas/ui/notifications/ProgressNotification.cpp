//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressNotification.h" resolved

#include "ProgressNotification.hpp"

#include <moc_ProgressNotification.cpp>

#include <tracy/Tracy.hpp>

#include "core/notifications/notifications.hpp"
#include "core/utils/mainThread/mainThread.hpp"
#include "ui_ProgressNotification.h"

ProgressNotification::ProgressNotification( QWidget* parent ) :
  Notification( parent ),
  ui( new Ui::ProgressNotification )
{
	ui->setupUi( this );
}

ProgressNotification::~ProgressNotification()
{
	delete ui;
}

void ProgressSignaler::hookSignaler( ProgressNotification* notif )
{
	Ui::ProgressNotification* ui { notif->ui };

	connect( this, &ProgressSignaler::progressChanged, ui->progressBar, &QProgressBar::setValue );
	connect( this, &ProgressSignaler::maxChanged, ui->progressBar, &QProgressBar::setMaximum );
	connect( this, &ProgressSignaler::messageChanged, ui->message, &QLabel::setText );
	connect( this, &ProgressSignaler::subMessageChanged, ui->subMessage, &QLabel::setText );
	connect( this, &ProgressSignaler::selfClose, notif, &Notification::selfCloseTrigger );
}

void ProgressSignaler::setMax( int i )
{
	max = static_cast< decltype( max ) >( i );
	emit maxChanged( i );
}

void ProgressSignaler::setSubMessage( const QString str )
{
	emit subMessageChanged( str );
}

void ProgressSignaler::setMessage( const QString str )
{
	emit messageChanged( str );
}

ProgressSignaler::ProgressSignaler()
{
	ZoneScoped;
	utils::executeOnMain(
		[ this ]() -> int
		{
			auto* ptr { new ProgressNotification() };
			hookSignaler( ptr );
			atlas::notifications::handle().addNotification( ptr );
			return 0;
		} );
}

ProgressSignaler::ProgressSignaler( QString str ) : ProgressSignaler()
{
	this->setMessage( std::move( str ) );
}

ProgressSignaler::~ProgressSignaler()
{
	emit selfClose();
}
