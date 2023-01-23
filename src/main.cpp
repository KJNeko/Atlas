//
// Created by kj16609 on 1/11/23.
//

#include <QApplication>

#include "h95/database/database.hpp"

#include "ui/MainWindow.hpp"
#include <tracy/Tracy.hpp>

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

	MainWindow window;
	window.show();

	return app.exec();
}
