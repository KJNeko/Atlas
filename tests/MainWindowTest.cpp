//
// Created by kj16609 on 5/18/23.
//

#include <QObject>
#include <QTest>

#include <catch2/catch_test_macros.hpp>

#include "ui/mainwindow.h"

#define TEST_APP_HELPER                                                                                                \
	int argc { 0 };                                                                                                    \
	char** argv { nullptr };                                                                                           \
	QApplication app { argc, argv };

TEST_CASE( "MainWindow", "[gui][mainwindow]" )
{
	TEST_APP_HELPER

	Database::initalize( ":memory:" );

	SECTION( "init" )
	{
		MainWindow window;
		REQUIRE_NOTHROW( window.show() );

		//TODO: The rest of the fucking test
	}

	Database::deinit();
}
