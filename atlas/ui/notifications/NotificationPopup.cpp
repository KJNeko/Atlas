//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TaskPopup.h" resolved

#include "NotificationPopup.hpp"

#include <QScrollBar>

#include "core/logging.hpp"
#include "ui_NotificationPopup.h"

NotificationPopup::NotificationPopup( QWidget* parent ) :
  QDialog( parent, Qt::FramelessWindowHint ),
  ui( new Ui::NotificationPopup )
{
	ui->setupUi( this );

	this->layout()->setSizeConstraint( QLayout::SetFixedSize );
}

NotificationPopup::~NotificationPopup()
{
	delete ui;
}

void NotificationPopup::on_btnHideShow_clicked()
{
	ui->scrollArea->setVisible( ui->btnHideShow->isChecked() );
	//ui->listFrame->setVisible( ui->btnHideShow->isChecked());

	this->updateGeometry();
	this->update();
	this->window()->resize( this->minimumSize() );
}

void NotificationPopup::resizeEvent( QResizeEvent* event )
{
	QDialog::resizeEvent( event );
	emit popupResized();
}

void NotificationPopup::addMessage( QWidget* message )
{
	ui->scrollAreaWidgetContents->layout()->addWidget( message );
	ui->scrollArea->scroll( 0, ui->scrollArea->verticalScrollBar()->maximum() );
}

void NotificationPopup::removeMessage( QWidget* message )
{
	delete message;
}

void NotificationPopup::expand()
{
	ui->btnHideShow->setChecked( true );
}

void NotificationPopup::on_btnClearHistory_pressed()
{
	emit clearHistory();
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
