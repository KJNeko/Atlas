//
// Created by kj16609 on 4/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordEditor.h" resolved

#include "RecordEditor.hpp"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMimeData>

#include "ui/dialog/ProgressBarDialog.hpp"
#include "ui/models/FilepathModel.hpp"
#include "ui/views/VersionView.hpp"
#include "ui_RecordEditor.h"

RecordEditor::RecordEditor( const RecordID record, QWidget* parent, Transaction transaction ) :
  QDialog( parent ),
  m_record( record, transaction ),
  m_banner_path( m_record->getBannerPath( transaction ) ),
  m_preview_paths( m_record->getPreviewPaths( transaction ) ),
  m_versions( m_record->getVersions( transaction ) ),
  ui( new Ui::RecordEditor )
{
	ui->setupUi( this );
}

void RecordEditor::loadRecordInfo()
{
	ui->titleLineEdit->setText( m_record->getTitle() );
	ui->creatorLineEdit->setText( m_record->getCreator() );
	ui->engineLineEdit->setText( m_record->getEngine() );
	//ui->gameText->setText( m_record->getDescription() );
}

void RecordEditor::loadBanners()
{
	ui->bannerPreview
		->setPixmap( m_record->getBanner( ui->bannerPreview->size() - QSize( 25, 40 ), KEEP_ASPECT_RATIO ) );
}

void RecordEditor::loadPreviews()
{
	ui->previewList->setPaths( m_record->getPreviewPaths() );
}

void RecordEditor::loadTags()
{
	const auto user_tags { m_record->getUserTags() };
	QStringList user_tags_list;
	for ( const auto& tag : user_tags ) user_tags_list.emplace_back( tag );

	ui->userTagsList->clear();
	ui->userTagsList->addItems( std::move( user_tags_list ) );

	const auto all_tags { m_record->getAllTags() };
	QStringList all_tags_list;
	for ( const auto& tag : all_tags ) all_tags_list.emplace_back( tag );

	ui->processedTagsList->clear();
	ui->processedTagsList->addItems( std::move( all_tags_list ) );
}

RecordEditor::~RecordEditor()
{
	delete ui;
}

void RecordEditor::resizeEvent( QResizeEvent* event )
{
	loadBanners();

	QDialog::resizeEvent( event );
}

void RecordEditor::on_splitter_splitterMoved( [[maybe_unused]] int pos, [[maybe_unused]] int index )
{
	loadBanners();
}

void RecordEditor::showEvent( QShowEvent* event )
{
	QDialog::showEvent( event );
	loadRecordInfo();
	loadBanners();
	loadPreviews();
	loadTags();
	loadVersions();
}

void RecordEditor::on_btnSetBanner_pressed()
{
	QFileDialog file_dialog { this, "Select Banner" };

	file_dialog.setFileMode( QFileDialog::ExistingFile );
	file_dialog.setNameFilter( "Images (*.png *.jpg *.jpeg *.bmp *.webp)" );
	file_dialog.setViewMode( QFileDialog::Detail );

	if ( file_dialog.exec() )
		m_record->setBanner( std::filesystem::path( file_dialog.selectedFiles().first().toStdString() ) );
}

void RecordEditor::on_btnAddPreviews_pressed()
{
	QFileDialog file_dialog { this, "Select Previews" };

	file_dialog.setFileMode( QFileDialog::ExistingFiles );
	file_dialog.setNameFilter( "Images (*.png *.jpg *.jpeg *.bmp *.webp)" );
	file_dialog.setViewMode( QFileDialog::Detail );

	if ( file_dialog.exec() )
	{
		for ( const auto& path : file_dialog.selectedFiles() ) m_record->addPreview( { path.toStdString() } );
	}
}

void RecordEditor::on_btnRemovePreviews_pressed()
{
	//TODO
}

void RecordEditor::on_btnDeleteVersion_pressed()
{
	if ( ui->versionList->count() == 0 ) return;

	const auto selected_list { ui->versionList->selectedItems() };
	if ( selected_list.size() == 0 ) return;
	const auto selected { selected_list.at( 0 ) };

	const auto version { m_record->getVersion( selected->text() ) };

	if ( !version.has_value() ) return;

	if (
		QMessageBox::question(
			this,
			"Are you sure?",
			QString(
				"Are you absolutely sure you want to delete %1. This can NOT be undone and you will lose ALL files associated with this version!" )
				.arg( version->getVersionName() ) )
		== QMessageBox::Yes )
	{
		m_record->removeVersion( *version );

		for ( const auto& file : std::filesystem::recursive_directory_iterator( version->getPath() ) )
		{
			try
			{
				if ( file.is_regular_file() ) std::filesystem::remove( file );
			}
			catch ( std::filesystem::filesystem_error& e )
			{
				spdlog::error( "Failed to delete {} due to {}", file.path(), e.what() );
			}
		}

		try
		{
			std::filesystem::remove_all( version->getPath() );
		}
		catch ( std::filesystem::filesystem_error& e )
		{
			spdlog::error( "Failed to delete all due to {}", e.what() );
		}

		loadVersions();
	}
	else
		return;
}

