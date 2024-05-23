# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'ProgressBarDialog.ui'
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
from PySide6.QtWidgets import (QApplication, QDialog, QLabel, QProgressBar,
    QSizePolicy, QVBoxLayout, QWidget)

class Ui_ProgressBarDialog(object):
    def setupUi(self, ProgressBarDialog):
        if not ProgressBarDialog.objectName():
            ProgressBarDialog.setObjectName(u"ProgressBarDialog")
        ProgressBarDialog.resize(1204, 124)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(ProgressBarDialog.sizePolicy().hasHeightForWidth())
        ProgressBarDialog.setSizePolicy(sizePolicy)
        ProgressBarDialog.setMaximumSize(QSize(16777215, 16777215))
        self.verticalLayout = QVBoxLayout(ProgressBarDialog)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.progressLabel = QLabel(ProgressBarDialog)
        self.progressLabel.setObjectName(u"progressLabel")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.progressLabel.sizePolicy().hasHeightForWidth())
        self.progressLabel.setSizePolicy(sizePolicy1)
        self.progressLabel.setAlignment(Qt.AlignCenter)

        self.verticalLayout.addWidget(self.progressLabel)

        self.progressBar = QProgressBar(ProgressBarDialog)
        self.progressBar.setObjectName(u"progressBar")
        self.progressBar.setValue(0)

        self.verticalLayout.addWidget(self.progressBar)

        self.subProgressBar = QProgressBar(ProgressBarDialog)
        self.subProgressBar.setObjectName(u"subProgressBar")
        self.subProgressBar.setValue(0)

        self.verticalLayout.addWidget(self.subProgressBar)

        self.subProgressLabel = QLabel(ProgressBarDialog)
        self.subProgressLabel.setObjectName(u"subProgressLabel")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.subProgressLabel.sizePolicy().hasHeightForWidth())
        self.subProgressLabel.setSizePolicy(sizePolicy2)
        self.subProgressLabel.setMaximumSize(QSize(16777215, 16777215))
        self.subProgressLabel.setLayoutDirection(Qt.LeftToRight)
        self.subProgressLabel.setAlignment(Qt.AlignCenter)
        self.subProgressLabel.setWordWrap(False)

        self.verticalLayout.addWidget(self.subProgressLabel)


        self.retranslateUi(ProgressBarDialog)

        QMetaObject.connectSlotsByName(ProgressBarDialog)
    # setupUi

    def retranslateUi(self, ProgressBarDialog):
        ProgressBarDialog.setWindowTitle(QCoreApplication.translate("ProgressBarDialog", u"ProgressBarDialog", None))
        self.progressLabel.setText(QCoreApplication.translate("ProgressBarDialog", u"TextLabel", None))
        self.subProgressLabel.setText(QCoreApplication.translate("ProgressBarDialog", u"TextLabel", None))
    # retranslateUi

