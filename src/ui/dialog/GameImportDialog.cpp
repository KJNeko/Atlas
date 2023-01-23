//
// Created by kj16609 on 1/17/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameImportDialog.h" resolved

#include "GameImportDialog.hpp"
#include "ui_GameImportDialog.h"
#include <filesystem>
#include <QFileDialog>
#include <QDirIterator>
#include <QMimeDatabase>
#include <iostream>

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

void GameImportDialog::verifySettings()
{
	ui->pathLabel->setText(
		QString::fromStdString( path_manager.fillPath( ui->dest->text().toStdString() ).string() ) );


	//Check that path and executable exists
	if ( !std::filesystem::exists( ui->folderPath->text().toStdString() ) )
	{
		ui->infoLabel->setText( "Input folder path invalid or does not exist!" );
		return;
	}

	if ( !( std::filesystem::exists( ui->execPath->text().toStdString() ) ) )
	{
		ui->infoLabel->setText( "Executable path is invalid or does not exist!" );
		return;
	}
	else
	{
		const QMimeDatabase mime_db;
		const auto mime_type { mime_db.mimeTypeForFile( ui->execPath->text(), QMimeDatabase::MatchContent ) };
		if ( mime_type.name() != "application/x-ms-dos-executable" )
		{
			ui->infoLabel->setText(
				QString( "File executable not proper type (Wants: application/x-ms-dos-executable vs Actual: %1" )
					.arg( mime_type.name() ) );
		}
	}

	//Check that title, creator and version are filled out
	if ( ui->title->text().isEmpty() || ui->creator->text().isEmpty() || ui->version->text().isEmpty() )
	{
		ui->infoLabel->setText( "One of the required Game Info fields were not populated" );
		return;
	}

	ui->infoLabel->setText( "Good to import!" );
}

void GameImportDialog::on_importButton_pressed()
{
	if ( ui->move_dest->isChecked() )
	{
		const auto path_str { ui->pathLabel->text() };
		const std::filesystem::path path { path_str.toStdString() };

		std::filesystem::create_directories( path.parent_path() );
		std::filesystem::copy(
			ui->folderPath->text().toStdString(),
			path_str.toStdString(),
			std::filesystem::copy_options::recursive );

		if(ui->deleteAfterCopy->isChecked())
			std::filesystem::remove( ui->execPath->text().toStdString() );
	}

	//Seperate preview paths
	const auto list { ui->previewPaths->text().split( ';', Qt::SkipEmptyParts ) };
	std::vector< std::filesystem::path > previews;
	for ( const auto& path : list ) previews.emplace_back( path.toStdString() );

	//Import
	Record::create(
		ui->title->text(),
		ui->creator->text(),
		ui->version->text(),
		ui->engine->text(),
		{ ui->folderPath->text().toStdString(), ui->execPath->text().toStdString() },
		ui->bannerPath->text().toStdString(),
		previews );

	emit importComplete();
	this->close();
}

void GameImportDialog::on_selectPath_pressed()
{
	QFileDialog dialog;
	dialog.setFileMode( QFileDialog::Directory );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	dialog.setLabelText( QFileDialog::LookIn, "Select game folder" );

	QMimeDatabase mime_db;

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };

		if ( list.empty() ) return;

		ui->folderPath->setText( list.first() );

		std::filesystem::path dir { list.first().toStdString() };
		if ( !std::filesystem::exists( dir ) ) return;

		//Locate supporting files
		for ( const auto& temp_dir : std::filesystem::directory_iterator( dir ) )
		{
			const auto mime_type {
				mime_db.mimeTypeForFile( QString::fromStdString( temp_dir.path().string() ) ).name() };

			// Locate executable
			if ( temp_dir.is_regular_file() && temp_dir.path().extension() == ".exe"
				 && mime_type == "application/x-ms-dos-executable" )
			{
				ui->execPath->setText( QString::fromStdString( temp_dir.path().string() ) );
				break;
			}
			//Locate banner
			else if (
				temp_dir.is_regular_file() && ( mime_type == "image/jpeg" || mime_type == "image/png" )
				&& temp_dir.path().filename().string().substr( 0, 7 ) == "banner" )
			{
				ui->bannerPath->setText( QString::fromStdString( temp_dir.path().string() ) );
			}
			//Locate previews
			else if ( temp_dir.is_directory() && temp_dir.path().filename() == "previews" )
			{
				QString previews;

				for ( const auto& file : std::filesystem::directory_iterator( temp_dir.path() ) )
				{
					const auto file_type {
						mime_db.mimeTypeForFile( QString::fromStdString( file.path().string() ) ).name() };

					if ( file.is_regular_file() && ( file_type == "image/jpeg" || file_type == "image/png" ) )
					{
						previews += QString::fromStdString( file.path().string() + ";" );
					}
				}

				if ( previews.endsWith( ':' ) ) previews.chop( 1 );

				ui->previewPaths->setText( previews );
			}
		}
	}
}

void GameImportDialog::on_selectExec_pressed()
{
	QFileDialog dialog{this};
	//TODO: Read more into the spec and try to see if I can find a list of all mime types
	dialog.setMimeTypeFilters( { "application/x-ms-dos-executable" } );
	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	dialog.setLabelText( QFileDialog::LookIn, "Select game executable" );

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };

		if ( list.empty() )
			return;
		else
			ui->execPath->setText( list.first() );
	}
}

const QStringList file_filters {"Image files (*.png, *.jpg *.gif *.tiff)", "Any files (*)"};

void GameImportDialog::on_selectBanner_pressed()
{
	QFileDialog dialog{this};
	dialog.setNameFilters(file_filters);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setOption(QFileDialog::ReadOnly);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setLabelText(QFileDialog::LookIn, "Select banner image");

	if(!dialog.exec())
		return;
	else
	{
		const auto list {dialog.selectedFiles()};

		if(list.empty())
			return;
		else
			ui->bannerPath->setText(list.first());
	}

	verifySettings();
}

void GameImportDialog::on_selectPreviews_pressed()
{
	QFileDialog dialog{this};
	dialog.setNameFilters(file_filters);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setOption(QFileDialog::ReadOnly);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setLabelText(QFileDialog::LookIn, "Select previews");

	if(!dialog.exec())
		return;
	else
	{
		const auto list {dialog.selectedFiles()};

		if(list.empty())
			return;

		QString previews;

		for(const auto& file : list)
			previews += file + ';';

		if(previews.endsWith(';'))
			previews.chop(1);

		ui->previewPaths->setText(previews);
	}

	verifySettings();
}


void GameImportDialog::on_title_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{title}", text );
	verifySettings();
}

void GameImportDialog::on_creator_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{creator}", text );
	verifySettings();
}

void GameImportDialog::on_version_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{version}", text );
	verifySettings();
}

void GameImportDialog::on_engine_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{engine}", text );
	verifySettings();
}

void GameImportDialog::on_folderPath_textChanged( [[maybe_unused]] const QString text )
{
	if ( ui->shouldParsePath->isChecked() )
		path_manager.populateValues( text.toStdString(), ui->pathParse->text().toStdString() );

	ui->title->setText( path_manager.value( { "{title}" } ) );
	ui->creator->setText( path_manager.value( "{creator}" ) );
	ui->version->setText( path_manager.value( "{version}" ) );
	ui->engine->setText( path_manager.value( "{engine}" ) );

	path_manager.setRoot( text.toStdString() );

	verifySettings();
}

void GameImportDialog::on_execPath_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_dest_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}
