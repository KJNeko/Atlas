//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TaskPopup.h" resolved

#include "NotificationPopup.hpp"

#include "core/logging.hpp"
#include "ui_NotificationPopup.h"

NotificationPopup::NotificationPopup( QWidget* parent ) :
  QDialog( parent, Qt::FramelessWindowHint ),
  ui( new Ui::NotificationPopup )
{
	ui->setupUi( this );
}

NotificationPopup::~NotificationPopup()
{
	delete ui;
}

void NotificationPopup::on_btnHideShow_clicked()
{
	if ( ui->btnHideShow->isChecked() )
	{
		ui->listFrame->show();
		this->resize( QSize( 200, 600 ) );
	}
	else
	{
		ui->listFrame->hide();
		this->window()->resize( QSize( 0, 0 ) );
	}
}

void NotificationPopup::resizeEvent( QResizeEvent* event )
{
	QDialog::resizeEvent( event );
	emit popupResized();
}

void NotificationPopup::addMessage( QWidget* message )
{
	ui->listFrame->layout()->addWidget( message );
}

void NotificationPopup::removeMessage( QWidget* message )
{
	delete message;
}

void NotificationPopup::expand()
{
	ui->btnHideShow->setChecked( true );
}

namespace internal
{
	NotificationPopup* taskPopup { nullptr };
}

void initNotificationPopup( QWidget* parent )
{
	if ( internal::taskPopup == nullptr ) internal::taskPopup = new NotificationPopup( parent );
}

NotificationPopup* getNotificationPopup()
{
	if ( internal::taskPopup == nullptr ) throw std::runtime_error( "NotificationPopup not initialized" );

	return internal::taskPopup;
}
