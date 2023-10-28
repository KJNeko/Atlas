#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //There are two entry points for this program. The first will just extract the files and move them, the second is a full updater.
}
