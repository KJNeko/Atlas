#include "batchimportdialog.h"

#include <QFileDialog>
#include <QMimeDatabase>

#include "h95/config.hpp"
#include "h95/regex.hpp"
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
	std::vector< GameImportData > folder_data;

	/*
	 * base: /run/media/kj16609/HDDWIN/NSFWSorted/
	 * regex: {path}/{creator}/{title}/{version}
	 *
	 */

	const std::filesystem::path base { ui->tbPath->text().toStdString() };
	const std::filesystem::path search { ui->tbFormat->text().toStdString() };

	//Calculate search depth
	const int depth { [ & ]()
		              {
						  auto temp { search };
						  int counter { 0 };

						  while ( !temp.empty() )
						  {
							  ++counter;
							  temp = temp.parent_path();
						  }

						  return counter - 1;
					  }() };

	const QString cleaned_regex {
		"^" + regexify( ui->tbFormat->text().replace( "{path}/", QString::fromStdString( base.string() ) ) ) + "$"
	};

	for ( auto folder = std::filesystem::recursive_directory_iterator( ui->tbPath->text().toStdString() );
	      folder != std::filesystem::recursive_directory_iterator();
	      ++folder )
	{
		if ( folder.depth() != depth - 1 ) continue;

		if ( folder->is_directory() && valid( cleaned_regex, QString::fromStdString( folder->path().string() ) ) )
		{
			std::vector< std::filesystem::path > potential_executables;

			//Check for a valid game in the folder
			for ( const auto& file : std::filesystem::directory_iterator( folder->path() ) )
			{
				if ( file.is_regular_file() )
				{
					QMimeDatabase mime_db;
					const auto type { mime_db.mimeTypeForFile(
						QString::fromStdString( file.path().string() ), QMimeDatabase::MatchContent ) };

					if ( ( type.inherits( "text/html" ) && file.path().filename() == "index.html" )
					     || ( type.inherits( "application/x-ms-dos-executable" )
					          && file.path().extension() == ".exe" ) )
						potential_executables.emplace_back( std::filesystem::relative( file, folder->path() ) );
				}
			}

			if ( potential_executables.size() > 0 )
			{
				const auto [ title, version, creator ] =
					extractGroups( cleaned_regex, QString::fromStdString( folder->path().string() ) );

				//TODO: Calculate size stuff

				GameImportData game_data { std::filesystem::relative( folder->path(), base ),
					                       title,
					                       version,
					                       creator,
					                       QString::number( 0 ),
					                       potential_executables,
					                       potential_executables.at( 0 ),
					                       ui->moveImported->isChecked() };
				folder_data.emplace_back( game_data );
			}
		}
	}

	//Add all games into list
	for ( const auto& folder : folder_data )
		dynamic_cast< BatchImportModel* >( ui->twGames->model() )->addGame( folder );

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
{}
