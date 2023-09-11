//
// Created by kj16609 on 7/21/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevNotification.h" resolved

#include "DevNotification.hpp"

#include <QClipboard>
#include <QGuiApplication>

#include "ui_DevNotification.h"

DevNotification::DevNotification( std::string body, QString info, QWidget* parent ) :
  Notification( parent ),
  ui( new Ui::DevNotification )
{
	ui->setupUi( this );
	ui->errorMessage->setText( QString::fromStdString( body ) );
	ui->errorData->setText( std::move( info ) );
}

DevNotification::~DevNotification()
{
	delete ui;
}

void DevNotification::on_copyError_pressed()
{
	QClipboard* clip { QGuiApplication::clipboard() };
	clip->setText( ui->errorData->text() );
}
