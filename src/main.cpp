//
// Created by kj16609 on 1/11/23.
//

#include <QApplication>

#include "h95/database/database.hpp"

#include "ui/MainWindow.hpp"
#include "h95/config.hpp"
#include <filesystem>

#include "h95/logging.hpp"

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

	const int result { std::atexit( spdlog_flush ) };
	spdlog::flush_on( spdlog::level::warn );

	QApplication app { argc, argv };

	const auto path { getSettings< QString >( "paths/data", "./data/games" ).toStdString() };
	std::filesystem::create_directories( path );

	if ( getSettings< bool >( "debug/very_vocal", true ) )
	{
		spdlog::set_level( spdlog::level::debug );
		spdlog::debug( "Debugging enabled." );
	}

	database::initalize();

	MainWindow window;
	window.show();

	return app.exec();
}
