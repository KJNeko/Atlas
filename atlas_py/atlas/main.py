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
    #Now that the settings are loaded. Save to add any new config items that were not in the older config file.

    logger.debug("Loading stylesheet")

    if settings.use_system_theme == False :
        if len(settings.current_theme) > 0:
            try:
                logger.info(os.path.join(os.getcwd(), settings.themes_path,settings.current_theme))
                style_sheet_file = os.path.join(os.getcwd(), settings.themes_path,settings.current_theme)

                if os.path.exists(style_sheet_file):
                    with open(style_sheet_file,"r") as fh:
                        app.setStyleSheet(fh.read())
            except Exception as e:
                logger.error(str(e))

    #Check database
    settings.database_abs_path = os.path.join(settings.database_path, "atlas.db")
    database.initialize(settings.database_abs_path)

    window = mainwindow.MainWindow()
    window.show()
    splash.finish(window)
    sys.exit(app.exec())

#def create_directories():
#    os.mkdir(settings.config['PATHS'][''])

main()


