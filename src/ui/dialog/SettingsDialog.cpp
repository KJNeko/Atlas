//
// Created by kj16609 on 1/23/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"


SettingsDialog::SettingsDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::SettingsDialog )
{
	ui->setupUi( this );
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}
