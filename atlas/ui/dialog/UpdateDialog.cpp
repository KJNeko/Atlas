#include "UpdateDialog.h"

#include "ui_UpdateDialog.h"

UpdateDialog::UpdateDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::UpdateDialog )
{
	ui->setupUi( this );
	ui->progressBar->setMinimum( 0 );
	ui->progressBar->setMaximum( 100 );
}

UpdateDialog::~UpdateDialog()
{
	delete ui;
}

void UpdateDialog::UpdateProgressBar( int progress )
{
	ui->progressBar->setValue( progress );
}
