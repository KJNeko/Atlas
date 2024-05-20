import sys
#IMPORT QRC FILE
import rc_resources
from ui import mainwindow
from core.logger import *
from core.settings import *
from core.database.database import *


from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt, QFile)
from PySide6.QtWidgets import (QApplication, QMainWindow, QSizePolicy, QStatusBar, QWidget, QSplashScreen)
from PySide6.QtGui import (QPixmap)


##try:
#   from ctypes import windll  # Only exists on Windows.
#    myappid = 'mycompany.myproduct.subproduct.version'
#    windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
#except ImportError:
#    pass


def main():  
    #Before doing anything, make sure this is not another instance running.   
    logger.is_console = True
    logger.is_enabled = True
    logger.info("Booting into Atlas")
    app = QApplication(sys.argv)
    splashscreen = QPixmap(":/images/assets/Atlas_logo_v2.svg")
    pixmap = splashscreen.scaled(200,200, Qt.AspectRatioMode.KeepAspectRatio)
    splash = QSplashScreen(pixmap)
    splash.show()
    app.processEvents()

    logger.debug("Checking config file")
    #Load settings from ini file.
    settings.load()

    logger.debug("Loading stylesheet")
    #Load dark.qss or system them. By default it will use dark.qss
    if settings.use_system_theme == False :
        if len(settings.current_theme) > 0:
            try:
                style_sheet_file = os.path.join(os.getcwd(), settings.themes_path,settings.current_theme)
                if os.path.exists(style_sheet_file):
                    with open(style_sheet_file,"r") as fh:
                        app.setStyleSheet(fh.read())
            except Exception as e:
                logger.error(str(e))

    #Check database
    settings.database_abs_path = os.path.join(settings.database_path, "atlas.db")
    database.initialize(settings.database_abs_path)

    #Show main window
    window = mainwindow.MainWindow()
    window.show()
    splash.finish(window)
    sys.exit(app.exec())

#def create_directories():
#    os.mkdir(settings.config['PATHS'][''])

main()


