# This Python file uses the following encoding: utf-8
import sys
import os
from PySide6.QtCore import QFile
from PySide6.QtWidgets import QDialog, QFileDialog
from atlas.core.logger import *
from atlas.core.scanner.game_scanner import *
from atlas.core.utilities.threading import *

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from atlas.ui.importer.ui_BatchImportDialog import Ui_BatchImportDialog

class BatchImporter(QDialog, Ui_BatchImportDialog):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.ui = Ui_BatchImportDialog()
        self.ui.setupUi(self)      
        
        self.ui.progressBar.hide()
        self.ui.btnBack.hide()
        self.ui.cbMoveImported.hide()
        self.ui.cbScanFilesize.hide()

        #Custom Code
        #self.ui.tbPath.setText("F:/Games")
        self.ui.cbSkipRegex.setChecked(True)
        
        self.ui.cbSkipRegex.checkStateChanged.connect(self.on_cbSkipRegex_state_changed)
        self.ui.btnSetFolder.pressed.connect(self.on_btnSetFolder_pressed)
        self.ui.btnNext.pressed.connect(self.on_btnNext_pressed)

        #Set up table
        self.ui.twGames.setColumnCount(6)
        self.ui.twGames.setHorizontalHeaderLabels(["Title", "Creator", "Engine", "Version", "Executable", "Folder"])

        #TODO update items from config

    def on_cbSkipRegex_state_changed(self):
        self.ui.tbFormat.setEnabled(not self.ui.tbFormat.isEnabled())
    def on_btnSetFolder_pressed(self):
        directory = QFileDialog.getExistingDirectory(self,"Select folder to add")

        if len(directory) < 0 or not QFile.exists(directory):
            return
        else:
            self.ui.tbPath.setText(directory)
    
    def on_btnNext_pressed(self):
        ui = self.ui
        if ui.btnNext.text == "Import":
            self.import_files()
        else:            
            path = ui.tbPath.text()
            if len(path) < 0 or not QFile.exists(path):
                ui.statusLabel.setText("Path not set or invalid. Please check")
                return
            if len(ui.tbFormat.text()) < 0:
                ui.statusLabel.setText("Autofill not set. Please fill out the required entries")
                return
            if not str(ui.tbFormat.text()).__contains__("{title}"):
                ui.statusLabel.setText("Autofill missing \"{title}\" which is required")
                return
            if not str(ui.tbFormat.text()).__contains__("{version}"):
                ui.statusLabel.setText("Autofill missing \"{version}\" which is required")
                return
            
            ui.swImportGames.setCurrentIndex(1)
            ui.btnBack.show()
            ui.btnNext.setDisabled(True)

            #add check for file size
            self.process_files()

    def import_files(self):
        return
    
    def process_files(self):
        ui = self.ui
        path = ui.tbPath.text()
        ui.btnNext.setEnabled( False )
        ui.btnNext.setText( "Import" )
        ui.btnBack.setHidden( False )
        if ui.cbSkipRegex.isChecked() == True:
            logger.debug("Running in Auto Mode")
            gs = game_scanner(ui, path, "", False)
            gs.run()
            #game_scanner.start(ui, path, "", False)
        else:
            logger.debug("Running in Parse Mode")

    
    def check_engine(self):
        return