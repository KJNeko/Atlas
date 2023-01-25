//
// Created by kj16609 on 1/11/23.
//

#include <QApplication>

#include "h95/database/database.hpp"

#include "ui/MainWindow.hpp"
#include "h95/config.hpp"
#include <tracy/Tracy.hpp>
#include <filesystem>

#ifdef TRACY_ENABLE
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

int main( int argc, char** argv )
{
	QApplication app { argc, argv };

	database::initalize();

	//Do basic setup to ensure all paths are proper

	const auto path	{getSettings< QString >( "paths/data", "./data/games" ).toStdString()};
	std::filesystem::create_directories(path);


	MainWindow window;
	window.show();

	return app.exec();
}
