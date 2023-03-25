//
// Created by kj16609 on 3/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "SettingsDialog.hpp"

#include <QDirIterator>
#include <QMessageBox>

#include "h95/config.hpp"
#include "h95/logging.hpp"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::SettingsDialog )
{
	ui->setupUi( this );

	//TODO: Load qss options into ui->themeBox

	//Fill settingsPaths

	//Set 'root' canomical path
	ui->canonicalPath->setText( QString::fromStdString( std::filesystem::canonical( "./" ).string() ) );

	//Set paths
	ui->imagesPath->setText( config::paths::images::getQString() );
	ui->gamePath->setText( config::paths::games::getQString() );
	ui->databasePath->setText( config::paths::database::getQString() );

	QLocale locale { this->locale() };

	//Set filesizes
	const auto image_dir { config::paths::images::get() };
	QDirIterator images_itter( image_dir, QDirIterator::Subdirectories );

	uint64_t total_images_b { 0 };

	while ( images_itter.hasNext() ) total_images_b += static_cast< uint64_t >( images_itter.nextFileInfo().size() );

	ui->imagesSizeLabel->setText( locale.formattedDataSize( static_cast< qint64 >( total_images_b ) ) );

	const auto game_dir { config::paths::games::get() };
	QDirIterator game_itter( game_dir, QDirIterator::Subdirectories );

	uint64_t total_game_b { 0 };

	while ( game_itter.hasNext() ) total_game_b += static_cast< uint64_t >( game_itter.nextFileInfo().size() );

	ui->gamesSizeLabel->setText( locale.formattedDataSize( static_cast< qint64 >( total_game_b ) ) );

	ui->databaseSizeLabel
		->setText( locale
	                   .formattedDataSize( static_cast<
										   qint64 >( std::filesystem::file_size( config::paths::database::get() ) ) ) );

}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::on_settingsList_currentRowChanged( int idx )
{
	ui->stackedWidget->setCurrentIndex( idx );
}

void SettingsDialog::on_applySettings_pressed()
{
	//Handle pathSettings
	if ( ui->gamePath->text() != config::paths::games::getQString() )
	{
		const std::filesystem::path new_path { ui->gamePath->text().toStdString() };

		if (
			QMessageBox::question(
				this,
				"Folder movement required",
				"We need to move the folder in order to set it to it's new location. This could take awhile, Are you sure?" )
			== QMessageBox::No )
			return;

		if ( !std::filesystem::exists( new_path ) )
		{
			if ( QMessageBox::question(
					 this,
					 "Directory does not exist",
					 "The Game directory given does not exist. Should we create it? 'No' will abort." )
			     == QMessageBox::No )
				return;
			else
				std::filesystem::create_directories( new_path );
		}

		//TODO: Make progress bar dialog

		for(const auto& game : std::filesystem::directory_iterator(config::paths::games::get()))
		{
			spdlog::info("copying {} to {}", game.path(), new_path / game.path().filename());
			std::filesystem::copy(
				game,
				new_path / game.path().filename(),
				std::filesystem::copy_options::recursive | std::filesystem::copy_options::copy_symlinks );
		}

		//Delete old game folder
		std::filesystem::remove_all(config::paths::games::get());

		//Set new config option
		config::paths::games::set(new_path);
	}

	if(ui->imagesPath->text() != config::paths::images::getQString())
	{
		const std::filesystem::path new_image_path {ui->imagesPath->text().toStdString()};

		//TODO: Make progress bar for copying images
		for(const auto& image : std::filesystem::directory_iterator(config::paths::images::get()))
			std::filesystem::copy(image, new_image_path / image.path().filename());

		//TODO: Might as well clean any orphans here too.

		//Delete old image folder
		std::filesystem::remove_all(config::paths::images::get());

		config::paths::images::set(new_image_path);
	}

	this->accept();
}

void SettingsDialog::on_cancelSettings_pressed()
{
	this->reject();
}

void SettingsDialog::reject()
{
	if(QMessageBox::question(this, "Are you sure?", "Are you sure you want to exit? Any unapplied changes will be lost") == QMessageBox::Yes)
		QDialog::reject();
}
