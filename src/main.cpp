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
#include <QPixmapCache>

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

	QFile style_sheet_file { config::paths::theme::get() };

	if ( !style_sheet_file.exists() )
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

	std::filesystem::create_directory( config::paths::database::getPath().parent_path() );
	std::filesystem::create_directory( config::paths::games::getPath() );
	std::filesystem::create_directory( config::paths::images::getPath() );

	Database::initalize( config::paths::database::getPath() );

	QPixmapCache::setCacheLimit( 1024 * 128 );

	MainWindow w;
	w.show();
	return app.exec();
}
