//
// Created by kj16609 on 3/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "SettingsDialog.hpp"

#include <QDirIterator>
#include <QMessageBox>

#include "ProgressBarDialog.hpp"
#include "h95/config.hpp"
#include "h95/logging.hpp"
#include "h95/utils.hpp"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::SettingsDialog )
{
	ui->setupUi( this );

	//TODO: Load qss options into ui->themeBox

	prepareThemeSettings();
	preparePathsSettings();
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::prepareThemeSettings()
{
	//Load all valid options.
	for ( auto& qss_option : std::filesystem::directory_iterator( "./data/qss" ) )
	{
		if ( qss_option.is_regular_file() && qss_option.path().extension() == ".qss" )
			ui->themeBox->addItem( QString::fromStdString( qss_option.path().filename().string() ) );
	}

	//Select current option
	for ( int i = 0; i < ui->themeBox->count(); ++i )
	{
		if ( ui->themeBox->itemText( i ).toStdString() == config::paths::theme::get().filename().string() )
		{
			//Found
			ui->themeBox->setCurrentIndex( i );
			break;
		}
	}
}

void SettingsDialog::saveThemeSettings()
{
	config::paths::theme::set( "./data/qss/" + ui->themeBox->currentText() );

	reloadTheme();
}

void SettingsDialog::preparePathsSettings()
{
	//Set 'root' canomical path
	ui->canonicalPath->setText( QString::fromStdString( std::filesystem::canonical( "./" ).string() ) );

	//Set paths
	ui->imagesPath->setText( config::paths::images::getQString() );
	ui->gamePath->setText( config::paths::games::getQString() );
	ui->databasePath->setText( config::paths::database::getQString() );

	QLocale locale { this->locale() };

	//Set filesizes
	ui->imagesSizeLabel
		->setText( locale.formattedDataSize( static_cast< qint64 >( folderSize( config::paths::images::get() ) ) ) );

	ui->gamesSizeLabel
		->setText( locale.formattedDataSize( static_cast< qint64 >( folderSize( config::paths::games::get() ) ) ) );

	ui->databaseSizeLabel
		->setText( locale
	                   .formattedDataSize( static_cast<
										   qint64 >( std::filesystem::file_size( config::paths::database::get() ) ) ) );
}

void SettingsDialog::savePathsSettings()
{
	//Handle pathSettings
	if ( ui->gamePath->text() != config::paths::games::getQString() )
	{
		const std::filesystem::path new_path { ui->gamePath->text().toStdString() };

		if (
			QMessageBox::question(
				this,
				"Folder movement required",
				"We need to move the folder in order to set it to it's new location. This could take awhile, Are you sure? This CAN NOT be aborted once started." )
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

		ProgressBarDialog progress_dialog { this };
		progress_dialog.show();

		std::vector< std::filesystem::path > folders;
		for ( const auto& group : std::filesystem::directory_iterator( config::paths::games::get() ) )
			if ( group.is_directory() ) folders.emplace_back( group.path() );

		progress_dialog.setMax( static_cast< int >( folders.size() ) );
		progress_dialog.showSubProgress( true );

		for ( std::size_t i = 0; i < folders.size(); ++i )
		{
			progress_dialog.setValue( static_cast< int >( i ) );
			progress_dialog
				.setText( QString::fromStdString( "Top level folder: " + folders.at( i ).filename().string() ) );

			std::vector< std::filesystem::path > files;

			const auto games_path { config::paths::games::get() };

			//Find all files that we need to copy.
			for ( const auto& file : std::filesystem::recursive_directory_iterator( folders.at( i ) ) )
				if ( file.is_regular_file() ) files.emplace_back( std::filesystem::relative( file, games_path ) );

			progress_dialog.setSubMax( static_cast< int >( files.size() ) );

			for ( std::size_t fidx = 0; fidx < files.size(); ++fidx )
			{
				progress_dialog.setSubValue( static_cast< int >( fidx ) );

				const auto source_path { games_path / files.at( fidx ) };
				const auto dest_path { new_path / files.at( fidx ) };

				if ( !std::filesystem::exists( dest_path.parent_path() ) )
					std::filesystem::create_directories( dest_path.parent_path() );

				progress_dialog.setSubText( QString::fromStdString( files.at( fidx ).string() ) );
				adjustSize();

				std::filesystem::copy( source_path, dest_path );

				QApplication::processEvents();
			}
		}

		progress_dialog.hide();

		/*
		for ( const auto& game : std::filesystem::directory_iterator( config::paths::games::get() ) )
		{
			spdlog::info( "copying {} to {}", game.path(), new_path / game.path().filename() );
			std::filesystem::copy(
				game,
				new_path / game.path().filename(),
				std::filesystem::copy_options::recursive | std::filesystem::copy_options::copy_symlinks );
		}*/

		//Delete old game folder
		std::filesystem::remove_all( config::paths::games::get() );

		//Set new config option
		config::paths::games::set( new_path );
	}

	if ( ui->imagesPath->text() != config::paths::images::getQString() )
	{
		const std::filesystem::path new_image_path { ui->imagesPath->text().toStdString() };

		//TODO: Make progress bar for copying images
		for ( const auto& image : std::filesystem::directory_iterator( config::paths::images::get() ) )
			std::filesystem::copy( image, new_image_path / image.path().filename() );

		//TODO: Might as well clean any orphans here too.

		//Delete old image folder
		std::filesystem::remove_all( config::paths::images::get() );

		config::paths::images::set( new_image_path );
	}
}

void SettingsDialog::on_settingsList_currentRowChanged( int idx )
{
	ui->stackedWidget->setCurrentIndex( idx );
}

void SettingsDialog::on_applySettings_pressed()
{
	savePathsSettings();
	saveThemeSettings();
	this->accept();
}

void SettingsDialog::on_cancelSettings_pressed()
{
	this->reject();
}

void SettingsDialog::reject()
{
	if ( QMessageBox::
	         question( this, "Are you sure?", "Are you sure you want to exit? Any unapplied changes will be lost" )
	     == QMessageBox::Yes )
	{
		QDialog::reject();
		reloadTheme();
	}
}

void SettingsDialog::on_themeBox_currentTextChanged( const QString& text )
{
	spdlog::info( "Theme changed to {}", text );

	QFile file { "./data/qss/" + text };
	file.open( QFile::ReadOnly );

	QString style { file.readAll() };

	dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( style );

	ensurePolished();
}

void SettingsDialog::reloadTheme()
{
	QFile file { config::paths::theme::getQString() };

	if ( !file.exists() )
	{
		config::paths::theme::setDefault();

		file.open( QFile::ReadOnly );

		QString style { file.readAll() };

		dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( style );

		ensurePolished();

		return;
	}

	file.open( QFile::ReadOnly );

	QString style { file.readAll() };

	dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( style );

	ensurePolished();
}
