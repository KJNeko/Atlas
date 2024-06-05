from PySide6.QtCore import QCoreApplication
from PySide6.QtWidgets import QApplication, QMainWindow, QDialog, QProgressBar, QLabel
from PySide6.QtWidgets import QStyledItemDelegate, QStyle

class RecordBannerDelegate(QStyledItemDelegate):

    def __init__(self, parent, listwidget):
        super(RecordBannerDelegate, self).__init__(parent)
        self.listwidget = listwidget

    def __init__(self, parent=None):
        super().__init__(parent)

    def paint(self, painter, option, index):
        return