//
// Created by kj16609 on 1/13/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ImportDialog.h" resolved

#include "ImportDialog.hpp"
#include "ui_ImportDialog.h"


ImportDialog::ImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::ImportDialog )
{
	ui->setupUi( this );
}

ImportDialog::~ImportDialog()
{
	delete ui;
}
