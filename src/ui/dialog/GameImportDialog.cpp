//
// Created by kj16609 on 1/17/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameImportDialog.h" resolved

#include <filesystem>

#include <QFileDialog>
#include <QDirIterator>
#include <QMimeDatabase>

#include <tracy/Tracy.hpp>

#include "GameImportDialog.hpp"
#include "ui_GameImportDialog.h"

#include <h95/Importer.hpp>
#include <QMessageBox>


constexpr char preview_delim { ';' };

inline std::vector< std::filesystem::path > deserializePreviews( const QString& str )
{
	const auto list { str.split( preview_delim, Qt::SkipEmptyParts ) };

	std::vector< std::filesystem::path > paths;

	for ( const auto& file : list ) paths.emplace_back( file.toStdString() );

	return paths;
}

inline QString serializePreviews( const QList< QString >& list )
{
	if ( list.empty() ) return {};

	QString previews;

	for ( const auto& file : list ) previews += file + preview_delim;

	if ( previews.endsWith( preview_delim ) ) previews.chop( 1 );

	return previews;
}

inline QString serializePreviews( const std::vector< std::filesystem::path >& paths )
{
	QString previews;

	for ( const auto& file : paths ) previews += QString::fromStdString( file.string() ) + preview_delim;

	if ( previews.endsWith( preview_delim ) ) previews.chop( 1 );

	return previews;
}

inline QString fixPathDelimiter( QString str )
{
	return str.replace( '/', QDir::separator() ).replace( '\\', QDir::separator() );
}

GameImportDialog::GameImportDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::GameImportDialog )
{
	ui->setupUi( this );

	ui->shouldParsePath->setChecked( getSettings< bool >( "import/should_autofill", true ) );
	ui->pathParse->setText(
		fixPathDelimiter( getSettings< QString >( "import/auto_fill_path", "{data}/{title}/{version}" ) ) );

	ui->copyToDest->setChecked( getSettings< bool >( "import/should_copy", true ) );
	ui->dest->setText(
		fixPathDelimiter( getSettings< QString >( "import/dest_path", "{h95_games}/{title}/{version}" ) ) );

	ui->deleteAfterCopy->setChecked( getSettings< bool >( "import/should_delete", false ) );
}

GameImportDialog::GameImportDialog( const QUrl& url, QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::GameImportDialog )
{
	ui->setupUi( this );

	ui->shouldParsePath->setChecked( getSettings< bool >( "import/should_autofill", true ) );
	ui->pathParse->setText(
		fixPathDelimiter( getSettings< QString >( "import/auto_fill_path", "{data}/{title}/{version}" ) ) );

	ui->copyToDest->setChecked( getSettings< bool >( "import/should_copy", true ) );
	ui->dest->setText(
		fixPathDelimiter( getSettings< QString >( "import/dest_path", "{h95_games}/{title}/{version}" ) ) );

	ui->deleteAfterCopy->setChecked( getSettings< bool >( "import/should_delete", false ) );

	ui->folderPath->setText( url.toLocalFile() );
	parseInfo();
}

GameImportDialog::~GameImportDialog()
{
	setSettings( "import/should_autofill", ui->shouldParsePath->isChecked() );
	setSettings( "import/auto_fill_path", ui->pathParse->text() );
	setSettings( "import/should_copy", ui->copyToDest->isChecked() );
	setSettings( "import/dest_path", ui->dest->text() );
	setSettings( "import/should_delete", ui->deleteAfterCopy->isChecked() );

	delete ui;
}


void GameImportDialog::on_cancelButton_pressed()
{
	this->close();
}

