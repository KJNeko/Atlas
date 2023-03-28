#include "batchimportdialog.h"

#include <QFileDialog>
#include <QFutureWatcher>
#include <QMimeDatabase>
#include <QtConcurrentRun>

#include <tracy/Tracy.hpp>

#include "h95/config.hpp"
#include "h95/regex.hpp"
#include "h95/utils.hpp"
#include "ui/delegates/BatchImportDelegate.hpp"
#include "ui/models/BatchImportModel.hpp"
#include "ui_batchimportdialog.h"

batchImportDialog::batchImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::batchImportDialog )
{
	ui->setupUi( this );
	ui->progressBar->hide();

	ui->twGames->setModel( new BatchImportModel() );
	ui->twGames->setItemDelegate( new BatchImportDelegate() );

	ui->tbFormat->setText( config::importer::pathparse::get() );
}

batchImportDialog::~batchImportDialog()
{
	config::importer::pathparse::set( ui->tbFormat->text() );

	delete ui;
}

void batchImportDialog::on_btnSetFolder_pressed()
{
	auto directory { QFileDialog::getExistingDirectory( this, "Select folder to add" ) };

	if ( directory.isEmpty() || !QFile::exists( directory ) )
		return;
	else
		ui->tbPath->setText( std::move( directory ) );
}

void batchImportDialog::processFiles()
{
	ZoneScoped;
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

	const QString cleaned_regex { regexify( escapeStr( QString::fromStdString( (base / search).string() ) ) ) };

	const bool move_imported { ui->moveImported->isChecked() };

	auto processGame = [ cleaned_regex, base, move_imported ]( const std::filesystem::path folder ) -> GameImportData
	{
		ZoneScopedN( "test folder" );
		std::vector< std::filesystem::path > potential_executables;

		//Check for a valid game in the folder
		for ( const auto& file : std::filesystem::directory_iterator( folder ) )
		{
			ZoneScopedN( "Scan files" );
			if ( file.is_regular_file() )
			{
				QMimeDatabase mime_db;
				const auto type {
					mime_db
						.mimeTypeForFile( QString::fromStdString( file.path().string() ), QMimeDatabase::MatchContent )
				};

				if ( ( type.inherits( "text/html" ) && file.path().filename() == "index.html" )
				     || ( type.inherits( "application/x-ms-dos-executable" ) && file.path().extension() == ".exe" ) )
					potential_executables.emplace_back( std::filesystem::relative( file, folder ) );
			}
		}

		if ( potential_executables.size() > 0 )
		{
			ZoneScopedN( "Add to list" );
			const auto [ title, version, creator ] =
				extractGroups( cleaned_regex, QString::fromStdString( folder.string() ) );

			const GameImportData game_data { std::filesystem::relative( folder, base ),
				                             title,
				                             version,
				                             creator,
				                             folderSize( folder ),
				                             potential_executables,
				                             potential_executables.at( 0 ),
				                             move_imported };
			return game_data;
		}
		else
			throw std::runtime_error(
				"Fuck" ); //TODO: This needs better error handling. It REALLY shouldn't ever fail due to us though.
	};

	std::vector< QFuture< GameImportData > > futures;

	spdlog::info( "Scanning {} for games", base );

	for ( auto itter = std::filesystem::
	          recursive_directory_iterator( base, std::filesystem::directory_options::skip_permission_denied );
	      itter != std::filesystem::recursive_directory_iterator();
	      ++itter )
	{
		auto& file { *itter };
		if ( file.is_directory() && valid( cleaned_regex, QString::fromStdString( itter->path().string() ) ) )
		{
			const auto future { QtConcurrent::run( processGame, file.path() ) };
			futures.push_back( future );
			itter.pop();
			if ( itter == std::filesystem::recursive_directory_iterator() ) break;
		}
	}

	for ( const auto& future : futures )
		if ( future.isValid() ) dynamic_cast< BatchImportModel* >( ui->twGames->model() )->addGame( future.result() );

	ui->twGames->resizeColumnsToContents();
}

void batchImportDialog::on_btnNext_pressed()
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

	ui->swImportGames->setCurrentIndex( 1 );
	ui->btnBack->setEnabled( true );
	ui->btnNext->setEnabled( false );

	processFiles();
}

void batchImportDialog::on_btnBack_pressed()
{
	//Clear the model
	dynamic_cast< BatchImportModel* >( ui->twGames->model() )->clearData();
}
