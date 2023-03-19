
#include "ui/mainwindow.h"

#include <filesystem>
#include <fstream>
#include <QApplication>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //NEED TO ADD A CLASS TO LOAD CUSTOM qss FILES. WE WILL NEED TO MAKE A NEW DIR SO THE CAN BE ADDED.
    //QFile styleFile( "C:/Users/tower/source/repos/hydrus95_gui/ui/qss/OledBlack.qss" );
    //styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    //QString styleSheet = QLatin1String(styleFile.readAll());
    //app.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return app.exec();
}
