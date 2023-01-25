//
// Created by kj16609 on 1/17/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameImportDialog.h" resolved

#include "GameImportDialog.hpp"
#include "ui_GameImportDialog.h"
#include <filesystem>
#include <QFileDialog>
#include <QDirIterator>
#include <QMimeDatabase>
#include <iostream>

#include <tracy/Tracy.hpp>

constexpr char preview_delim { ';' };

inline std::vector< std::filesystem::path > deserializePreviews( const QString& str )
{
	const auto list { str.split( preview_delim ) };

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
	ui->pathParse->setText( fixPathDelimiter( getSettings< QString >( "import/auto_fill_path", "{data}/{title}" ) ) );

	ui->copyToDest->setChecked( getSettings< bool >( "import/should_copy", true ) );
	ui->dest->setText( fixPathDelimiter(getSettings<QString>("import/dest_path", "{h95_games}/{title}")));

	ui->deleteAfterCopy->setChecked(getSettings<bool>("import/should_delete", false));
}

GameImportDialog::~GameImportDialog()
{
	setSettings("import/should_autofill", ui->shouldParsePath->isChecked());
	setSettings("import/auto_fill_path", ui->pathParse->text());
	setSettings("import/should_copy", ui->copyToDest->isChecked());
	setSettings("import/dest_path", ui->dest->text());
	setSettings("import/should_delete", ui->deleteAfterCopy->isChecked());

	delete ui;
}


void GameImportDialog::on_cancelButton_pressed()
{
	this->close();
}

void GameImportDialog::verifySettings()
{
	ZoneScoped;
	good_import = false;
	ui->pathLabel->setText(
		QString::fromStdString( path_manager.fillPath( ui->dest->text().toStdString() ).string() ) );


	//Check that path and executable exists
	if ( !std::filesystem::exists( ui->folderPath->text().toStdString() ) )
	{
		ui->infoLabel->setText( "Input folder path invalid or does not exist!" );
		return;
	}

	if ( !( std::filesystem::exists( ui->execPath->text().toStdString() ) ) )
	{
		ui->infoLabel->setText( "Executable path is invalid or does not exist!" );
		return;
	}
	else
	{
		const QMimeDatabase mime_db;
		const auto mime_type { mime_db.mimeTypeForFile( ui->execPath->text(), QMimeDatabase::MatchContent ) };
		if ( mime_type.name() != "application/x-ms-dos-executable" )
		{
			ui->infoLabel->setText(
				QString( "File executable not proper type (Wants: application/x-ms-dos-executable vs Actual: %1" )
					.arg( mime_type.name() ) );
		}
	}

	//Check that title, creator and version are filled out
	if ( ui->title->text().isEmpty() || ui->creator->text().isEmpty() || ui->version->text().isEmpty() )
	{
		ui->infoLabel->setText( "One of the required Game Info fields were not populated" );
		return;
	}

	if ( ui->pathLabel->text().contains( '{' ) || ui->pathLabel->text().contains( '}' ) )
	{
		ui->infoLabel->setText( "Path label malformed. All {} must be properly filled out" );
		return;
	}

	ui->infoLabel->setText( "Good to import!" );
	good_import = true;
}

void GameImportDialog::on_importButton_pressed()
{
	ZoneScoped;
	verifySettings();
	if(!good_import)
		return;

	if ( ui->copyToDest->isChecked() )
	{
		const auto path_str { ui->pathLabel->text() };
		const std::filesystem::path game_path { path_str.toStdString() };
		path_manager.setRoot( game_path );

		//Copy game
		std::filesystem::create_directories( game_path.parent_path() );
		std::filesystem::copy(
			ui->folderPath->text().toStdString(),
			game_path,
			std::filesystem::copy_options::recursive );

		//Copy extra information (previews + banners)
		const std::filesystem::path banner { ui->bannerPath->text().toStdString() };
		const std::filesystem::path banner_new { game_path / ( "banner" + banner.extension().string() ) };

		//Check that we didn't copy the banner during our first copy

		//Copy the banner
		if ( std::filesystem::exists( banner ) && !std::filesystem::exists( banner_new ) )
			std::filesystem::copy( banner, banner_new );

		//Set banner path to be the new relative path
		if ( std::filesystem::exists( banner_new ) )
			ui->bannerPath->setText( QString::fromStdString( path_manager.relative( banner_new ).string() ) );

		//Copy the previews
		auto previews { deserializePreviews( ui->previewPaths->text() ) };

		uint16_t counter { 0 };
		for ( auto& preview : previews )
		{
			const std::filesystem::path preview_new { game_path / "previews" / preview.filename() };
			std::filesystem::copy( preview, preview_new );
			preview = path_manager.relative( preview_new );

			++counter;
		}

		//Set preview paths to the new relative path
		ui->previewPaths->setText( serializePreviews( previews ) );

		if ( ui->deleteAfterCopy->isChecked() )
		{
			std::filesystem::remove( ui->execPath->text().toStdString() );
			if ( std::filesystem::exists( banner ) ) std::filesystem::remove( banner );
			for ( const auto& preview : previews ) std::filesystem::remove( preview );
		}
	}
	else
	{
		const std::filesystem::path filepath { ui->folderPath->text().toStdString() };
		const std::filesystem::path executable { ui->execPath->text().toStdString() };

		path_manager.setRoot( filepath );
		ui->execPath->setText( QString::fromStdString( path_manager.relative( executable ).string() ) );
	}

	//Seperate preview paths
	const auto list { ui->previewPaths->text().split( ';', Qt::SkipEmptyParts ) };
	std::vector< std::filesystem::path > previews;
	for ( const auto& path : list ) previews.emplace_back( path.toStdString() );

	//Import
	Record::create(
		ui->title->text(),
		ui->creator->text(),
		ui->version->text(),
		ui->engine->text(),
		{ ui->folderPath->text().toStdString(), ui->execPath->text().toStdString() },
		ui->bannerPath->text().toStdString(),
		previews );

	emit importComplete();
	this->close();
}

