#include "BatchImportDialog.hpp"

#include <QFileDialog>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrentRun>

#include <tracy/Tracy.hpp>

#include "ProgressBarDialog.hpp"
#include "h95/config.hpp"
#include "h95/regex.hpp"
#include "h95/utils.hpp"
#include "ui/delegates/BatchImportDelegate.hpp"
#include "ui/models/BatchImportModel.hpp"
#include "ui_BatchImportDialog.h"

BatchImportDialog::BatchImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::BatchImportDialog )
{
	ui->setupUi( this );
	ui->progressBar->hide();

	ui->twGames->setModel( new BatchImportModel() );
	ui->twGames->setItemDelegate( new BatchImportDelegate() );
	ui->twGames->setEditTriggers( QAbstractItemView::AllEditTriggers );

	processor.moveToThread( &processing_thread );
	preprocessor.moveToThread( &processing_thread );
	processing_thread.start();

	connect( this, &BatchImportDialog::startProcessingDirectory, &preprocessor, &ImportPreProcessor::processDirectory );
	connect(
		&preprocessor, &ImportPreProcessor::finishedDirectory, this, &BatchImportDialog::processFinishedDirectory );
	connect( &preprocessor, &ImportPreProcessor::finishedProcessing, this, &BatchImportDialog::finishedProcessing );

	connect(
		this,
		&BatchImportDialog::addToModel,
		dynamic_cast< BatchImportModel* >( ui->twGames->model() ),
		&BatchImportModel::addGame );

	connect( this, &BatchImportDialog::startImportingGames, &processor, &ImportProcessor::importGames );
	connect( &processor, &ImportProcessor::importComplete, this, &BatchImportDialog::finishedImporting );

	//Connecting progress bar
	connect( &processor, &ImportProcessor::updateText, &progress, &ProgressBarDialog::setText );
	connect( &processor, &ImportProcessor::updateSubText, &progress, &ProgressBarDialog::setSubText );
	connect( &processor, &ImportProcessor::updateMax, &progress, &ProgressBarDialog::setMax );
	connect( &processor, &ImportProcessor::updateSubMax, &progress, &ProgressBarDialog::setSubMax );
	connect( &processor, &ImportProcessor::updateValue, &progress, &ProgressBarDialog::setValue );
	connect( &processor, &ImportProcessor::updateSubValue, &progress, &ProgressBarDialog::setSubValue );
	connect( &processor, &ImportProcessor::startProgressBar, &progress, &ProgressBarDialog::show );
	connect( &processor, &ImportProcessor::closeProgressBar, &progress, &ProgressBarDialog::hide );

	progress.showSubProgress( true );

	if ( config::geometry::batch_import_dialog::hasValue() )
		restoreGeometry( config::geometry::batch_import_dialog::get() );

	loadConfig();
}

void BatchImportDialog::loadConfig()
{
	ui->tbFormat->setText( config::importer::pathparse::get() );

	ui->cbCheckLocal->setChecked( config::importer::searchGameInfo::get() );
	ui->cbSkipFilesize->setChecked( config::importer::skipFilesize::get() );
	ui->cbDownloadBanners->setChecked( config::importer::downloadBanner::get() );
	ui->cbDownloadVNDB->setChecked( config::importer::downloadVNDB::get() );
	ui->cbMoveImported->setChecked( config::importer::moveImported::get() );
}

void BatchImportDialog::saveConfig()
{
	config::importer::pathparse::set( ui->tbFormat->text() );

	config::importer::searchGameInfo::set( ui->cbCheckLocal->isChecked() );
	config::importer::skipFilesize::set( ui->cbSkipFilesize->isChecked() );
	config::importer::downloadBanner::set( ui->cbDownloadBanners->isChecked() );
	config::importer::downloadVNDB::set( ui->cbDownloadVNDB->isChecked() );
	config::importer::moveImported::set( ui->cbMoveImported->isChecked() );
}

BatchImportDialog::~BatchImportDialog()
{
	saveConfig();
	config::geometry::batch_import_dialog::set( saveGeometry() );

	processing_thread.quit();
	processing_thread.wait();

	delete ui;
}

