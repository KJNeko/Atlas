# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtWidgets import QDialog

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from atlas.ui.dialog.ui_AboutAtlas import Ui_AboutAtlas

class AboutAtlas(QDialog, Ui_AboutAtlas):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.setupUi(self)
