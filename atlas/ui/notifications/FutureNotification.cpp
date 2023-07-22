//
// Created by kj16609 on 7/22/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FutureNotification.h" resolved

#include "FutureNotification.hpp"

#include "ui_FutureNotification.h"

FutureNotification::FutureNotification( QWidget* parent ) : Notification( parent ), ui( new Ui::FutureNotification )
{
	ui->setupUi( this );
}

FutureNotification::~FutureNotification()
{
	delete ui;
}