void GameImportDialog::on_selectPath_pressed()
{
	ZoneScoped;
	QFileDialog dialog;
	dialog.setFileMode( QFileDialog::Directory );
	dialog.setOption( QFileDialog::ReadOnly );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	dialog.setLabelText( QFileDialog::LookIn, "Select game folder" );

	QMimeDatabase mime_db;

	if ( !dialog.exec() )
		return;
	else
	{
		const auto list { dialog.selectedFiles() };

		if ( list.empty() ) return;

		ui->folderPath->setText( list.first() );

		std::filesystem::path dir { list.first().toStdString() };
		if ( !std::filesystem::exists( dir ) ) return;

		//Locate supporting files
		for ( const auto& temp_dir : std::filesystem::directory_iterator( dir ) )
		{
			const auto mime_type {
				mime_db.mimeTypeForFile( QString::fromStdString( temp_dir.path().string() ) ).name() };

			// Locate executable
			if ( temp_dir.is_regular_file() && temp_dir.path().extension() == ".exe"
				 && mime_type == "application/x-ms-dos-executable" )
			{
				ui->execPath->setText( QString::fromStdString( temp_dir.path().string() ) );
				break;
			}
			//Locate banner
			else if (
				temp_dir.is_regular_file() && ( mime_type == "image/jpeg" || mime_type == "image/png" )
				&& temp_dir.path().filename().string().substr( 0, 7 ) == "banner" )
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

					if ( file.is_regular_file() && ( file_type == "image/jpeg" || file_type == "image/png" ) )
					{
						previews += QString::fromStdString( file.path().string() + ";" );
					}
				}

				if ( previews.endsWith( ':' ) ) previews.chop( 1 );

				ui->previewPaths->setText( previews );
			}
		}
	}
}

void GameImportDialog::on_selectExec_pressed()
{
	ZoneScoped;
	QFileDialog dialog { this };
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

const QStringList file_filters { "Image files (*.png, *.jpg *.gif *.tiff)", "Any files (*)" };

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

	verifySettings();
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

	verifySettings();
}


void GameImportDialog::on_title_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{title}", text );
	verifySettings();
}

void GameImportDialog::on_creator_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{creator}", text );
	verifySettings();
}

void GameImportDialog::on_version_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{version}", text );
	verifySettings();
}

void GameImportDialog::on_engine_textChanged( [[maybe_unused]] const QString text )
{
	path_manager.registerReplacement( "{engine}", text );
	verifySettings();
}

void GameImportDialog::on_folderPath_textChanged( [[maybe_unused]] const QString text )
{
	ZoneScoped;
	if ( ui->shouldParsePath->isChecked() )
		path_manager.populateValues( text.toStdString(), ui->pathParse->text().toStdString() );

	ui->title->setText( path_manager.value( { "{title}" } ) );
	ui->creator->setText( path_manager.value( "{creator}" ) );
	ui->version->setText( path_manager.value( "{version}" ) );
	ui->engine->setText( path_manager.value( "{engine}" ) );

	path_manager.setRoot( text.toStdString() );

	verifySettings();
}

void GameImportDialog::on_execPath_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}

void GameImportDialog::on_dest_textChanged( [[maybe_unused]] const QString text )
{
	verifySettings();
}
