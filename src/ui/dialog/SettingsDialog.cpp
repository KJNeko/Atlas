//
// Created by kj16609 on 1/23/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"

#include <h95/config.hpp>


SettingsDialog::SettingsDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::SettingsDialog )
{
	ui->setupUi( this );

	ui->itemWidth->setValue( getSettings< int >( "main_view/item_width", 400 ) );
	ui->itemHeight->setValue( getSettings< int >( "main_view/item_height", 200 ) );
	ui->bannerWidth->setValue( getSettings< int >( "main_view/banner_width", 400 ) );
	ui->bannerHeight->setValue( getSettings< int >( "main_view/banner_height", 180 ) );
	ui->itemPadding->setValue( getSettings< int >( "main_view/padding", 4) );
	ui->globalExecCommand->setText( getSettings< QString >( "execution/global_command", "{command}" ) );
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::on_apply_pressed()
{
	setSettings( "main_view/item_width", ui->itemWidth->value() );
	setSettings( "main_view/item_height", ui->itemHeight->value() );
	setSettings( "main_view/banner_width", ui->bannerWidth->value() );
	setSettings( "main_view/banner_height", ui->bannerHeight->value() );
	setSettings( "main_view/padding", ui->itemPadding->value() );
	setSettings( "execution/global_command", ui->globalExecCommand->text() );

	this->close();
}

void SettingsDialog::on_cancel_pressed()
{
	this->close();
}
