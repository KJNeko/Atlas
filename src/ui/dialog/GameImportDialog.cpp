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

void GameImportDialog::verifySettings()
{
	//Check that path and executable exists
	if ( !std::filesystem::exists( ui->folderPath->text().toStdString() ) )
	{
		ui->infoLabel->setText( "Path could not be verified to exist." );
		return;
	}

	if ( !( std::filesystem::exists( ui->execPath->text().toStdString() ) ) )
	{
		ui->infoLabel->setText( "Executable path could not be verified to exist" );
		return;
	}
	else
	{
		const QMimeDatabase mime_db;
		const auto mime_type { mime_db.mimeTypeForFile( ui->execPath->text(), QMimeDatabase::MatchContent ) };
		if ( mime_type.name() != "application/x-ms-dos-executable" )
		{
			ui->infoLabel->setText( QString( "File executable not proper type (application/x-ms-dos-executable vs %s" )
										.arg( mime_type.name() ) );
		}
	}

	//Check if we should autofill
	if ( ui->shouldParsePath->isChecked() )
	{
		const auto parse_text { ui->pathParse->text() };
		if ( !parse_text.contains( "{path}" )
			 || !(
				 parse_text.contains( "{title}" ) || parse_text.contains( "{creator}" )
				 || parse_text.contains( "{version}" ) || parse_text.contains( "{engine}" ) ) )
		{
			ui->infoLabel->setText(
				"Autofill from path should contain {path} and at least one of `{creator}`, `{title}`, `{version}`, or `{engine}`" );
			return;
		}

		//Parse path
		std::filesystem::path path { ui->folderPath->text().toStdString() };
		std::filesystem::path search_path { parse_text.toStdString() };

		uint16_t step_counter { 0 };

		std::vector< std::pair< std::string, std::string > > parsed_pieces;

		while ( !search_path.empty() && step_counter < 512 )
		{
			const auto key { search_path.filename() };

			if ( key == "{path}" )
			{
				parsed_pieces.emplace_back( key, path.string() );
				break;
			}
			else
				parsed_pieces.emplace_back( key, path.filename() );

			path = path.parent_path();
			search_path = search_path.parent_path();

			++step_counter;
		}

		if ( step_counter == 512 )
		{
			ui->infoLabel->setText(
				"Attempting to parse path exceeded 512 steps! Aborting. Report path and parser text to the dev" );
			return;
		}

		for ( const auto& [key, string] : parsed_pieces )
		{
			if ( key == "{path}" )
				continue;
			else if ( key == "{title}" )
				ui->title->setText( QString::fromStdString( string ) );
			else if ( key == "{creator}" )
				ui->creator->setText( QString::fromStdString( string ) );
			else if ( key == "{version}" )
				ui->version->setText( QString::fromStdString( string ) );
			else if ( key == "{engine}" )
				ui->engine->setText( QString::fromStdString( string ) );
			else
			{
				ui->infoLabel->setText( "Unsupported key in parser. Aborting" );
				return;
			}
		}
	}

	//Check that title, creator and version are filled out
	if ( ui->title->text().isEmpty() || ui->creator->text().isEmpty() || ui->version->text().isEmpty() )
	{
		ui->infoLabel->setText( "One of the required Game Info fields were not populated" );
		return;
	}

	auto path_str { ui->dest->text() };
	if ( path_str.contains( "{h95_data}" ) )
	{
		const std::filesystem::path data_dir { getSettings< QString >( "paths/h95_data", "./data" ).toStdString() };
		const auto filepath { std::filesystem::canonical( data_dir ) };
		path_str.replace( "{h95_data}", QString::fromStdString( filepath.string() ) );
	}


	if ( path_str.contains( "{creator}" ) ) path_str.replace( "{creator}", ui->creator->text() );

	if ( path_str.contains( "{title}" ) ) path_str.replace( "{title}", ui->title->text() );

	if ( path_str.contains( "{version}" ) ) path_str.replace( "{version}", ui->version->text() );

	if ( path_str.contains( "{engine}" ) ) path_str.replace( "{engine}", ui->engine->text() );

	ui->pathLabel->setText( path_str );

	ui->infoLabel->setText( "Good to import!" );
}

void GameImportDialog::on_importButton_pressed()
{
	if ( ui->move_dest->isChecked() )
	{
		const auto path_str { ui->pathLabel->text() };
		const std::filesystem::path path { path_str.toStdString() };

		std::filesystem::create_directories( path.parent_path() );
		std::filesystem::copy( ui->execPath->text().toStdString(), path_str.toStdString() );
		std::filesystem::remove( ui->execPath->text().toStdString() );
		ui->execPath->setText( path_str );
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

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };

		if ( list.empty() ) return;

		ui->folderPath->setText( list.first() );

		const QDir dir { list.first() };
		if ( !dir.exists() ) return;

		QDirIterator dir_itter { dir.path(), QDirIterator::NoIteratorFlags };

		while ( dir_itter.hasNext() )
		{
			const auto file_info { dir_itter.nextFileInfo() };

			if ( file_info.isFile() )
			{
				QMimeDatabase mime_db;
				const auto type { mime_db.mimeTypeForFile( file_info, QMimeDatabase::MatchContent ) };
				if ( type.name() == "application/x-ms-dos-executable" )
				{
					ui->execPath->setText( file_info.filePath() );
					break;
				}
			}
		}
	}
}

void GameImportDialog::on_selectExec_pressed()
{
	QFileDialog dialog;
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


void GameImportDialog::on_title_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_creator_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_version_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_engine_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_folderPath_textChanged( [[maybe_unused]] const QString text )
{
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
