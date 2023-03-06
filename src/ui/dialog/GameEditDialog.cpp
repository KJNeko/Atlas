//
// Created by kj16609 on 2/22/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameEditDialog.h" resolved
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include "ui_GameEditDialog.h"
#include "GameEditDialog.hpp"
#include "h95/imageManager.hpp"

#include <h95/config.hpp>
#include <h95/database/Record.hpp>
#include <h95/logging.hpp>
#include <QMimeDatabase>

GameEditDialog::GameEditDialog( const RecordID game_id, QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::GameEditDialog )
{
	ui->setupUi( this );

	this->restoreGeometry( getSettings< QByteArray >( "game_edit_dialog/geometry" ) );

	{
		Transaction transaction;
		m_record = Record::select( game_id, transaction );
		transaction.commit();
	}

	ui->creatorEdit->setLabel( "Creator" );
	ui->creatorEdit->setText( m_record->m_creator );

	ui->titleEdit->setLabel( "Title" );
	ui->titleEdit->setText( m_record->m_title );

	ui->engineEdit->setLabel( "Engine" );
	ui->engineEdit->setText( m_record->m_engine );

	ui->banner->setEditable( true );
	ui->banner->setBanner( m_record->m_banner );

	ui->previewList->setPreviews( m_record->m_previews );
	//ui->previewList->setEditable(true);

	ui->versionList->setVersions( m_record->m_versions );

	show();
}

GameEditDialog::~GameEditDialog()
{
	setSettings( "game_edit_dialog/geometry", saveGeometry() );

	delete ui;
}

void GameEditDialog::on_applyChanges_pressed()
{
	Record modified_record {
		m_record.value().m_id,
		ui->titleEdit->text(),
		ui->creatorEdit->text(),
		ui->engineEdit->text(),
		ui->versionList->versions(),
		ui->banner->path(),
		ui->previewList->previews() };

	if ( modified_record == m_record )
		close();
	else
	{
		//Prompt user here before making modifications
		if ( QMessageBox::question(
				 this,
				 "Are you sure?",
				 "Are you sure you would like to make changes to this record? This can not be reverted" )
			 == QMessageBox::Yes )
		{
			const auto image_path {imageManager::getImagePath()};
			const auto& image_path_str {image_path.string()};

			//Handle banner.
			if(!modified_record.m_banner.string().starts_with( image_path_str ))
				modified_record.m_banner = imageManager::importImage(modified_record.m_banner);

			//Handle previews
			for(auto& preview : modified_record.m_previews)
			{
				if(!preview.string().starts_with( image_path_str ))
					preview = imageManager::importImage(preview);
			}

			const auto& game_path {gamePath()};
			const auto& game_path_str {game_path.string()};

			//Handle copying versions over.
			for(const auto& version : modified_record.m_versions)
			{
				//Check if version is where it should be
				if(!version.m_game_path.string().starts_with(game_path_str))
				{
					//Game must be moved.




				}
			}

			Transaction transaction;

			Record::update( m_record->m_id, modified_record, transaction );

			transaction.commit();
		}
	}
}

void GameEditDialog::on_cancelChanges_pressed()
{
	Record modified_record {
		m_record.value().m_id,
		ui->titleEdit->text(),
		ui->creatorEdit->text(),
		ui->engineEdit->text(),
		ui->versionList->versions(),
		ui->banner->path(),
		ui->previewList->previews() };

	if ( modified_record == m_record )
		close();
	else
	{
		//Prompt user here before making modifications
		if ( QMessageBox::question( this, "Are you sure?", "Are you sure you want to abort all changes?" )
			 == QMessageBox::Yes )
		{
			close();
		}
	}
}

void GameEditDialog::resizeEvent( QResizeEvent* event )
{
	QDialog::resizeEvent( event );
}

inline static std::vector< std::string > common_names { "Game.exe" };
inline static std::vector< std::string > blacklist { "UnityCrashHandler64.exe", "UnityCrashHandler.exe" };

void GameEditDialog::on_addVersion_pressed()
{
	//Ask user for location
	QFileDialog dialog;
	dialog.setFileMode( QFileDialog::Directory );

	if ( dialog.exec() )
	{
		const auto& files { dialog.selectedFiles() };

		if ( files.size() == 0 ) return;

		const auto& first { files.at( 0 ) };

		std::filesystem::path path { first.toStdString() };

		if ( std::filesystem::exists( path ) && std::filesystem::is_directory( path ) )
		{
			//Determine executable
			std::filesystem::path executable;

			std::vector< std::filesystem::path > suspected_paths;

			QMimeDatabase mime_db;

			for ( const auto& dir_entry : std::filesystem::directory_iterator( path ) )
			{
				if ( dir_entry.is_regular_file() )
				{
					const auto& item_path { dir_entry.path() };

					//Get mime information.
					//Should make filter this down more via pre-check through just looking for `.exe`. Using the `MatchContent`
					// will make it read at least a few bytes from every file. Might be slower
					const QMimeType mime { mime_db.mimeTypeForFile(
						QString::fromStdString( item_path.string() ),
						QMimeDatabase::MatchContent ) };

					//TODO: Find if linux has any specific mime type to look for
					if ( mime.inherits( "application/x-ms-dos-executable" ) )
					{
						//Apparently .dll files also fall under this? Filter to remove then
						if ( item_path.extension() == "dll" ) continue;

						if ( std::find( blacklist.begin(), blacklist.end(), item_path.filename() ) == blacklist.end() )
						{
							spdlog::debug( "Filepath {} found as executable.", item_path.string() );
							suspected_paths.emplace_back( item_path );
						}
					}
				}
			}

			spdlog::debug( "{} paths suspected as executable", suspected_paths.size() );

			if ( suspected_paths.size() > 1 )
			{
				//Try to see if any of our suspected paths are in the common_names and select that one.
				for ( const auto& suspected_path : suspected_paths )
				{
					spdlog::debug( "Trying {}", suspected_path.string() );
					if ( std::find( common_names.begin(), common_names.end(), suspected_path.filename() )
						 != common_names.end() )
					{
						//We found it in the list. It's probably what we want
						executable = std::filesystem::relative( suspected_path, path );
						break;
					}
					spdlog::debug( "Filepath {} not in common name list", suspected_path.string() );
				}
			}

			QMessageBox::information(
				nullptr,
				"Executable?",
				"Failed to auto-determine the executable for the game. Please select it manually" );

			//Prompt user to pick
			QFileDialog executable_dialog;
			executable_dialog.setMimeTypeFilters( { "application/x-ms-dos-executable" } );
			executable_dialog.setDirectory( path );
			if ( executable_dialog.exec() )
			{
				const auto selected { executable_dialog.selectedFiles() };

				if ( selected.size() > 0 )
					executable = std::filesystem::relative( selected.at( 0 ).toStdString(), path );
			}

			if ( executable.empty() || !std::filesystem::exists( path / executable ) )
			{
				QMessageBox::information(
					this,
					"Add version failed",
					"Failed to add version. Was unable to determine executable. Aborting." );
				return;
			}

			//Get user string from user
			auto version_str { QInputDialog::getText( this, "Enter version string", "Version" ) };

			//Add to the model
			ui->versionList->addVersion( { std::move( version_str ), path, executable } );
		}
	}
}

void GameEditDialog::on_removeVersion_pressed() {}
