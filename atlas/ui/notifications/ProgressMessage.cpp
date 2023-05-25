//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressMessage.h" resolved

#include "ProgressMessage.hpp"

#include "NotificationPopup.hpp"
#include "ui_ProgressMessage.h"

ProgressMessage::ProgressMessage( const QString name, QWidget* parent ) :
  QWidget( parent ),
  ui( new Ui::ProgressMessage )
{
	ui->setupUi( this );

	ui->lblPrimary->setText( name );
}

ProgressMessage::~ProgressMessage()
{
	delete ui;
}

std::unique_ptr< ProgressMessageSignaler > ProgressMessage::getSignaler()
{
	std::unique_ptr< ProgressMessageSignaler > signaler { std::make_unique< ProgressMessageSignaler >() };

	connect( signaler.get(), &ProgressMessageSignaler::maxChanged, this, &ProgressMessage::changeMax );
	connect( signaler.get(), &ProgressMessageSignaler::progressChanged, this, &ProgressMessage::changeProgress );
	connect( signaler.get(), &ProgressMessageSignaler::messageChanged, this, &ProgressMessage::changeMessage );
	connect( signaler.get(), &ProgressMessageSignaler::selfClose, this, &ProgressMessage::closeSelf );

	return signaler;
}

void ProgressMessage::changeMax( int max )
{
	ui->progressBar->setMaximum( max );
}

void ProgressMessage::changeProgress( int progress )
{
	ui->progressBar->setValue( progress );

	if ( progress == ui->progressBar->maximum() )
	{
		ui->btnDismiss->setEnabled( true );
		if ( ui->checkBox->isChecked() ) closeSelf();
	}
}

void ProgressMessage::changeMessage( QString message )
{
	ui->lblProgressMsg->setText( message );
}

void ProgressMessage::closeSelf()
{
	if ( ui->progressBar->maximum() == ui->progressBar->value() ) getNotificationPopup()->removeMessage( this );
}

void ProgressMessageSignaler::setMax( int max )
{
	emit maxChanged( max );
}

void ProgressMessageSignaler::setProgress( int progress )
{
	emit progressChanged( progress );
}

void ProgressMessageSignaler::setMessage( QString message )
{
	emit messageChanged( message );
}

void ProgressMessageSignaler::setFinished()
{
	emit selfClose();
}
