#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
	//Check if argc is 0 or 1, If it is a 0 dont show the gui
    
	QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
