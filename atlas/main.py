import sys
import platform
from shutil import copyfile
from os import path, mkdir, listdir, getcwd
#IMPORT QRC FILE
import atlas.rc_resources
from atlas.ui import mainwindow
from atlas.core.logger import *
from atlas.core.settings import *
from atlas.core.database.database import *

from PySide6.QtCore import (Qt)
from PySide6.QtWidgets import (QApplication, QSplashScreen)
from PySide6.QtGui import (QPixmap)

#Before doing anything, make sure this is not another instance running.   
def init():
    #Get the operating system
    settings.os = platform.system
    logger.is_console = True
    logger.is_enabled = True
    logger.info("Booting into Atlas")
    app = QApplication(sys.argv)
    splashscreen = QPixmap(":/images/assets/Atlas_logo_v2.svg")
    pixmap = splashscreen.scaled(200,200, Qt.AspectRatioMode.KeepAspectRatio)
    splash = QSplashScreen(pixmap)
    splash.show()
    app.processEvents()

    #Create data folder
    logger.debug("Creating data folder")
    if not path.exists(settings.root_path):
        mkdir(settings.root_path)
        if not path.exists(settings.themes_path):
            mkdir(settings.themes_path)
        if not path.exists(settings.images_path):
            mkdir(settings.images_path)

    #Copy over themes
    logger.debug("Checking themes files")
    #This will only work once the executable has been created
    interal_path = "_internal/Resources/themes"
    if path.exists(interal_path):
        for file in listdir(interal_path):
            #Copy themes files if the are not in the folder.
            if not path.isfile(path.join("data/themes", file)):
                copyfile(path.join(interal_path, file), path.join("data/themes", file) )
                #shutil.copyfile('./demo.py', './demo1.py')
    else:
        for file in listdir("atlas/assets/themes"):
            #Copy themes files if the are not in the folder.
            if not path.isfile(path.join("data/themes", file)):
                copyfile(path.join("atlas/assets/themes", file), path.join("data/themes", file) )

    logger.debug("Checking config file")
    #Load settings from ini file.
    settings.load()

    logger.debug("Loading stylesheet")
    #Load dark.qss or system them. By default it will use dark.qss
    if settings.use_system_theme == False :
        if len(settings.current_theme) > 0:
            try:
                style_sheet_file = path.join(getcwd(), settings.themes_path,settings.current_theme)
                if path.exists(style_sheet_file):
                    with open(style_sheet_file,"r") as fh:
                        app.setStyleSheet(fh.read())
            except Exception as e:
                logger.error(str(e))

    #Check database
    settings.database_abs_path = path.join(settings.database_path, "atlas.db")
    database.initialize(settings.database_abs_path)

    #Show main window
    window = mainwindow.MainWindow()
    splash.close()
    window.show()
    sys.exit(app.exec())