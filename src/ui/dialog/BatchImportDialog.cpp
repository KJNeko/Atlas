#include "BatchImportDialog.hpp"

#include <QFileDialog>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrentRun>

#include <tracy/Tracy.hpp>

#include "ProgressBarDialog.hpp"
#include "atlas/config.hpp"
#include "atlas/foldersize.hpp"
#include "atlas/utils/regex/regex.hpp"
#include "ui/delegates/BatchImportDelegate.hpp"
#include "ui/models/BatchImportModel.hpp"
#include "ui_BatchImportDialog.h"

BatchImportDialog::BatchImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::BatchImportDialog )
{
	ZoneScoped;
	ui->setupUi( this );
	ui->progressBar->hide();
	ui->btnBack->setHidden( true );

	ui->twGames->setModel( new BatchImportModel() );
	ui->twGames->setItemDelegate( new BatchImportDelegate() );
	ui->twGames->setEditTriggers( QAbstractItemView::AllEditTriggers );

	processor.moveToThread( &processing_thread );
	preprocessor.moveToThread( &processing_thread );
	processing_thread.start();

	connect( this, &BatchImportDialog::startProcessingDirectory, &preprocessor, &ImportPreProcessor::processDirectory );
	connect(
		&preprocessor, &ImportPreProcessor::finishedDirectory, this, &BatchImportDialog::processFinishedDirectory );
	connect( &preprocessor, &ImportPreProcessor::finishedProcessing, this, &BatchImportDialog::finishedPreProcessing );

	connect(
		this,
		&BatchImportDialog::addToModel,
		dynamic_cast< BatchImportModel* >( ui->twGames->model() ),
		&BatchImportModel::addGames );

	connect(
		this,
		&BatchImportDialog::startImportingGames,
		&processor,
		&ImportProcessor::importGames,
		Qt::QueuedConnection );
	connect(
		&processor,
		&ImportProcessor::importComplete,
		this,
		&BatchImportDialog::finishedImporting,
		Qt::QueuedConnection );
	connect(
		&processor, &ImportProcessor::importFailure, this, &BatchImportDialog::importFailure, Qt::QueuedConnection );

	//Must be issued on the dialog's thread or else it'll never run.
	connect( this, &BatchImportDialog::unpauseImport, &processor, &ImportProcessor::unpause, Qt::DirectConnection );

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
	ZoneScoped;
	ui->tbFormat->setText( config::importer::pathparse::get() );

	ui->cbCheckLocal->setChecked( config::importer::searchGameInfo::get() );
	ui->cbSkipFilesize->setChecked( config::importer::skipFilesize::get() );
	ui->cbDownloadBanners->setChecked( config::importer::downloadBanner::get() );
	ui->cbDownloadVNDB->setChecked( config::importer::downloadVNDB::get() );
	ui->cbMoveImported->setChecked( config::importer::moveImported::get() );
}

void BatchImportDialog::saveConfig()
{
	ZoneScoped;
	config::importer::pathparse::set( ui->tbFormat->text() );

	config::importer::searchGameInfo::set( ui->cbCheckLocal->isChecked() );
	config::importer::skipFilesize::set( ui->cbSkipFilesize->isChecked() );
	config::importer::downloadBanner::set( ui->cbDownloadBanners->isChecked() );
	config::importer::downloadVNDB::set( ui->cbDownloadVNDB->isChecked() );
	config::importer::moveImported::set( ui->cbMoveImported->isChecked() );
}

BatchImportDialog::~BatchImportDialog()
{
	ZoneScoped;
	saveConfig();
	config::geometry::batch_import_dialog::set( saveGeometry() );

	processing_thread.quit();
	processing_thread.wait();

	delete ui;
}

void BatchImportDialog::on_btnSetFolder_pressed()
{
	ZoneScoped;
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
	ui->btnNext->setText( "Import" );
	ui->btnBack->setHidden( false );
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

	spdlog::debug( "Scanning {} for games", base );

	emit startProcessingDirectory( cleaned_regex, base, ui->cbSkipFilesize->isChecked() );

	ui->twGames->resizeColumnsToContents();
}

