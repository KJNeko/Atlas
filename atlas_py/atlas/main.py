import sys
#IMPORT QRC FILE
import rc_resources
from ui import mainwindow
from core.logger import *
from core.settings import *


from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtWidgets import (QApplication, QMainWindow, QSizePolicy, QStatusBar, QWidget, QSplashScreen)
from PySide6.QtGui import (QPixmap)


try:
    from ctypes import windll  # Only exists on Windows.
    myappid = 'mycompany.myproduct.subproduct.version'
    windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
except ImportError:
    pass


def main():    
    logger.info("Booting into Atlas")
    app = QApplication(sys.argv)
    splashscreen = QPixmap(":/images/assets/Atlas_logo_v2.svg")
    pixmap = splashscreen.scaled(200,200, Qt.AspectRatioMode.KeepAspectRatio)
    splash = QSplashScreen(pixmap)
    splash.show()
    app.processEvents()

    logger.debug("Checking config file")
    settings.load()

    logger.debug("Loading stylesheet")
    

    window = mainwindow.MainWindow()
    window.show()
    splash.finish(window)
    sys.exit(app.exec())

#def create_directories():
#    os.mkdir(settings.config['PATHS'][''])

main()


