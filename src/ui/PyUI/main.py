import sys
from ui.window.mainwindow import MainWindow
from ui.UIStyle import *
from core.Hydrus95FileManagement import *
from PyQt6 import QtCore, QtGui, QtWidgets

if __name__ == "__main__":

    app = QtWidgets.QApplication(sys.argv)

    #QFile styleFile( "resources/qss/default.hydrus.qss" );
    #styleFile.open( QFile::ReadOnly );
    #QString style( styleFile.readAll() );
    #app.setStyleSheet( style );
    #InitialiseDefaults()
    window = MainWindow()
    window.show()
    app.exec()
