//
// Created by kj16609 on 1/17/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameImportDialog.h" resolved

#include "GameImportDialog.hpp"
#include "ui_GameImportDialog.h"
#include <filesystem>
#include "h95/config.hpp"

GameImportDialog::GameImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::GameImportDialog )
{
	ui->setupUi( this );
}

GameImportDialog::~GameImportDialog()
{
	delete ui;
}


void GameImportDialog::on_cancelButton_pressed()
{
	this->close();
}

void GameImportDialog::on_importButton_pressed()
{
	// Check filepath
	if ( !std::filesystem::exists( ui->execPath->text().toStdString() )
		 || !std::filesystem::exists( ui->folderPath->text().toStdString() ) )
		return;

	//Check that title, creator and version are not empty
	if ( ui->title->text().isEmpty() || ui->creator->text().isEmpty() || ui->version->text().isEmpty() )
		return;

	if(ui->move_dest->isChecked())
	{
		auto path_str {ui->dest->text()};
		if(path_str.contains("{game_data}"))
			path_str.replace("{game_data}", getSettings<QString>("paths/game_data", "./data/"));

		if(path_str.contains("{creator}"))
			path_str.replace("{creator}", ui->creator->text());

		if(path_str.contains("{title}"))
			path_str.replace("{title}", ui->title->text());

		if(path_str.contains("{version}"))
			path_str.replace("{version}", ui->version->text());

		const std::filesystem::path path {path_str.toStdString()};

		std::filesystem::create_directories(path.parent_path());
		std::filesystem::copy(ui->execPath->text().toStdString(), path_str.toStdString());
		std::filesystem::remove(ui->execPath->text().toStdString());
		ui->execPath->setText(path_str);
	}


}

void GameImportDialog::on_selectPath_pressed() {}

void GameImportDialog::on_selectExec_pressed() {}
