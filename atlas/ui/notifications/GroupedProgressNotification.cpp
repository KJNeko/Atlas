//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GroupedProgressNotification.h" resolved

#include "GroupedProgressNotification.hpp"

#include "ui_GroupedProgressNotification.h"

GroupedProgressNotification::GroupedProgressNotification( QWidget* parent ) :
  Notification( parent ),
  ui( new Ui::GroupedProgressNotification )
{
	ui->setupUi( this );
}

GroupedProgressNotification::~GroupedProgressNotification()
{
	delete ui;
}
