//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TaskPopup.h" resolved

#include "NotificationPopup.hpp"

#include <moc_NotificationPopup.cpp>

#include <QScrollBar>

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

void NotificationPopup::showEvent( QShowEvent* event )
{
	QDialog::showEvent( event );
	adjustSize();
}

QSize NotificationPopup::sizeHint() const
{
	//Get size hint of bottom row
	const auto bottom_row { ui->controlRow->sizeHint() };
	const auto scroll_area { ui->scrollAreaWidgetContents->sizeHint() };

	return { bottom_row.width(), bottom_row.height() + scroll_area.height() + 15 };
}

void NotificationPopup::on_btnHideShow_toggled( bool checked )
{
	ZoneScoped;

	ui->scrollArea->setVisible( checked );
	adjustSize();
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

	const auto count { ui->scrollAreaWidgetContents->children().size() - 1 };
	if ( count > 0 )
	{
		ui->btnHideShow->setEnabled( true );
	}
	ui->label->setText( QString( "%1 messages" ).arg( count ) );
	adjustSize();
}

void NotificationPopup::removeMessage( QWidget* message )
{
	delete message;
	const auto count { ui->scrollAreaWidgetContents->children().size() - 1 };
	if ( count == 0 )
	{
		ui->btnHideShow->setEnabled( false );
		ui->btnHideShow->setChecked( false );
		ui->scrollArea->setVisible( false );
	}
	ui->label->setText( QString( "%1 messages" ).arg( count ) );
	adjustSize();
}

void NotificationPopup::expand()
{
	triggerShow();
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