void GameImportDialog::verifySettings()
try
{
	ZoneScoped;
	good_import = false;
	ui->pathLabel->setText(
		QString::fromStdString( path_manager.fillPath( ui->dest->text().toStdString() ).string() ) );

	//Check that source is valid
	if ( !std::filesystem::exists( ui->folderPath->text().toStdString() ) )
	{
		QMessageBox::information( this, "Folder invalid", "Source path invalid or does not exist" );
		return;
	}

	//Check that the executable is valid
	if ( !( std::filesystem::exists( ui->execPath->text().toStdString() ) ) )
	{
		QMessageBox::information( this, "Executable path invalid", "Executable path invalid or does not exist" );
		return;
	}

	ui->infoLabel->setText( "One of the required Game Info fields were not populated" );

	//Check that title, creator and version are filled out
	if ( ui->title->text().isEmpty() )
	{
		QMessageBox::information( this, "Required field no populated", "Title field not populated" );
		return;
	}

	if ( ui->creator->text().isEmpty() )
	{
		QMessageBox::information( this, "Required field not populated", "Creator field not populated" );
		return;
	}

	if ( ui->version->text().isEmpty() )
	{
		QMessageBox::information( this, "Required field not populated", "Version field not populated." );
		return;
	}

	if ( ui->copyToDest->isChecked()
		 && ( ui->pathLabel->text().contains( '{' ) && ui->pathLabel->text().contains( '}' ) ) )
	{
		QMessageBox::information( this, "Path label malformed", "All replacement values were not replaced properly" );
		return;
	}

	if ( ui->copyToDest->isChecked() && !ui->dest->text().contains( "{version}" ) )
	{
		QMessageBox::information( this, "Required replacement not present", "Destination path requires {version}" );
		return;
	}

	if ( ui->copyToDest->isChecked() && !ui->dest->text().contains( "{title}" ) )
	{
		QMessageBox::information( this, "Required replacement not present", "Destination path requires {title}" );
		return;
	}

	//Check that there is not already a folder where we want to place the file
	if ( std::filesystem::exists( ui->pathLabel->text().toStdString() )
		 && ui->copyToDest->isChecked() )  //&& !ui->forceCopy->isChecked())
	{
		QMessageBox::information(
			this,
			"Record already exists",
			"Destination folder already exists. Are you trying to update? Use a new version instead" );
		return;
	}

	ui->infoLabel->setText( "Good to import!" );
	good_import = true;
}
catch ( std::exception& e )
{
	spdlog::warn( "Something went wrong while verifying settings!" );
	spdlog::dump_backtrace();
}

void GameImportDialog::parseInfo()
{
	QMimeDatabase mime_db;

	std::vector< std::filesystem::path > exec_paths;

	//Locate supporting files
	for ( const auto& temp_dir : std::filesystem::directory_iterator( ui->folderPath->text().toStdString() ) )
	{
		spdlog::debug( "Searching for what {} is", temp_dir.path().string() );

		const auto mime_type { mime_db.mimeTypeForFile( QString::fromStdString( temp_dir.path().string() ) ).name() };

		constexpr auto banner_str_size { std::string( "banner" ).size() };

		// Locate executable
		if ( temp_dir.is_regular_file() && mime_type == "application/x-ms-dos-executable" )
		{
			exec_paths.emplace_back( temp_dir );
		}
		//Locate banner
		else if (
			temp_dir.is_regular_file() && temp_dir.path().filename().string().substr( 0, banner_str_size ) == "banner" )
		{
			ui->bannerPath->setText( QString::fromStdString( temp_dir.path().string() ) );
		}
		//Locate previews
		else if ( temp_dir.is_directory() && temp_dir.path().filename() == "previews" )
		{
			QString previews;

			for ( const auto& file : std::filesystem::directory_iterator( temp_dir.path() ) )
			{
				const auto file_type {
					mime_db.mimeTypeForFile( QString::fromStdString( file.path().string() ) ).name() };

				spdlog::debug( "Found {} in previews with filetype {}", file.path().string(), file_type.toStdString() );

				if ( file.is_regular_file()
					 && ( file_type == "image/jpeg" || file_type == "image/png" || file_type == "image/webp" ) )
				{
					previews += QString::fromStdString( file.path().string() + ";" );
				}
			}

			if ( previews.endsWith( ':' ) ) previews.chop( 1 );

			ui->previewPaths->setText( previews );
		}
	}

	std::vector< std::string > anti_list { "UnityCrashHandler64.exe", "UnityCrashHandler32.exe" };

	//Remove any anit-list
	std::remove_if(
		exec_paths.begin(),
		exec_paths.end(),
		[&]( const std::filesystem::path& path )
		{
			for ( const auto& anti : anti_list )
			{
				if ( path.filename().string().starts_with( anti ) )
					return true;
				else
					continue;
			}
			return false;
		} );

	if ( exec_paths.size() == 1 )
		ui->execPath->setText( QString::fromStdString( exec_paths.back().string() ) );
	else
	{
		//Have user pick the file
		QMessageBox::information(
			this,
			"Unable to auto-determine",
			"Unable to auto determine executable. Please select it manually" );
		on_selectExec_pressed();
	}
}

