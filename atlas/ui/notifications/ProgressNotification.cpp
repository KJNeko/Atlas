//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressNotification.h" resolved

#include "ProgressNotification.hpp"

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

std::unique_ptr< ProgressSignaler > ProgressNotification::getSignaler()
{
	auto ptr { std::make_unique< ProgressSignaler >() };
	auto* raw { ptr.get() };

	connect( raw, &ProgressSignaler::progressChanged, ui->progressBar, &QProgressBar::setValue );
	connect( raw, &ProgressSignaler::maxChanged, ui->progressBar, &QProgressBar::setMaximum );
	connect( raw, &ProgressSignaler::messageChanged, ui->message, &QLabel::setText );
	connect( raw, &ProgressSignaler::subMessageChanged, ui->subMessage, &QLabel::setText );

	return ptr;
}

void ProgressSignaler::setMax( int i )
{
	emit maxChanged( i );
}

void ProgressSignaler::setProgress( int i )
{
	emit progressChanged( i );
}

void ProgressSignaler::setSubMessage( const QString str )
{
	emit subMessageChanged( str );
}

void ProgressSignaler::setMessage( const QString str )
{
	emit messageChanged( str );
}