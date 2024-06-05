# This Python file uses the following encoding: utf-8
import sys

#from atlas.core.logger import *
from PySide6.QtCore import QCoreApplication, QAbstractItemModel
from PySide6.QtWidgets import QApplication, QMainWindow, QDialog, QProgressBar, QLabel
from atlas.ui.importer.BatchImporter import BatchImporter
from atlas.ui.dialog.AboutAtlas import AboutAtlas
from atlas.ui.delegates.record_banner_delegate import *
from atlas.ui.models.record_list_model import *

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
        self.ui.actionOptions.setVisible(False)
        self.ui.actionManage.setVisible(False)
        self.ui.menuView.setEnabled(False)
        self.ui.menuHelp.setEnabled(False)
        self.ui.menuDownload.setEnabled(False)
        self.ui.actionGameListImporter.setVisible(False)
        self.ui.actionSimpleImporter.setVisible(False)
        self.ui.actionSingleImporter.setVisible(False)
        self.ui.actionExtractionImporter.setVisible(False)
        self.ui.actionShowConsole.setVisible(False)
        self.ui.actionBulkImporter.triggered.connect(self.on_actionBulkImporter_triggered)
        self.ui.actionAboutAtlas.triggered.connect(self.on_actionAboutAtlas_triggered)
        self.ui.actionExit.triggered.connect(QCoreApplication.quit)
        #self.ui.actionAboutQt.triggered.connect(self.on_actionAboutQtDialog_triggered)
        #Need code for setting application font
        #Notification window
        #self.ui.recordView.model().setHeaderData(0, "Horizontal", "Games",2)

        #Setup StatusBar
        self.progress_bar = QProgressBar()
        self.status_text = QLabel()
        self.status_text.setMinimumWidth(100)
        self.ui.statusBar.addWidget(self.status_text, 1)
        #self.status_text.setText("This is a test")
        self.ui.statusBar.addWidget(self.progress_bar, 2)
        self.progress_bar.setGeometry(0, 0, 50, 5)
        self.progress_bar.setValue(0)
        self.progress_bar.hide()

        #Setup QListView with model
        model = RecordListModel()
        self.ui.recordView.setItemDelegate(RecordBannerDelegate())
        self.ui.recordView.setModel(model)

    def on_actionBulkImporter_triggered(self):
        window = BatchImporter(self)
        window.worker.signals.initprogress.connect(self.set_progress_max)
        window.worker.signals.progress.connect(self.update_progress)
        window.show()

    def on_actionAboutAtlas_triggered(self):
        window = AboutAtlas(self)
        window.show()
    
    def set_progress_max(self, s:int):
        self.progress_bar.show()
        self.progress_bar.setMaximum(s)
    
    def update_progress(self, s: int):
        self.progress_bar.setValue(s)
        
    #def on_actionAboutQtDialog_triggered(self):
    #    window = AboutQt(self)
    #    window.show()
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget
    widget.show()
    sys.exit(app.exec())
