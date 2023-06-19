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
	//setlocale( LC_ALL, ".UTF8" );

	spdlog::info( "Booting Atlas version {}", ATLAS_VERSION_STR );

	//initLogging();
	QApplication app { argc, argv };
	/*
	if ( std::filesystem::exists( "atlas_lock" ) )
	{
		if ( std::ifstream ifs( "atlas_lock" ); ifs )
		{
			//Check if PID still is running
#ifdef _WIN32
			DWORD pid;
			ifs >> pid;

			/*
			HANDLE handle = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pid );
			//Check if process is the same executable
			if ( handle )
			{
				std::array< char, MAX_PATH > buffer;
				GetModuleFileNameExA( handle, 0, buffer.data(), MAX_PATH );
				const std::filesystem::path path { std::filesystem::current_path() };
				const std::string_view prev_path { std::string_view( buffer.data(), strlen( buffer.data() ) ) };
				if ( path != prev_path )
				{
					//Process is not the same executable
					QMessageBox::warning(
						nullptr,
						"Atlas is already running?",
						QString(
							"Atlas is already running with pid: %s. Please close the other instance before opening a new one." )
							.arg( QString::number( pid ) ) );
					return 0;
				}
			}
			else
			{
				//We can only assume our previous instance is dead.
				//Remove the lock file and continue.
				std::filesystem::remove( "atlas_lock" );
			}

#else
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
#endif
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
*/
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
