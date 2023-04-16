//
// Created by kj16609 on 1/11/23.
//

#include <filesystem>
#include <fstream>

#include <QApplication>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QPixmapCache>

#include "atlas/config.hpp"
#include "atlas/database/Database.hpp"
#include "ui/mainwindow.h"

void checkWebp()
{
	const auto writer_formats { QImageWriter::supportedImageFormats() };
	const auto reader_formats { QImageReader::supportedImageFormats() };

	if ( !writer_formats.contains( "webp" ) || !reader_formats.contains( "webp" ) )
	{
		QMessageBox::critical( nullptr, "Missing dependency", "Atlas requires libwebp support but did not find it!" );
		std::terminate();
	}
}

void checkSupport()
{
	checkWebp();
}

int main( int argc, char** argv )
{
	//initLogging();

	QApplication app { argc, argv };

	//Fix for windeployqt not adding the bin directory to itself for some reason
	QApplication::addLibraryPath( QString::fromStdString( std::filesystem::canonical( "." ).string() ) );

	// Check for everything we need/use
	checkSupport();

	if ( !config::ui::use_system_theme::get() )
	{
		QFile style_sheet_file { config::paths::theme::get() };

		if ( !style_sheet_file.exists() && !config::ui::use_system_theme::get() )
		{
			config::paths::theme::setDefault();
			QFile default_sheet { config::paths::theme::get() };
			default_sheet.open( QFile::ReadOnly );

			app.setStyleSheet( default_sheet.readAll() );
		}
		else
		{
			style_sheet_file.open( QFile::ReadOnly );

			app.setStyleSheet( style_sheet_file.readAll() );
		}
	}

	std::filesystem::create_directory( config::paths::database::getPath().parent_path() );
	std::filesystem::create_directory( config::paths::games::getPath() );
	std::filesystem::create_directory( config::paths::images::getPath() );


	std::filesystem::path db_path = config::paths::database::getPath() / "atlas.db";
	Database::initalize( db_path );

	QPixmapCache::setCacheLimit( 1024 * 128 );

	MainWindow w;
	w.show();
	return app.exec();
}
