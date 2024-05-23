# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtWidgets import QDialog

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from atlas.ui.importer.ui_BatchImportDialog import Ui_BatchImportDialog

class BatchImporter(QDialog, Ui_BatchImportDialog):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.setupUi(self)
