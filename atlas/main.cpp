//
// Created by kj16609 on 1/11/23.
//

#include <QApplication>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QPixmapCache>
#include <QSplashScreen>

#include <filesystem>
#include <fstream>

#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/version.hpp"
#include "ui/mainwindow.h"

// clang-format off
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <csignal>
#include <sys/stat.h>
#endif
// clang-format on

void clear_lock()
{
	std::filesystem::remove( "atlas_lock" );
}

int main( int argc, char** argv )
{

	spdlog::info( "EntryPoint" );
	//initLogging();
	QApplication app { argc, argv };
	QPixmap splashscreen( ":/images/assets/Atlas_logo_v2.svg" );
	QSplashScreen splash(splashscreen.scaled( QSize( 200, 200 ), Qt::KeepAspectRatio ));
	splash.show();
    app.processEvents();

#ifdef _WIN32
	setlocale( LC_ALL, ".UTF8" );
#endif

	spdlog::info( "Booting Atlas version {}", ATLAS_VERSION_STR );

#ifdef _WIN32
	CreateMutexA( nullptr, FALSE, "Local\\$myprogram$" ); // try to create a named mutex
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) // did the mutex already exist?
		return -1; // quit; mutex is released automatically

#else
	if ( std::filesystem::exists( "atlas_lock" ) )
	{
		if ( std::ifstream ifs( "atlas_lock" ); ifs )
		{
			//Check if PID still is running

			int pid;
			ifs >> pid;

			struct stat sts;
			const std::string str { "/proc/" + std::to_string( pid ) };

			if ( kill( pid, 0 ) == -1 && errno == ESRCH )
			{
				//Process doesn't exist
				spdlog::info( "App is dead but didn't clean up it's own lock, removing lock" );
				std::filesystem::remove( "atlas_lock" );
			}
			else if ( stat( str.c_str(), &sts ) == 0 && errno == ENOENT )
			{
				//TODO: Check if process is the same executable and continue if not
			}
			else
			{
				QMessageBox::warning(
					nullptr,
					"Atlas is already running?",
					QString(
						"Atlas is already running with pid: %1. Please close the other instance before opening a new one." )
						.arg( QString::number( pid ) ) );
			}
		}
	}

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
#endif
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

	QPixmapCache::setCacheLimit( 1024 * 512 );

	MainWindow window;
	window.show();
	splash.finish(&window);
	const int code { app.exec() };
	Database::deinit();
	clear_lock();
	return code;
}
