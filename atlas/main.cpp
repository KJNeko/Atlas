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

#include <dlfcn.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

/*
char tmpbuff[ 1024 ];
unsigned long tmppos = 0;
unsigned long tmpallocs = 0;

void* memset( void*, int, size_t );
void* memmove( void* to, const void* from, size_t size );

//static void* ( *myfn_calloc )( size_t nmemb, size_t size );
static void* ( *myfn_malloc )( size_t size );
static void ( *myfn_free )( void* ptr );
//static void* ( *myfn_realloc )( void* ptr, size_t size );
//static void* ( *myfn_memalign )( size_t blocksize, size_t bytes );

static void init()
{
	myfn_malloc = reinterpret_cast< decltype( myfn_malloc ) >( dlsym( RTLD_NEXT, "malloc" ) );
	myfn_free = reinterpret_cast< decltype( myfn_free ) >( dlsym( RTLD_NEXT, "free" ) );
	//myfn_calloc = reinterpret_cast< decltype( myfn_calloc ) >( dlsym( RTLD_NEXT, "calloc" ) );
	//myfn_realloc = reinterpret_cast< decltype( myfn_realloc ) >( dlsym( RTLD_NEXT, "realloc" ) );
	//myfn_memalign = reinterpret_cast< decltype( myfn_memalign ) >( dlsym( RTLD_NEXT, "memalign" ) );

	if ( !myfn_malloc || !myfn_free) // || !myfn_calloc || !myfn_realloc || !myfn_memalign )
	{
		fprintf( stderr, "Error in `dlsym`: %s\n", dlerror() );
		exit( 1 );
	}
}

void* malloc( size_t size )
{
	static int initializing = 0;
	if ( myfn_malloc == nullptr )
	{
		if ( !initializing )
		{
			initializing = 1;
			init();
			initializing = 0;

			fprintf(
				stdout,
				"jcheck: allocated %lu bytes of temp memory in %lu chunks during initialization\n",
				tmppos,
				tmpallocs );
		}
		else
		{
			if ( tmppos + size < sizeof( tmpbuff ) )
			{
				void* retptr = tmpbuff + tmppos;
				tmppos += size;
				++tmpallocs;
				return retptr;
			}
			else
			{
				fprintf( stdout, "jcheck: too much memory requested during initialisation - increase tmpbuff size\n" );
				exit( 1 );
			}
		}
	}

	void* ptr = myfn_malloc( size );
	TracyAlloc( ptr, size );
	return ptr;
}

void free( void* ptr )
{
	// something wrong if we call free before one of the allocators!
	//  if (myfn_malloc == NULL)
	//      init();

	if ( ptr >= (void*)tmpbuff && ptr <= (void*)( tmpbuff + tmppos ) )
		fprintf( stdout, "freeing temp memory\n" );
	else
	{
		myfn_free( ptr );
		TracyFree( ptr );
	}
}*/

void clear_lock()
{
	std::filesystem::remove( "atlas_lock" );
}

int main( int argc, char** argv )
{
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

	MainWindow w;
	w.show();
	const int code { app.exec() };
	Database::deinit();
	clear_lock();
	return code;
}