void RecordEditor::on_btnAddVersion_pressed()
{}

void RecordEditor::switchTabs( const int index )
{
	ui->tabWidget->setCurrentIndex( index );
}

void RecordEditor::on_btnChangeTitle_pressed()
{
	if ( const auto output =
	         QInputDialog::getText( this, "Change Title", "New Title", QLineEdit::Normal, m_record->getTitle() );
	     output != m_record->getTitle() && !output.isEmpty() )
	{
		Transaction trans { Autocommit };
		std::size_t count { 0 };
		trans << "SELECT COUNT(*) FROM records WHERE title = ? AND creator = ?;" << output.toStdString()
			  << m_record->getCreator( trans ).toStdString() << m_record->getEngine( trans ).toStdString()
			>> count;

		if ( count != 0 )
		{
			QMessageBox::warning( this, "Duplicate!", "A title with that name already exists on the same creator!" );
			return;
		}

		m_record->setTitle( output, trans );
	}
	loadRecordInfo();
}

void RecordEditor::on_btnChangeCreator_pressed()
{
	if ( const auto output =
	         QInputDialog::getText( this, "Change Creator", "New Creator", QLineEdit::Normal, m_record->getCreator() );
	     output != m_record->getCreator() && !output.isEmpty() )
	{
		Transaction trans { Autocommit };
		std::size_t count { 0 };
		trans << "SELECT COUNT(*) FROM records WHERE title = ? AND creator = ? AND engine = ?;" << output.toStdString()
			  << m_record->getCreator( trans ).toStdString() << m_record->getEngine( trans ).toStdString()
			>> count;

		if ( count != 0 )
		{
			QMessageBox::warning(
				this,
				"Duplicate!",
				"The creator already has a game with this name and engine! Merging is not implemented yet" );
			return;
		}

		m_record->setCreator( output, trans );
	}
	loadRecordInfo();
}

void RecordEditor::on_btnChangeEngine_pressed()
{
	if ( const auto output =
	         QInputDialog::getText( this, "Change Engine", "New Engine", QLineEdit::Normal, m_record->getEngine() );
	     output != m_record->getEngine() && !output.isEmpty() )
	{
		Transaction trans { Autocommit };
		std::size_t count { 0 };
		trans << "SELECT COUNT(*) FROM records WHERE title = ? AND creator = ? AND engine = ?;" << output.toStdString()
			  << m_record->getCreator( trans ).toStdString() << m_record->getEngine( trans ).toStdString()
			>> count;

		if ( count != 0 )
		{
			QMessageBox::
				warning( this, "Duplicate!", "There is already a game with the same creator, name and engine!" );
			return;
		}
		m_record->setEngine( output, trans );
	}
	loadRecordInfo();
}

void RecordEditor::on_btnApplyDesc_pressed()
{
	m_record->setDesc( ui->gameText->toPlainText() );
}

void RecordEditor::on_tagEdit_returnPressed()
{
	const auto text { ui->tagEdit->text() };

	//If tag is already added then remove it instead
	if ( const auto items = ui->userTagsList->findItems( text, Qt::MatchExactly ); items.size() > 0 )
		m_record->removeUserTag( text );
	else
		m_record->addUserTag( text );

	ui->tagEdit->clear();

	loadTags();
}

void RecordEditor::on_versionList_currentRowChanged( [[maybe_unused]] int idx )
{
	if ( ui->versionList->count() == 0 || idx < 0 )
	{
		ui->activeVersion->setVersion( std::nullopt );
		return;
	}
	else
	{
		const auto item { ui->versionList->item( idx ) };

		if ( item == nullptr ) return;

		const auto version { m_record->getVersion( item->text() ) };

		ui->activeVersion->setVersion( version );
	}
}

void RecordEditor::loadVersions()
{
	ui->versionList->clear();

	QStringList list;

	for ( const auto& version : m_record->getVersions() ) list.emplace_back( version.getVersionName() );

	ui->versionList->addItems( std::move( list ) );
}

void RecordEditor::on_previewList_reordered()
{
	m_record->reorderPreviews( ui->previewList->model()->getFilepaths() );
}
