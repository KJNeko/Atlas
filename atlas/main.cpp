//
// Created by kj16609 on 1/11/23.
//

#include <filesystem>
#include <fstream>

#include <QApplication>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QPixmapCache>

#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/version.hpp"
#include "ui/mainwindow.h"

int main( int argc, char** argv )
{
	spdlog::info( "Booting Atlas version {}", ATLAS_VERSION_STR );

	//initLogging();
	QApplication app { argc, argv };

	//Fix for windeployqt not adding the bin directory to itself for some reason
	QApplication::addLibraryPath( QString::fromStdString( std::filesystem::canonical( "." ).string() ) );

	qDebug() << QApplication::libraryPaths();

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