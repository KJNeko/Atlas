//
// Created by kj16609 on 3/25/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProgressBarDialog.h" resolved

#include "ProgressBarDialog.hpp"

#include "ui_ProgressBarDialog.h"

ProgressBarDialog::ProgressBarDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::ProgressBarDialog )
{
	ui->setupUi( this );

	showSubProgress( false );
}

ProgressBarDialog::~ProgressBarDialog()
{
	delete ui;
}

void ProgressBarDialog::setMax( const int max )
{
	ui->progressBar->setMaximum( max );
}

void ProgressBarDialog::setValue( const int value )
{
	ui->progressBar->setValue( value );
}

void ProgressBarDialog::setText( const QString text )
{
	ui->progressLabel->setText( text );
}

void ProgressBarDialog::showSubProgress( const bool show )
{
	ui->subProgressBar->setVisible( show );
	ui->subProgressLabel->setVisible( show );
}

void ProgressBarDialog::setSubMax( const int max )
{
	ui->subProgressBar->setMaximum( max );
}

void ProgressBarDialog::setSubValue( const int value )
{
	ui->subProgressBar->setValue( value );
}

void ProgressBarDialog::setSubText( const QString text )
{
	ui->subProgressLabel->setText( text );
}
