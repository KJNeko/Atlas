# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'AddGameDialog.ui'
##
## Created by: Qt User Interface Compiler version 6.7.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QDialog, QLabel, QSizePolicy,
    QWidget)

class Ui_AddGameDialog(object):
    def setupUi(self, AddGameDialog):
        if not AddGameDialog.objectName():
            AddGameDialog.setObjectName(u"AddGameDialog")
        AddGameDialog.resize(400, 300)
        self.label = QLabel(AddGameDialog)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(120, 130, 63, 20))

        self.retranslateUi(AddGameDialog)

        QMetaObject.connectSlotsByName(AddGameDialog)
    # setupUi

    def retranslateUi(self, AddGameDialog):
        AddGameDialog.setWindowTitle(QCoreApplication.translate("AddGameDialog", u"Dialog", None))
        self.label.setText(QCoreApplication.translate("AddGameDialog", u"TextLabel", None))
    # retranslateUi

