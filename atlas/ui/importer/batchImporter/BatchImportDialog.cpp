#include "BatchImportDialog.hpp"

#include <moc_BatchImportDialog.cpp>

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>

#include "BatchImportDelegate.hpp"
#include "BatchImportModel.hpp"
#include "core/config.hpp"
#include "core/import/Importer.hpp"
#include "core/utils/regex/regex.hpp"
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

	connect( &scanner, &GameScanner::scanComplete, this, &BatchImportDialog::finishedPreProcessing );
	connect( &scanner, &GameScanner::foundGame, this, &BatchImportDialog::processFinishedDirectory );
	connect(
		this,
		&BatchImportDialog::addToModel,
		static_cast< BatchImportModel* >( ui->twGames->model() ),
		&BatchImportModel::addGame );

	loadConfig();
}

void BatchImportDialog::loadConfig()
{
	ZoneScoped;
	ui->tbFormat->setText( config::importer::pathparse::get() );

	ui->cbCheckLocal->setChecked( config::importer::searchGameInfo::get() );
	ui->cbMoveImported->setChecked( config::importer::moveImported::get() );
}

void BatchImportDialog::saveConfig()
{
	ZoneScoped;
	config::importer::pathparse::set( ui->tbFormat->text() );

	config::importer::searchGameInfo::set( ui->cbCheckLocal->isChecked() );
	config::importer::moveImported::set( ui->cbMoveImported->isChecked() );
}

BatchImportDialog::~BatchImportDialog()
{
	saveConfig();
	config::geometry::batch_import_dialog::set( saveGeometry() );

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

	const QString cleaned_regex {
		regex::regexify( regex::escapeStr( QString::fromStdString( ( base / search ).string() ) ) )
	};

	spdlog::debug( "Scanning {} for games", base );

	scanner.start( base, cleaned_regex );

	ui->twGames->resizeColumnsToContents();
}

void BatchImportDialog::importFiles()
{
	ZoneScoped;
	const auto games { dynamic_cast< BatchImportModel* >( ui->twGames->model() )->getData() };

	if ( ui->cbMoveImported->isChecked() )
		if (
			QMessageBox::question(
				this,
				"Are you sure?",
				"You have \"Move after import\" checked. This will DELETE the game from the source direction after we have copied it. Are you ABSOLUTELY SURE?" )
			== QMessageBox::No )
			return;

	ui->btnBack->setDisabled( true );

	const bool owning { ui->cbMoveImported->isChecked() };
	const std::filesystem::path root { ui->tbPath->text().toStdString() };

	(void)QtConcurrent::run(
		[ games, owning, root ]()
		{
			QThreadPool import_pool;
			import_pool.setMaxThreadCount( 4 );

			for ( auto game : games )
			{
				spdlog::debug( "Triggering import game for {}", game.title );
				(void)importGame( std::move( game ), root, owning, import_pool );
			}

			import_pool.waitForDone();
		} );

	spdlog::debug( "Finished queueing imports" );

	accept();
}

void BatchImportDialog::on_btnNext_pressed()
{
	ZoneScoped;

	if ( import_triggered ) return;

	spdlog::debug( "next pressed" );
	if ( ui->btnNext->text() == "Import" )
	{
		import_triggered = true;
		importFiles();
	}
	else
	{
		if ( search_started ) return;

		search_started = true;
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
	spdlog::debug( "Back pressed" );
	//Clear the model
	dynamic_cast< BatchImportModel* >( ui->twGames->model() )->clearData();

	scanner.abort();
	search_started = false;

	ui->btnNext->setText( "Next" );
	ui->btnNext->setEnabled( true );
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

void BatchImportDialog::processFinishedDirectory( const GameImportData game_data )
{
	ZoneScoped;
	emit addToModel( game_data );
	ui->twGames->resizeColumnsToContents();
	ui->statusLabel->setText( QString( "Processed %1 games" ).arg( ui->twGames->model()->rowCount() ) );
}

void BatchImportDialog::finishedPreProcessing()
{
	ZoneScoped;
	ui->statusLabel->setText( QString( "Finished processing all games (Found %1 games)" )
	                              .arg( ui->twGames->model()->rowCount() ) );
	ui->btnNext->setEnabled( true );
}

void BatchImportDialog::on_btnCancel_pressed()
{
	reject();
}

void BatchImportDialog::reject()
{
	ZoneScoped;
	if ( QMessageBox::question( this, "Cancel Import", "Are you sure you want to cancel the import?" )
	     == QMessageBox::Yes )
	{
		scanner.abort();
	}

	QDialog::reject();
}

void BatchImportDialog::importFailure( const QString top, const QString bottom )
{
	ZoneScoped;
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
		scanner.abort();
		this->setEnabled( true );
	}
	else
		emit unpauseImport();
}
