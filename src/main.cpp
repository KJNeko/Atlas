//
// Created by kj16609 on 1/11/23.
//

#include <filesystem>
#include <fstream>

#include <QApplication>

#include "h95/database/Database.hpp"
#include "h95/config.hpp"
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

	(void) std::atexit( spdlog_flush );
	spdlog::flush_on( spdlog::level::err );

	QApplication app { argc, argv };

	const auto path { getSettings< QString >( "paths/data", "./data" ).toStdString() };
	std::filesystem::create_directories( path );

#ifdef NDEBUG
	if ( getSettings< bool >( "debug/very_vocal", false ) )
	{
		spdlog::set_level( spdlog::level::debug );
		spdlog::debug( "Debugging enabled." );
	}
#else
	spdlog::set_level(spdlog::level::debug);
	spdlog::debug("Debugging forcefully enabled due to not compiling with NDEBUG");
#endif

	Database::initalize("./data/hydrus95.db");
}
