//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageNotification.h" resolved

#include "MessageNotification.hpp"

#include <moc_MessageNotification.cpp>

#include "ui_MessageNotification.h"

MessageNotification::MessageNotification( QString str, QWidget* parent ) :
  Notification( parent ),
  ui( new Ui::MessageNotification )
{
	ui->setupUi( this );
	ui->label->setText( std::move( str ) );
}

MessageNotification::~MessageNotification()
{
	delete ui;
}
