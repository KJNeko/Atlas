//
// Created by kj16609 on 7/3/23.
//

#include <iostream>

#include <QCoreApplication>
#include <QTimer>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <catch2/catch_session.hpp>
#pragma GCC diagnostic pop

int main( int argc, char* argv[] )
{
#ifdef _WIN32
	setlocale( LC_ALL, ".UTF8" );
#endif

	QCoreApplication app { argc, argv };

	QTimer::singleShot( 0, [ & ]() { app.exit( Catch::Session().run( argc, argv ) ); } );

	int val { app.exec() };

#ifdef _WIN32
	std::string fuck;
	std::cout << "press any key to exit" << std::endl;
	std::cin >> fuck;
#endif
	return val;
}