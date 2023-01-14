//
// Created by kj16609 on 1/11/23.
//

#include <QApplication>

#include <backend/database.hpp>
#include <ui/MainWindow.hpp>

int main( int argc, char** argv )
{
	QApplication app { argc, argv };

	database::initalize();

	MainWindow window;
	window.show();

	return app.exec();
}