void BatchImportDialog::on_btnSetFolder_pressed()
{
	auto directory { QFileDialog::getExistingDirectory( this, "Select folder to add" ) };

	if ( directory.isEmpty() || !QFile::exists( directory ) )
		return;
	else
		ui->tbPath->setText( std::move( directory ) );
}

void BatchImportDialog::processFiles()
{
	ZoneScoped;
	ui->btnNext->setEnabled( false );
	ui->btnNext->setText( "Import Games" );
	/*
	 * base: /run/media/kj16609/HDDWIN/NSFWSorted/
	 * regex: {path}/{creator}/{title}/{version}
	 *
	 */

	auto stripEndSlash = []( const std::filesystem::path path ) -> std::filesystem::path
	{
		ZoneScoped;
		if ( const auto str = path.string(); str.ends_with( '/' ) || str.ends_with( '\\' ) )
			return { str.substr( 0, str.size() - 1 ) };
		else
			return path;
	};

	const std::filesystem::path base {
		stripEndSlash( std::filesystem::path( ui->tbPath->text().toStdString() ).make_preferred() )
	};

	const std::filesystem::path search {
		stripEndSlash( std::filesystem::path( ui->tbFormat->text().toStdString() ).make_preferred() )
	};

	const QString cleaned_regex { regexify( escapeStr( QString::fromStdString( ( base / search ).string() ) ) ) };

	spdlog::info( "Scanning {} for games", base );

	emit startProcessingDirectory(
		cleaned_regex, base, ui->cbMoveImported->isChecked(), ui->cbSkipFilesize->isChecked() );

	ui->twGames->resizeColumnsToContents();
}

void BatchImportDialog::importFiles()
{
	const auto& games { dynamic_cast< BatchImportModel* >( ui->twGames->model() )->getData() };

	emit startImportingGames( games, ui->tbPath->text().toStdString() );
}

void BatchImportDialog::on_btnNext_pressed()
{
	if ( ui->btnNext->text() == "Import Games" )
	{
		importFiles();
	}
	else
	{
		//Verify that the path is set
		const auto& path { ui->tbPath->text() };
		if ( path.isEmpty() || !QFile::exists( path ) )
		{
			ui->statusLabel->setText( "Path not set or invalid. Please check" );
			return;
		}

		if ( ui->tbFormat->text().isEmpty() )
		{
			ui->statusLabel->setText( "Autofill not set. Please fill out the required entries" );
			return;
		}

		if ( !ui->tbFormat->text().contains( "{title}" ) )
		{
			ui->statusLabel->setText( "Autofill missing \"{title}\" which is required" );
			return;
		}

		ui->swImportGames->setCurrentIndex( 1 );
		ui->btnBack->setEnabled( true );
		ui->btnNext->setEnabled( false );

		processFiles();
	}
}

void BatchImportDialog::on_btnBack_pressed()
{
	//Clear the model
	dynamic_cast< BatchImportModel* >( ui->twGames->model() )->clearData();
}

void BatchImportDialog::modelChanged(
	[[maybe_unused]] const QModelIndex& topLeft,
	[[maybe_unused]] const QModelIndex& bottomRight,
	[[maybe_unused]] const QList< int >& roles )
{
	ui->twGames->resizeColumnsToContents();
}

void BatchImportDialog::processFinishedDirectory( const GameImportData game_data )
{
	emit addToModel( game_data );
	ui->twGames->resizeColumnsToContents();
	ui->statusLabel->setText( QString( "Processed %1 games" ).arg( ui->twGames->model()->rowCount() ) );
}

void BatchImportDialog::finishedProcessing()
{
	ui->statusLabel->setText( "Finished processing all games" );
	ui->btnNext->setEnabled( true );
}

void BatchImportDialog::finishedImporting()
{
	emit importComplete( this->processor.getCompleted() );
	this->close();
}

void BatchImportDialog::on_btnCancel_pressed()
{
	if ( QMessageBox::question( this, "Cancel Import", "Are you sure you want to cancel the import?" )
	     == QMessageBox::Yes )
		this->close();
}