void BatchImportDialog::importFiles()
{
	ZoneScoped;
	const auto& games { dynamic_cast< BatchImportModel* >( ui->twGames->model() )->getData() };

	if ( ui->cbMoveImported->isChecked() )
		if (
			QMessageBox::question(
				this,
				"Are you sure?",
				"You have \"Move after import\" checked. This will DELETE the game from the source direction after we have copied it. Are you ABSOLUTELY SURE?" )
			== QMessageBox::No )
			return;

	ui->btnBack->setDisabled( true );
	emit startImportingGames( games, ui->tbPath->text().toStdString(), ui->cbMoveImported->isChecked() );

	this->setDisabled( true );
}

void BatchImportDialog::on_btnNext_pressed()
{
	ZoneScoped;
	if ( ui->btnNext->text() == "Import" )
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

		if ( !ui->tbFormat->text().contains( "{version}" ) )
		{
			ui->statusLabel->setText( "Autofill missing \"{version}\" which is required" );
			return;
		}

		ui->swImportGames->setCurrentIndex( 1 );
		ui->btnBack->setEnabled( true );
		ui->btnNext->setDisabled( true );

		processFiles();
	}
}

void BatchImportDialog::on_btnBack_pressed()
{
	ZoneScoped;
	//Clear the model
	dynamic_cast< BatchImportModel* >( ui->twGames->model() )->clearData();

	//Abort the processing thread
	this->processor.abort();
	this->preprocessor.abort();

	ui->btnNext->setText( "Next" );
	ui->swImportGames->setCurrentIndex( 0 );
	ui->btnBack->setHidden( true );
}

void BatchImportDialog::modelChanged(
	[[maybe_unused]] const QModelIndex& topLeft,
	[[maybe_unused]] const QModelIndex& bottomRight,
	[[maybe_unused]] const QList< int >& roles )
{
	ZoneScoped;
	ui->twGames->resizeColumnsToContents();
}

void BatchImportDialog::processFinishedDirectory( const std::vector< GameImportData > game_data )
{
	emit addToModel( game_data );
	ui->twGames->resizeColumnsToContents();
	ui->statusLabel->setText( QString( "Processed %1 games" ).arg( ui->twGames->model()->rowCount() ) );
}

void BatchImportDialog::finishedPreProcessing()
{
	ui->statusLabel->setText( QString( "Finished processing all games (Found %1 games)" )
	                              .arg( ui->twGames->model()->rowCount() ) );
	ui->btnNext->setEnabled( true );
}

void BatchImportDialog::finishedImporting()
{
	emit importComplete( this->processor.getCompleted() );
	accept();
}

void BatchImportDialog::on_btnCancel_pressed()
{
	if ( QMessageBox::question( this, "Cancel Import", "Are you sure you want to cancel the import?" )
	     == QMessageBox::Yes )
	{
		this->processor.abort();
		this->preprocessor.abort();
		this->close();
	}
}

void BatchImportDialog::reject()
{
	if ( QMessageBox::question( this, "Cancel Import", "Are you sure you want to cancel the import?" )
	     == QMessageBox::Yes )
	{
		this->processor.abort();
		this->preprocessor.abort();
	}

	QDialog::reject();
}

void BatchImportDialog::importFailure( const QString top, const QString bottom )
{
	spdlog::warn( "An import failure signal was detected" );
	if ( QMessageBox::warning(
			 this,
			 top,
			 QString( "Something went wrong during the import process. Do you want to continue?\nMsg: %1" )
				 .arg( bottom ),
			 QMessageBox::Yes | QMessageBox::No,
			 QMessageBox::No )
	     == QMessageBox::No )
	{
		processor.abort();
		preprocessor.abort();
		this->setEnabled( true );
		progress.hide();
	}
	else
		emit unpauseImport();
}
