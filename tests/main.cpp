//
// Created by kj16609 on 7/3/23.
//

#include <QCoreApplication>
#include <QTimer>

#include <catch2/catch_session.hpp>

int main( int argc, char* argv[] )
{
	QCoreApplication app { argc, argv };

	QTimer::singleShot( 0, [ & ]() { app.exit( Catch::Session().run( argc, argv ) ); } );

	return app.exec();
}