void GameImportDialog::on_importButton_pressed()
try
{
	ZoneScoped;
	verifySettings();
	if ( !good_import ) return;

	Importer importer {};

	importer.setSource( ui->folderPath->text().toStdString() );
	importer.setExecutable( ui->execPath->text().toStdString() );

	spdlog::debug( "Is copyToDest checked? {}", ui->copyToDest->isChecked() );
	if ( ui->copyToDest->isChecked() ) { importer.setDestination( ui->pathLabel->text().toStdString() ); }
	else { importer.setDestination( ui->folderPath->text().toStdString() ); }

	importer.setPreviews( deserializePreviews( ui->previewPaths->text() ) );
	importer.setBanner( ui->bannerPath->text().toStdString() );

	importer.import_game(
		ui->title->text(),
		ui->creator->text(),
		ui->version->text(),
		ui->engine->text(),
		ui->deleteAfterCopy->isTristate() );

	emit importComplete();
	this->close();
}
catch ( std::exception& e )
{
	spdlog::error( "Failed to import file! Send the this error to the dev. Exception: {}", e.what() );
	spdlog::dump_backtrace();
}

void GameImportDialog::on_selectPath_pressed()
try
{
	ZoneScoped;
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

		std::filesystem::path dir { list.first().toStdString() };
		if ( !std::filesystem::exists( dir ) )
			return;
		else
			parseInfo();
	}
}
catch ( std::exception& e )
{
	spdlog::warn( "Failed to process on_selectPath_pressed(): {}", e.what() );
}

void GameImportDialog::on_selectExec_pressed()
{
	ZoneScoped;
	QFileDialog dialog { this };
	dialog.setDirectory( ui->folderPath->text() );
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

const QStringList file_filters { "Image files (*.png, *.jpg *.gif *.tiff *.webp)", "Any files (*)" };

void GameImportDialog::on_selectBanner_pressed()
{
	ZoneScoped;
	QFileDialog dialog { this };
	dialog.setNameFilters( file_filters );
	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	dialog.setLabelText( QFileDialog::LookIn, "Select banner image" );

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };

		if ( list.empty() )
			return;
		else
			ui->bannerPath->setText( list.first() );
	}
}

void GameImportDialog::on_selectPreviews_pressed()
{
	ZoneScoped;
	QFileDialog dialog { this };
	dialog.setNameFilters( file_filters );
	dialog.setFileMode( QFileDialog::ExistingFiles );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	dialog.setLabelText( QFileDialog::LookIn, "Select previews" );

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };


		ui->previewPaths->setText( serializePreviews( list ) );
	}
}


void GameImportDialog::on_title_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.key_replacer.registerKey( "{title}", text );
}

void GameImportDialog::on_creator_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.key_replacer.registerKey( "{creator}", text );
}

void GameImportDialog::on_version_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.key_replacer.registerKey( "{version}", text );
}

void GameImportDialog::on_engine_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.key_replacer.registerKey( "{engine}", text );
}

void GameImportDialog::on_folderPath_textChanged( [[maybe_unused]] const QString text )
{
	ZoneScoped;
	if ( ui->shouldParsePath->isChecked() )
		path_manager.populateValues( text.toStdString(), ui->pathParse->text().toStdString() );

	if ( !no_replace )
	{
		ui->title->setText( path_manager.key_replacer.value( { "{title}" } ) );
		ui->creator->setText( path_manager.key_replacer.value( "{creator}" ) );
		ui->version->setText( path_manager.key_replacer.value( "{version}" ) );
		ui->engine->setText( path_manager.key_replacer.value( "{engine}" ) );
	}
}

void GameImportDialog::setInformation( const QString& creator, const QString& title, const QString& engine )
{
	no_replace = true;
	ui->creator->setText( creator );
	ui->title->setText( title );
	ui->engine->setText( engine );
}
