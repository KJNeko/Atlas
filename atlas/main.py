import sys
import platform
from shutil import copyfile
from os import path, mkdir, listdir, getcwd, makedirs
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
    settings.os = platform.system()
    print(platform.system())
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
        if not path.exists(settings.engines_path):
            makedirs(settings.engines_path)

    #Copy over themes
    logger.debug("Checking themes files")
    #This will only work once the executable has been created
    internal_theme_path = "_internal/Resources/themes"
    internal_engine_path = "_internal/Resources/engines/types"


    if path.exists(internal_theme_path):
        copy_all_files(internal_theme_path, settings.themes_path )       
    else:
        copy_all_files("atlas/assets/themes", settings.themes_path )      
    
    if path.exists(internal_theme_path):
        copy_all_files(internal_engine_path, settings.engines_path )       
    else:
        copy_all_files("atlas/assets/engines/types", settings.engines_path )      

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

#move this method
def copy_all_files(from_path, to_path):
    for file in listdir(from_path):
        if not path.isfile(path.join(to_path, file)):
            copyfile(path.join(from_path, file), path.join(to_path, file) )