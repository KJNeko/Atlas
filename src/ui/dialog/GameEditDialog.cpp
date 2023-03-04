//
// Created by kj16609 on 2/22/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameEditDialog.h" resolved
#include <QMessageBox>

#include "GameEditDialog.hpp"
#include "ui_GameEditDialog.h"
#include "GameImportDialog.hpp"

#include <h95/config.hpp>
#include <h95/database/Record.hpp>
#include <h95/logging.hpp>
#include <QFileDialog>

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

	ui->banner->setEditable(true);
	ui->banner->setBanner( m_record->m_banner );

	ui->previewList->setPreviews( m_record->m_previews );

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
		if ( QMessageBox::question(
			this,
			"Are you sure?",
			"Are you sure you want to abort all changes?" )
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

void GameEditDialog::on_addVersion_pressed()
{
	//Ask user for location
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);

	if(dialog.exec())
	{
		const auto& files {dialog.selectedFiles()};

		if(files.size() == 0)
			return;

		const auto& first {files.at(0)};

		std::filesystem::path path {first.toStdString()};

		if(std::filesystem::exists(path) && std::filesystem::is_directory(path))
		{
			//Find executable
			for(const auto& item : std::filesystem::directory_iterator(path))
			{
				if(item.is_regular_file() && )


			}




		}



	}

}

void GameEditDialog::on_removeVersion_pressed()
{

}
