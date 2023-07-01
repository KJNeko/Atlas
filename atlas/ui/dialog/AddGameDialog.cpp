#include "AddGameDialog.hpp"

#include <moc_AddGameDialog.cpp>

#include "ui_AddGameDialog.h"

AddGameDialog::AddGameDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::AddGameDialog )
{
	ui->setupUi( this );
}

AddGameDialog::~AddGameDialog()
{
	delete ui;
}
