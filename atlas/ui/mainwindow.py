# This Python file uses the following encoding: utf-8
import sys

#from atlas.core.logger import *
from PySide6.QtCore import (Qt)
from PySide6.QtWidgets import QApplication, QMainWindow, QDialog
from atlas.ui.importer.BatchImporter import BatchImporter
from atlas.ui.dialog.AboutAtlas import AboutAtlas

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from atlas.ui.ui_mainwindow import Ui_MainWindow

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        #FOR FUTURE: HIDE FOR NOW
        self.ui.NavTop.hide()
        self.ui.actionBulkImporter.triggered.connect(self.on_actionBulkImporter_triggered)
        self.ui.actionAboutAtlas.triggered.connect(self.on_actionAboutAtlas_triggered)
        #self.ui.actionAboutQt.triggered.connect(self.on_actionAboutQtDialog_triggered)
        #Need code for setting application font
        #Notification window

        #self.ui.recordView.model().setHeaderData(0, "Horizontal", "Games",2)

    def on_actionBulkImporter_triggered(self):
        window = BatchImporter(self)
        window.show()

    def on_actionAboutAtlas_triggered(self):
        window = AboutAtlas(self)
        window.show()

    #def on_actionAboutQtDialog_triggered(self):
    #    window = AboutQt(self)
    #    window.show()
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget
    widget.show()
    sys.exit(app.exec())
