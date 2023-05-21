//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TaskPopup.h" resolved

#include "TaskPopup.hpp"

#include "core/logging.hpp"
#include "ui_TaskPopup.h"

TaskPopup::TaskPopup( QWidget* parent ) : QDialog( parent, Qt::FramelessWindowHint ), ui( new Ui::TaskPopup )
{
	ui->setupUi( this );
}

TaskPopup::~TaskPopup()
{
	delete ui;
}

void TaskPopup::on_btnHideShow_clicked()
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

void TaskPopup::resizeEvent( QResizeEvent* event )
{
	QDialog::resizeEvent( event );
	emit popupResized();
}

void TaskPopup::addMessage( QWidget* message )
{
	ui->listFrame->layout()->addWidget( message );
}

void TaskPopup::removeMessage( QWidget* message )
{
	delete message;
}

void TaskPopup::expand()
{
	ui->btnHideShow->setChecked( true );
}

namespace internal
{
	TaskPopup* taskPopup { nullptr };
}

void initTaskPopup( QWidget* parent )
{
	if ( internal::taskPopup == nullptr ) internal::taskPopup = new TaskPopup( parent );
}

TaskPopup* getTaskPopup()
{
	if ( internal::taskPopup == nullptr ) throw std::runtime_error( "TaskPopup not initialized" );

	return internal::taskPopup;
}
