//
// Created by kj16609 on 1/11/23.
//

#include <filesystem>
#include <fstream>

#include <QApplication>
#include <QFile>

#include "h95/config.hpp"
#include "h95/database/Database.hpp"
#include "h95/logging.hpp"
#include "ui/mainwindow.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>

void* operator new( std::size_t count )
{
	auto ptr = malloc( count );
	TracySecureAlloc( ptr, count );
	return ptr;
}

void* operator new[]( std::size_t count )
{
	auto ptr = malloc( count );
	TracySecureAlloc( ptr, count );
	return ptr;
}

void operator delete( void* ptr ) noexcept
{
	TracySecureFree( ptr );
	free( ptr );
}

void operator delete[]( void* ptr ) noexcept
{
	TracySecureFree( ptr );
	free( ptr );
}

void operator delete( void* ptr, std::size_t ) noexcept
{
	TracySecureFree( ptr );
	free( ptr );
}

void operator delete[]( void* ptr, std::size_t ) noexcept
{
	TracySecureFree( ptr );
	free( ptr );
}
#endif

void spdlog_flush()
{
	spdlog::shutdown();
}

int main( int argc, char** argv )
{
	initLogging();

	(void)std::atexit( spdlog_flush );
	spdlog::flush_on( spdlog::level::err );

	QApplication app { argc, argv };

	QFile style_sheet_file { config::paths::theme::getQString() };

	if ( !style_sheet_file.exists() )
	{
		config::paths::theme::setDefault();
		QFile default_sheet { config::paths::theme::getQString() };
		default_sheet.open( QFile::ReadOnly );

		app.setStyleSheet( default_sheet.readAll() );
	}
	else
	{
		style_sheet_file.open( QFile::ReadOnly );

		app.setStyleSheet( style_sheet_file.readAll() );
	}

	std::filesystem::create_directory( config::paths::database::get().parent_path() );
	std::filesystem::create_directory( config::paths::games::get() );
	std::filesystem::create_directory( config::paths::images::get() );

#ifdef NDEBUG
	//if ( getSettings< bool >( "debug/very_vocal", false ) )
	{
		spdlog::set_level( spdlog::level::debug );
		spdlog::debug( "Debugging enabled." );
	}
#else
	spdlog::set_level( spdlog::level::debug );
	spdlog::debug( "Debugging forcefully enabled due to not compiling with NDEBUG" );
#endif

	Database::initalize( config::paths::database::get() );

	MainWindow w;
	w.show();
	return app.exec();
}
