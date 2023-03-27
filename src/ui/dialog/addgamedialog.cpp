#include "addgamedialog.h"

#include "ui_addgamedialog.h"

addGameDialog::addGameDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::addGameDialog )
{
	ui->setupUi( this );
}

addGameDialog::~addGameDialog()
{
	delete ui;
}
