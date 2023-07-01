#include "aboutqtdialog.h"

#include <moc_aboutqtdialog.cpp>

#include "ui_aboutqtdialog.h"

AboutQtDialog::AboutQtDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::AboutQtDialog )
{
	ui->setupUi( this );
}

AboutQtDialog::~AboutQtDialog()
{
	delete ui;
}
