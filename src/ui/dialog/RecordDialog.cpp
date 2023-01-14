//
// Created by kj16609 on 1/13/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordDialog.h" resolved

#include "RecordDialog.hpp"
#include "ui_RecordDialog.h"


RecordDialog::RecordDialog( QWidget* parent ) : QWidget( parent ), ui( new Ui::RecordDialog )
{
	ui->setupUi( this );
}

RecordDialog::~RecordDialog()
{
	delete ui;
}
