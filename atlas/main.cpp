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

#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/version.hpp"
#include "ui/mainwindow.h"

void clear_lock()
{
	std::filesystem::remove( "atlas_lock" );
}

int main( int argc, char** argv )
{
	setlocale(LC_ALL, ".UTF8");

	spdlog::info( "Booting Atlas version {}", ATLAS_VERSION_STR );

	//initLogging();
	QApplication app { argc, argv };

	if ( std::filesystem::exists( "atlas_lock" ) )
	{
		if ( std::ifstream ifs( "atlas_lock" ); ifs )
		{
			qint64 pid;
			ifs >> pid;

			QMessageBox::warning(
				nullptr,
				"Atlas is already running?",
				QString(
					"Atlas is already running with pid: %s. Please close the other instance before opening a new one." )
					.arg( pid ) );
		}
		else
		{
			//wtf?
			QMessageBox::critical(
				nullptr,
				"Failed to acquire lock",
				"Failed to acquire lock! Please make sure you have write permissions to the directory Atlas is in. Report to a dev if this issue persists. Wtf?" );
		}
		return EXIT_SUCCESS;
	}
	else
	{
		std::at_quick_exit( clear_lock );
		std::atexit( clear_lock );
		if ( std::ofstream ofs( "atlas_lock" ); ofs )
		{
			//Write PID
			ofs << std::to_string( QCoreApplication::applicationPid() );
		}
		else
		{
			QMessageBox::critical(
				nullptr,
				"Failed to acquire lock",
				"Failed to acquire lock! Please make sure you have write permissions to the directory Atlas is in. Report to a dev if this issue persists." );
			return EXIT_FAILURE;
		}
	}

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
	const int code { app.exec() };
	Database::deinit();
	clear_lock();
	return code;
}
