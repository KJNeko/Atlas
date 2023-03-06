//
// Created by kj16609 on 3/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressDialog.h" resolved

#include "ProgressDialog.hpp"
#include "ui_ProgressDialog.h"


ProgressDialog::ProgressDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::ProgressDialog )
{
	ui->setupUi( this );
}

ProgressDialog::~ProgressDialog()
{
	delete ui;
}
