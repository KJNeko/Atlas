//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressMessage.h" resolved

#include "ProgressMessage.hpp"

#include "TaskPopup.hpp"
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
}

void ProgressMessage::changeMessage( QString message )
{
	ui->lblProgressMsg->setText( message );
}

void ProgressMessage::closeSelf()
{
	getTaskPopup()->removeMessage( this );
}

namespace atlas::notifications
{
	std::unique_ptr< ProgressMessageSignaler > createProgressNotification( QString name, const bool reveal )
	{
		const auto& task_list { getTaskPopup() };

		auto* message { new ProgressMessage( name ) };

		auto signaler { message->getSignaler() };

		task_list->addMessage( message );

		if ( reveal )
		{
			task_list->show();
			task_list->expand();
		}

		return signaler;
	}
} // namespace atlas::notifications

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
