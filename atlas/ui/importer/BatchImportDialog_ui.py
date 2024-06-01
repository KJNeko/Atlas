# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'BatchImportDialog.ui'
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
from PySide6.QtWidgets import (QApplication, QCheckBox, QDialog, QFrame,
    QHBoxLayout, QHeaderView, QLabel, QLineEdit,
    QProgressBar, QPushButton, QSizePolicy, QSpacerItem,
    QStackedWidget, QTableWidget, QTableWidgetItem, QVBoxLayout,
    QWidget)

class Ui_BatchImportDialog(object):
    def setupUi(self, BatchImportDialog):
        if not BatchImportDialog.objectName():
            BatchImportDialog.setObjectName(u"BatchImportDialog")
        BatchImportDialog.resize(1082, 717)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(BatchImportDialog.sizePolicy().hasHeightForWidth())
        BatchImportDialog.setSizePolicy(sizePolicy)
        BatchImportDialog.setStyleSheet(u"")
        self.verticalLayout = QVBoxLayout(BatchImportDialog)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, -1)
        self.swImportGames = QStackedWidget(BatchImportDialog)
        self.swImportGames.setObjectName(u"swImportGames")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.swImportGames.sizePolicy().hasHeightForWidth())
        self.swImportGames.setSizePolicy(sizePolicy1)
        self.page_1 = QWidget()
        self.page_1.setObjectName(u"page_1")
        self.verticalLayout_6 = QVBoxLayout(self.page_1)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.verticalLayout_6.setContentsMargins(0, 0, 0, 0)
        self.frame_2 = QFrame(self.page_1)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.verticalLayout_5 = QVBoxLayout(self.frame_2)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.frame = QFrame(self.frame_2)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.horizontalLayout = QHBoxLayout(self.frame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.label_3 = QLabel(self.frame)
        self.label_3.setObjectName(u"label_3")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy2)

        self.horizontalLayout.addWidget(self.label_3)

        self.tbPath = QLineEdit(self.frame)
        self.tbPath.setObjectName(u"tbPath")

        self.horizontalLayout.addWidget(self.tbPath)

        self.btnSetFolder = QPushButton(self.frame)
        self.btnSetFolder.setObjectName(u"btnSetFolder")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Maximum)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.btnSetFolder.sizePolicy().hasHeightForWidth())
        self.btnSetFolder.setSizePolicy(sizePolicy3)
        self.btnSetFolder.setMinimumSize(QSize(75, 25))

        self.horizontalLayout.addWidget(self.btnSetFolder)


        self.verticalLayout_5.addWidget(self.frame)

        self.frame_3 = QFrame(self.frame_2)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.frame_3)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.label = QLabel(self.frame_3)
        self.label.setObjectName(u"label")

        self.horizontalLayout_2.addWidget(self.label)

        self.tbFormat = QLineEdit(self.frame_3)
        self.tbFormat.setObjectName(u"tbFormat")

        self.horizontalLayout_2.addWidget(self.tbFormat)

        self.cbSkipRegex = QCheckBox(self.frame_3)
        self.cbSkipRegex.setObjectName(u"cbSkipRegex")

        self.horizontalLayout_2.addWidget(self.cbSkipRegex)


        self.verticalLayout_5.addWidget(self.frame_3)

        self.helpText = QFrame(self.frame_2)
        self.helpText.setObjectName(u"helpText")
        self.helpText.setFrameShape(QFrame.NoFrame)
        self.helpText.setFrameShadow(QFrame.Plain)
        self.verticalLayout_4 = QVBoxLayout(self.helpText)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.label_4 = QLabel(self.helpText)
        self.label_4.setObjectName(u"label_4")

        self.verticalLayout_4.addWidget(self.label_4)

        self.label_7 = QLabel(self.helpText)
        self.label_7.setObjectName(u"label_7")

        self.verticalLayout_4.addWidget(self.label_7)

        self.label_8 = QLabel(self.helpText)
        self.label_8.setObjectName(u"label_8")

        self.verticalLayout_4.addWidget(self.label_8)

        self.label_6 = QLabel(self.helpText)
        self.label_6.setObjectName(u"label_6")

        self.verticalLayout_4.addWidget(self.label_6)


        self.verticalLayout_5.addWidget(self.helpText)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_5.addItem(self.verticalSpacer)

        self.flags = QFrame(self.frame_2)
        self.flags.setObjectName(u"flags")
        self.flags.setFrameShape(QFrame.StyledPanel)
        self.flags.setFrameShadow(QFrame.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.flags)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.cbCheckDownloadImages = QCheckBox(self.flags)
        self.cbCheckDownloadImages.setObjectName(u"cbCheckDownloadImages")
        self.cbCheckDownloadImages.setChecked(True)

        self.verticalLayout_3.addWidget(self.cbCheckDownloadImages)

        self.cbMoveImported = QCheckBox(self.flags)
        self.cbMoveImported.setObjectName(u"cbMoveImported")
        self.cbMoveImported.setEnabled(True)

        self.verticalLayout_3.addWidget(self.cbMoveImported)

        self.cbScanFilesize = QCheckBox(self.flags)
        self.cbScanFilesize.setObjectName(u"cbScanFilesize")

        self.verticalLayout_3.addWidget(self.cbScanFilesize)


        self.verticalLayout_5.addWidget(self.flags)


        self.verticalLayout_6.addWidget(self.frame_2)

        self.swImportGames.addWidget(self.page_1)
        self.page_2 = QWidget()
        self.page_2.setObjectName(u"page_2")
        self.verticalLayout_2 = QVBoxLayout(self.page_2)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.frame_5 = QFrame(self.page_2)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setFrameShape(QFrame.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Raised)
        self.verticalLayout_7 = QVBoxLayout(self.frame_5)
        self.verticalLayout_7.setObjectName(u"verticalLayout_7")
        self.verticalLayout_7.setContentsMargins(9, 9, 9, 9)
        self.label_2 = QLabel(self.frame_5)
        self.label_2.setObjectName(u"label_2")
        font = QFont()
        font.setPointSize(14)
        self.label_2.setFont(font)

        self.verticalLayout_7.addWidget(self.label_2)

        self.label_5 = QLabel(self.frame_5)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setWordWrap(True)

        self.verticalLayout_7.addWidget(self.label_5)

        self.twGames = QTableWidget(self.frame_5)
        self.twGames.setObjectName(u"twGames")
        self.twGames.setAlternatingRowColors(True)

        self.verticalLayout_7.addWidget(self.twGames)


        self.verticalLayout_2.addWidget(self.frame_5)

        self.swImportGames.addWidget(self.page_2)

        self.verticalLayout.addWidget(self.swImportGames)

        self.widget = QWidget(BatchImportDialog)
        self.widget.setObjectName(u"widget")
        sizePolicy2.setHeightForWidth(self.widget.sizePolicy().hasHeightForWidth())
        self.widget.setSizePolicy(sizePolicy2)
        self.widget.setMinimumSize(QSize(0, 40))
        self.widget.setMaximumSize(QSize(16777215, 60))
        self.horizontalLayout_3 = QHBoxLayout(self.widget)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.statusLabel = QLabel(self.widget)
        self.statusLabel.setObjectName(u"statusLabel")

        self.horizontalLayout_3.addWidget(self.statusLabel)

        self.progressBar = QProgressBar(self.widget)
        self.progressBar.setObjectName(u"progressBar")
        self.progressBar.setEnabled(True)
        self.progressBar.setValue(0)

        self.horizontalLayout_3.addWidget(self.progressBar)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer)

        self.btnBack = QPushButton(self.widget)
        self.btnBack.setObjectName(u"btnBack")
        self.btnBack.setEnabled(False)
        self.btnBack.setMinimumSize(QSize(75, 30))

        self.horizontalLayout_3.addWidget(self.btnBack)

        self.btnNext = QPushButton(self.widget)
        self.btnNext.setObjectName(u"btnNext")
        self.btnNext.setMinimumSize(QSize(75, 30))

        self.horizontalLayout_3.addWidget(self.btnNext)

        self.btnCancel = QPushButton(self.widget)
        self.btnCancel.setObjectName(u"btnCancel")
        self.btnCancel.setMinimumSize(QSize(75, 30))

        self.horizontalLayout_3.addWidget(self.btnCancel)


        self.verticalLayout.addWidget(self.widget)


        self.retranslateUi(BatchImportDialog)

        QMetaObject.connectSlotsByName(BatchImportDialog)
    # setupUi

    def retranslateUi(self, BatchImportDialog):
        BatchImportDialog.setWindowTitle(QCoreApplication.translate("BatchImportDialog", u"Import Games Wizard", None))
        self.label_3.setText(QCoreApplication.translate("BatchImportDialog", u"Root Path:", None))
        self.btnSetFolder.setText(QCoreApplication.translate("BatchImportDialog", u"Set Folder", None))
        self.label.setText(QCoreApplication.translate("BatchImportDialog", u"Autofill from path:", None))
        self.tbFormat.setText(QCoreApplication.translate("BatchImportDialog", u"{creator}/{title}/{version}", None))
        self.cbSkipRegex.setText(QCoreApplication.translate("BatchImportDialog", u"Auto Mode", None))
        self.label_4.setText(QCoreApplication.translate("BatchImportDialog", u"Valid options: title, creator, version. All must be surrounded with braces. Example {title}", None))
        self.label_7.setText(QCoreApplication.translate("BatchImportDialog", u"You can use various characters to indicate a directory. You can see examples below or more advanced ones in the docs (LINK HERE)", None))
        self.label_8.setText(QCoreApplication.translate("BatchImportDialog", u"Examples:\n"
"somecreator/somegame-v1.0: {creator}/{title}-{version}\n"
"somecreator/somegame/v1.0: {creator}/{title}/{version}", None))
        self.label_6.setText(QCoreApplication.translate("BatchImportDialog", u"Note that you can also use Auto Mode. It will attempt to find games based on engine structure", None))
        self.cbCheckDownloadImages.setText(QCoreApplication.translate("BatchImportDialog", u"Download Images if match found in Atlas Database (recommended)", None))
        self.cbMoveImported.setText(QCoreApplication.translate("BatchImportDialog", u"Move to game folder (Will delete after!)", None))
        self.cbScanFilesize.setText(QCoreApplication.translate("BatchImportDialog", u"Scan filesize during import", None))
        self.label_2.setText(QCoreApplication.translate("BatchImportDialog", u"Ready to Import", None))
        self.label_5.setText(QCoreApplication.translate("BatchImportDialog", u"This wizard is ready to import the games below. Please review the list to confirm that the correct games will be imported,and make any necessary changes. Change the game names in the Name column or remove items with the Delete key\n"
"Hover over the column headers for help text", None))
        self.statusLabel.setText("")
        self.btnBack.setText(QCoreApplication.translate("BatchImportDialog", u"Back", None))
        self.btnNext.setText(QCoreApplication.translate("BatchImportDialog", u"Next", None))
        self.btnCancel.setText(QCoreApplication.translate("BatchImportDialog", u"Cancel", None))
    # retranslateUi

