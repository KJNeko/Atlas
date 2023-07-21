//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevNotification.h" resolved

#include "DevNotification.hpp"

#include "ui_DevNotification.h"

DevNotification::DevNotification( QWidget* parent ) : Notification( parent ), ui( new Ui::DevNotification )
{
	ui->setupUi( this );
}

DevNotification::~DevNotification()
{
	delete ui;
}
