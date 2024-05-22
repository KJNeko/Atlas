import sys
from shutil import copyfile
from os import path, mkdir, listdir, getcwd
#IMPORT QRC FILE
import rc_resources
from ui import mainwindow
from core.logger import *
from core.settings import *
from core.database.database import *

from PySide6.QtCore import (Qt)
from PySide6.QtWidgets import (QApplication, QSplashScreen)
from PySide6.QtGui import (QPixmap)


##try:
#   from ctypes import windll  # Only exists on Windows.
#    myappid = 'mycompany.myproduct.subproduct.version'
#    windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
#except ImportError:
#    pass

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

#Create data folder and make sure themes are copied
logger.debug("Creating data folder")
if not path.exists("data"):
    mkdir("data")
    if not path.exists("data/themes"):
        mkdir("data/themes")

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
    logger.debug("You are running this program in debug mode. Please copy themes over manually")

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
