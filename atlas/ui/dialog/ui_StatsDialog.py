# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'StatsDialog.ui'
##
## Created by: Qt User Interface Compiler version 6.7.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCharts import QChartView
from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QDialog, QSizePolicy, QVBoxLayout,
    QWidget)

class Ui_StatsDialog(object):
    def setupUi(self, StatsDialog):
        if not StatsDialog.objectName():
            StatsDialog.setObjectName(u"StatsDialog")
        StatsDialog.resize(747, 633)
        self.verticalLayout = QVBoxLayout(StatsDialog)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.graphicsView = QChartView(StatsDialog)
        self.graphicsView.setObjectName(u"graphicsView")

        self.verticalLayout.addWidget(self.graphicsView)


        self.retranslateUi(StatsDialog)

        QMetaObject.connectSlotsByName(StatsDialog)
    # setupUi

    def retranslateUi(self, StatsDialog):
        StatsDialog.setWindowTitle(QCoreApplication.translate("StatsDialog", u"StatsDialog", None))
    # retranslateUi

