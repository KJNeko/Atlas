//
// Created by kj16609 on 2/22/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameEditDialog.h" resolved
#include <QMessageBox>

#include "GameEditDialog.hpp"
#include "ui_GameEditDialog.h"
#include "GameImportDialog.hpp"

// Let's `Record` use the default operator== in order to ensure it's not using the 'id' comparison only
#define ALLOW_RECORD_SLOWCOMPARE

#include <h95/config.hpp>
#include <h95/database/Record.hpp>
#include <h95/logging.hpp>
#include <ui/dialog/GameEditVersionModel.hpp>
#include <ui/dialog/GameEditImageModel.hpp>
#include <QFileDialog>

GameEditDialog::GameEditDialog( const RecordID game_id, QWidget* parent ) :
  QDialog( parent ),
  m_id( game_id ),
  ui( new Ui::GameEditDialog )
{
	ui->setupUi( this );

	this->restoreGeometry( getSettings< QByteArray >( "game_edit_dialog/geometry" ) );

	try
	{
		m_record = Record::select( game_id );

		ui->creatorEdit->setText( m_record->m_creator );
		ui->nameEdit->setText( m_record->m_title );
		ui->engineEdit->setText( m_record->m_engine );

		//Setup image stuff
		ui->imageTable->setModel( new GameEditImageModel( m_record->m_banner, m_record->m_previews ) );
		ui->imageTable->resizeColumnsToContents();

		//Version tab setup
		ui->versionList->setModel( new GameEditVersionModel( m_record->m_versions ) );
		ui->versionList->resizeColumnsToContents();

		connect(
			ui->imageTable->selectionModel(),
			&QItemSelectionModel::selectionChanged,
			this,
			&GameEditDialog::imageSelectionChanged );
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Loading record in editor failed: {}", e.what() );
		QMessageBox::critical( this, "Loading record in editor failed", QString::fromStdString( e.what() ) );
		close();
		return;
	}

	show();
}

GameEditDialog::~GameEditDialog()
{
	setSettings( "game_edit_dialog/geometry", saveGeometry() );

	delete ui;
}

void GameEditDialog::on_nameEdit_textChanged( const QString& text )
{
	m_record->m_title = text;
}

void GameEditDialog::on_creatorEdit_textChanged( const QString& text )
{
	m_record->m_creator = text;
}

void GameEditDialog::imageSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	if ( selected.size() == 0 )
		ui->imageLabel->clear();
	else
	{
		const auto& index { selected.indexes().back() };
		const auto& model { index.model() };
		const auto& row { index.row() };
		const auto& img_path { model->data( model->index( row, 0 ) ).value< QString >() };
		QPixmap pixmap { img_path };

		const auto max_height { ui->imageTable->height() };
		const auto max_width { this->width() / 2 };

		pixmap = pixmap.scaledToHeight( max_height );
		if ( pixmap.width() > max_width ) pixmap = pixmap.scaledToWidth( max_width );

		ui->imageLabel->setPixmap( std::move( pixmap ) );
	}
}


void GameEditDialog::on_replaceBanner_pressed()
{
	QFileDialog dialog;

	if ( dialog.exec() )
	{
		const auto& files { dialog.selectedFiles() };
		if ( files.size() == 0 )
		{
			QMessageBox::warning( this, "No files selected?", "No files selected to replace banner. Aborted" );
			return;
		}
		else
		{
			const auto& file { files.back() };
			this->m_record->m_banner = { file.toStdString() };
		}
	}
}

const QStringList file_filters { "Image file(s) (*.png, *.jpg *.gif *.tiff *.webp)", "Any file(s) (*)" };

void GameEditDialog::on_addPreview_pressed()
{
	QFileDialog dialog;
	dialog.setNameFilters( file_filters );
	dialog.setFileMode( QFileDialog::ExistingFiles );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setLabelText( QFileDialog::LookIn, "Select preview image(s)" );

	if ( dialog.exec() )
	{
		const auto& files { dialog.selectedFiles() };
		if ( files.size() == 0 )
		{
			QMessageBox::warning( this, "No files selected?", "No files selected to add. Aborted" );
			return;
		}
		else
		{
			const auto& model { dynamic_cast< GameEditImageModel* >( ui->imageTable->model() ) };

			for ( const auto& file : files ) { model->addPreview( file.toStdString() ); }
			//this->m_record->m_previews.emplace_back( file.toStdString() ); }
		}
	}
}

void GameEditDialog::on_removePreview_pressed()
{
	const auto& selected { ui->imageTable->selectionModel()->selection().indexes() };

	std::vector< std::size_t > indexes;
	for ( const auto& idx : selected )
	{
		if ( std::find( indexes.begin(), indexes.end(), idx.row() ) == indexes.end() )
			indexes.emplace_back( idx.row() );
	}

	dynamic_cast< GameEditImageModel* >( ui->imageTable->model() )->removeItems( indexes );
}


void GameEditDialog::on_applyChanges_pressed()
{
	if ( m_record.has_value() && m_record.value() != Record::select( m_id ) )
	{
		if ( QMessageBox::question( this, "Are you sure?", "Changes can not be reverted one applied" )
			 == QMessageBox::No )
			return;

		//Check if there is any existing record that is not us but has the same title and creator
		const auto new_record { Record::search( m_record->m_title, m_record->m_creator, m_record->m_engine, m_id ) };

		if ( new_record != 0 )
		{
			QMessageBox::warning( this, "Record already exists", "A record with this information already exists!" );
			return;
		}

		Record::update( m_id, m_record.value() );
	}

	close();
}

void GameEditDialog::on_cancelChanges_pressed()
{
	if ( m_record.has_value() && m_record.value() != Record::select( m_id ) )
	{
		if ( QMessageBox::question( this, "Are you sure?", "Changes will be lost", QMessageBox::Yes | QMessageBox::No )
			 == QMessageBox::Yes )
		{
			this->close();
		}
		else
			return;
	}
	this->close();
}


void GameEditDialog::on_addVersion_pressed()
{
	GameImportDialog import_dialog { this };
	import_dialog.setInformation( m_record->m_creator, m_record->m_title, m_record->m_engine );

	import_dialog.exec();
}

void GameEditDialog::on_removeVersion_pressed()
{
	if ( QMessageBox::question(
			 this,
			 "Are you sure?",
			 "Deleting a version will remove ALL files associated with it",
			 QMessageBox::Yes | QMessageBox::No )
		 == QMessageBox::No )
		return;

	const auto& selected { ui->versionList->selectionModel()->selectedIndexes() };

	if ( selected.size() == 0 ) return;

	const auto index { selected.back() };
	const auto& model { dynamic_cast< GameEditVersionModel* >( ui->versionList->model() ) };

	//Get data from row
	const auto& version_data { m_record->m_versions.at( index.row() ) };

	std::filesystem::remove_all( version_data.m_game_path );

	model->removeItem( static_cast< std::size_t >( index.row() ) );
}
