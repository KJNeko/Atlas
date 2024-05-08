# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'GameWidget.ui'
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
from PySide6.QtWidgets import (QAbstractScrollArea, QApplication, QFrame, QGridLayout,
    QLabel, QLayout, QListView, QPushButton,
    QSizePolicy, QSpacerItem, QTextEdit, QToolButton,
    QWidget)
import rc_resources

class Ui_GameWidget(object):
    def setupUi(self, GameWidget):
        if not GameWidget.objectName():
            GameWidget.setObjectName(u"GameWidget")
        GameWidget.resize(1150, 780)
        self.gridLayout_2 = QGridLayout(GameWidget)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.bannerFrame = QFrame(GameWidget)
        self.bannerFrame.setObjectName(u"bannerFrame")
        self.gridLayout = QGridLayout(self.bannerFrame)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setSizeConstraint(QLayout.SetNoConstraint)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalSpacer_8 = QSpacerItem(1118, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout.addItem(self.horizontalSpacer_8, 0, 0, 1, 1)

        self.frame = QFrame(self.bannerFrame)
        self.frame.setObjectName(u"frame")
        self.frame.setMinimumSize(QSize(0, 60))
        self.frame.setMaximumSize(QSize(16777215, 100))
        self.frame.setStyleSheet(u"background-color:rgba(0, 0, 0, 150);\n"
"border:none;")
        self.frame.setFrameShape(QFrame.NoFrame)
        self.frame.setFrameShadow(QFrame.Plain)
        self.gridLayout_6 = QGridLayout(self.frame)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.horizontalSpacer_22 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_22, 0, 3, 2, 1)

        self.tbSelectVersion = QToolButton(self.frame)
        self.tbSelectVersion.setObjectName(u"tbSelectVersion")
        self.tbSelectVersion.setMinimumSize(QSize(20, 38))
        self.tbSelectVersion.setStyleSheet(u"background-color: rgba(43, 185, 67, 255);\n"
"border-radius:none;\n"
"color: rgb(255, 255, 255);")
        icon = QIcon()
        icon.addFile(u":/images/assets/dropdownicon.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tbSelectVersion.setIcon(icon)
        self.tbSelectVersion.setIconSize(QSize(16, 12))

        self.gridLayout_6.addWidget(self.tbSelectVersion, 0, 2, 2, 1)

        self.horizontalSpacer_16 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_16, 0, 7, 2, 1)

        self.horizontalSpacer_7 = QSpacerItem(392, 36, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_7, 0, 9, 2, 1)

        self.Playtime = QWidget(self.frame)
        self.Playtime.setObjectName(u"Playtime")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.Playtime.sizePolicy().hasHeightForWidth())
        self.Playtime.setSizePolicy(sizePolicy)
        self.Playtime.setMinimumSize(QSize(100, 0))
        self.Playtime.setStyleSheet(u"background:none;")
        self.gridLayout_3 = QGridLayout(self.Playtime)
        self.gridLayout_3.setSpacing(0)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.lbTotalPlaytime = QLabel(self.Playtime)
        self.lbTotalPlaytime.setObjectName(u"lbTotalPlaytime")
        self.lbTotalPlaytime.setMinimumSize(QSize(70, 0))
        self.lbTotalPlaytime.setStyleSheet(u"background-color:none;color: rgb(255, 255, 255);")

        self.gridLayout_3.addWidget(self.lbTotalPlaytime, 1, 0, 1, 1)

        self.label = QLabel(self.Playtime)
        self.label.setObjectName(u"label")
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QSize(72, 0))
        font = QFont()
        font.setPointSize(10)
        font.setBold(True)
        self.label.setFont(font)
        self.label.setStyleSheet(u"color: rgb(255, 255, 255);")

        self.gridLayout_3.addWidget(self.label, 0, 0, 1, 1)


        self.gridLayout_6.addWidget(self.Playtime, 0, 6, 2, 1)

        self.horizontalSpacer_17 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_17, 0, 14, 2, 1)

        self.btnInfo = QPushButton(self.frame)
        self.btnInfo.setObjectName(u"btnInfo")
        self.btnInfo.setMinimumSize(QSize(30, 30))
        self.btnInfo.setMaximumSize(QSize(30, 30))
        self.btnInfo.setStyleSheet(u"background: rgba( 255, 255, 255, 50 );\n"
"border-radius:3px;")
        icon1 = QIcon()
        icon1.addFile(u":/images/assets/info.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnInfo.setIcon(icon1)
        self.btnInfo.setIconSize(QSize(24, 24))

        self.gridLayout_6.addWidget(self.btnInfo, 1, 12, 1, 1)

        self.btnPlay = QToolButton(self.frame)
        self.btnPlay.setObjectName(u"btnPlay")
        self.btnPlay.setMinimumSize(QSize(140, 38))
        font1 = QFont()
        font1.setPointSize(12)
        self.btnPlay.setFont(font1)
        self.btnPlay.setLayoutDirection(Qt.LeftToRight)
        self.btnPlay.setStyleSheet(u"background-color: qlineargradient(spread:pad, x1:1, y1:0.46, x2:0, y2:0.539636, stop:0 rgba(43, 185, 67, 255), stop:1 rgba(111, 204, 0, 255));\n"
"border-radius:none;")
        icon2 = QIcon()
        icon2.addFile(u":/images/assets/play_normal.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnPlay.setIcon(icon2)
        self.btnPlay.setIconSize(QSize(75, 30))
        self.btnPlay.setToolButtonStyle(Qt.ToolButtonIconOnly)
        self.btnPlay.setAutoRaise(False)
        self.btnPlay.setArrowType(Qt.NoArrow)

        self.gridLayout_6.addWidget(self.btnPlay, 0, 1, 2, 1)

        self.horizontalSpacer_14 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_14, 0, 5, 2, 1)

        self.horizontalSpacer_15 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer_15, 0, 0, 2, 1)

        self.LastPlayed = QWidget(self.frame)
        self.LastPlayed.setObjectName(u"LastPlayed")
        sizePolicy.setHeightForWidth(self.LastPlayed.sizePolicy().hasHeightForWidth())
        self.LastPlayed.setSizePolicy(sizePolicy)
        self.LastPlayed.setMinimumSize(QSize(130, 0))
        self.LastPlayed.setStyleSheet(u"background:none;")
        self.gridLayout_11 = QGridLayout(self.LastPlayed)
        self.gridLayout_11.setSpacing(2)
        self.gridLayout_11.setObjectName(u"gridLayout_11")
        self.gridLayout_11.setContentsMargins(2, 2, 2, 2)
        self.lbLastPlayed = QLabel(self.LastPlayed)
        self.lbLastPlayed.setObjectName(u"lbLastPlayed")
        self.lbLastPlayed.setMinimumSize(QSize(150, 0))
        self.lbLastPlayed.setStyleSheet(u"background-color:none;color: rgb(255, 255, 255);")

        self.gridLayout_11.addWidget(self.lbLastPlayed, 1, 0, 1, 1)

        self.label_16 = QLabel(self.LastPlayed)
        self.label_16.setObjectName(u"label_16")
        sizePolicy.setHeightForWidth(self.label_16.sizePolicy().hasHeightForWidth())
        self.label_16.setSizePolicy(sizePolicy)
        self.label_16.setFont(font)
        self.label_16.setStyleSheet(u"color: rgb(255, 255, 255);")

        self.gridLayout_11.addWidget(self.label_16, 0, 0, 1, 1)


        self.gridLayout_6.addWidget(self.LastPlayed, 0, 4, 2, 1)

        self.StorageUsed = QWidget(self.frame)
        self.StorageUsed.setObjectName(u"StorageUsed")
        sizePolicy.setHeightForWidth(self.StorageUsed.sizePolicy().hasHeightForWidth())
        self.StorageUsed.setSizePolicy(sizePolicy)
        self.StorageUsed.setMinimumSize(QSize(100, 0))
        self.StorageUsed.setStyleSheet(u"background:none;")
        self.gridLayout_4 = QGridLayout(self.StorageUsed)
        self.gridLayout_4.setSpacing(0)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.gridLayout_4.setContentsMargins(0, 0, 0, 0)
        self.lbStorageUsed = QLabel(self.StorageUsed)
        self.lbStorageUsed.setObjectName(u"lbStorageUsed")
        self.lbStorageUsed.setMinimumSize(QSize(90, 0))
        self.lbStorageUsed.setStyleSheet(u"background-color:none;color: rgb(255, 255, 255);")

        self.gridLayout_4.addWidget(self.lbStorageUsed, 1, 0, 1, 1)

        self.label_2 = QLabel(self.StorageUsed)
        self.label_2.setObjectName(u"label_2")
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        self.label_2.setMinimumSize(QSize(100, 0))
        self.label_2.setFont(font)
        self.label_2.setStyleSheet(u"color: rgb(255, 255, 255);")

        self.gridLayout_4.addWidget(self.label_2, 0, 0, 1, 1)


        self.gridLayout_6.addWidget(self.StorageUsed, 0, 8, 2, 1)

        self.btnManageRecord = QPushButton(self.frame)
        self.btnManageRecord.setObjectName(u"btnManageRecord")
        self.btnManageRecord.setMinimumSize(QSize(30, 30))
        self.btnManageRecord.setMaximumSize(QSize(30, 30))
        self.btnManageRecord.setStyleSheet(u"background: rgba( 255, 255, 255, 55);\n"
"             border-radius:3px;\n"
"            ")
        icon3 = QIcon()
        icon3.addFile(u":/images/assets/settings.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnManageRecord.setIcon(icon3)
        self.btnManageRecord.setIconSize(QSize(20, 20))

        self.gridLayout_6.addWidget(self.btnManageRecord, 1, 11, 1, 1)

        self.btnFavorite = QPushButton(self.frame)
        self.btnFavorite.setObjectName(u"btnFavorite")
        self.btnFavorite.setMinimumSize(QSize(30, 30))
        self.btnFavorite.setMaximumSize(QSize(30, 30))
        self.btnFavorite.setStyleSheet(u"background: rgba( 255, 255, 255, 50 );\n"
"             border-radius:3px;\n"
"            ")
        icon4 = QIcon()
        icon4.addFile(u":/images/assets/rating_icon.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnFavorite.setIcon(icon4)

        self.gridLayout_6.addWidget(self.btnFavorite, 1, 13, 1, 1)


        self.gridLayout.addWidget(self.frame, 1, 0, 1, 2)

        self.verticalSpacer_4 = QSpacerItem(20, 300, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)

        self.gridLayout.addItem(self.verticalSpacer_4, 0, 1, 1, 1)

        self.widget = QWidget(self.bannerFrame)
        self.widget.setObjectName(u"widget")

        self.gridLayout.addWidget(self.widget, 3, 0, 1, 1)

        self.bannerDetailFrame = QFrame(self.bannerFrame)
        self.bannerDetailFrame.setObjectName(u"bannerDetailFrame")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.bannerDetailFrame.sizePolicy().hasHeightForWidth())
        self.bannerDetailFrame.setSizePolicy(sizePolicy1)
        self.bannerDetailFrame.setStyleSheet(u"")
        self.gridLayout_12 = QGridLayout(self.bannerDetailFrame)
        self.gridLayout_12.setObjectName(u"gridLayout_12")
        self.InfoPanel = QWidget(self.bannerDetailFrame)
        self.InfoPanel.setObjectName(u"InfoPanel")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.InfoPanel.sizePolicy().hasHeightForWidth())
        self.InfoPanel.setSizePolicy(sizePolicy2)
        self.InfoPanel.setStyleSheet(u"border-radius: 2px;")
        self.gridLayout_8 = QGridLayout(self.InfoPanel)
        self.gridLayout_8.setSpacing(0)
        self.gridLayout_8.setObjectName(u"gridLayout_8")
        self.gridLayout_8.setContentsMargins(0, 0, 0, 0)
        self.detailView = QGridLayout()
        self.detailView.setObjectName(u"detailView")
        self.Links = QWidget(self.InfoPanel)
        self.Links.setObjectName(u"Links")
        self.Links.setMinimumSize(QSize(0, 100))
        self.Links.setStyleSheet(u"")
        self.gridLayout_13 = QGridLayout(self.Links)
        self.gridLayout_13.setObjectName(u"gridLayout_13")
        self.label_5 = QLabel(self.Links)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setStyleSheet(u"background:none;\n"
"border-style: outset;\n"
"border-width: 1px;\n"
"border-color: rgba(120, 120, 120, 100);\n"
"border-top:none;\n"
"border-right: none;\n"
"border-left:none;\n"
"padding-bottom:5px;")

        self.gridLayout_13.addWidget(self.label_5, 0, 0, 1, 1)

        self.teLinks = QTextEdit(self.Links)
        self.teLinks.setObjectName(u"teLinks")
        self.teLinks.setStyleSheet(u"")
        self.teLinks.setReadOnly(True)

        self.gridLayout_13.addWidget(self.teLinks, 1, 0, 1, 1)


        self.detailView.addWidget(self.Links, 4, 7, 1, 1)

        self.horizontalSpacer_18 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.detailView.addItem(self.horizontalSpacer_18, 4, 4, 1, 1)

        self.Info = QWidget(self.InfoPanel)
        self.Info.setObjectName(u"Info")
        self.Info.setMinimumSize(QSize(0, 100))
        self.Info.setStyleSheet(u"")
        self.gridLayout_9 = QGridLayout(self.Info)
        self.gridLayout_9.setSpacing(6)
        self.gridLayout_9.setObjectName(u"gridLayout_9")
        self.gridLayout_9.setContentsMargins(9, 9, 9, 9)
        self.label_10 = QLabel(self.Info)
        self.label_10.setObjectName(u"label_10")
        self.label_10.setStyleSheet(u"background:none;\n"
"border-style: outset;\n"
"border-width: 1px;\n"
"border-color: rgba(120, 120, 120, 100);\n"
"border-top:none;\n"
"border-right: none;\n"
"border-left:none;\n"
"padding-bottom:5px;\n"
"")

        self.gridLayout_9.addWidget(self.label_10, 0, 0, 1, 1)

        self.teDetails = QTextEdit(self.Info)
        self.teDetails.setObjectName(u"teDetails")
        self.teDetails.setStyleSheet(u"")
        self.teDetails.setReadOnly(True)

        self.gridLayout_9.addWidget(self.teDetails, 1, 0, 1, 1)


        self.detailView.addWidget(self.Info, 4, 5, 1, 1)

        self.horizontalSpacer_20 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.detailView.addItem(self.horizontalSpacer_20, 4, 0, 1, 1)

        self.horizontalSpacer_3 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.detailView.addItem(self.horizontalSpacer_3, 4, 8, 1, 1)

        self.Description = QWidget(self.InfoPanel)
        self.Description.setObjectName(u"Description")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.Description.sizePolicy().hasHeightForWidth())
        self.Description.setSizePolicy(sizePolicy3)
        self.Description.setMinimumSize(QSize(0, 100))
        self.Description.setStyleSheet(u"")
        self.gridLayout_10 = QGridLayout(self.Description)
        self.gridLayout_10.setObjectName(u"gridLayout_10")
        self.label_3 = QLabel(self.Description)
        self.label_3.setObjectName(u"label_3")
        font2 = QFont()
        font2.setUnderline(False)
        self.label_3.setFont(font2)
        self.label_3.setStyleSheet(u"background:none;\n"
"border-style: outset;\n"
"border-width: 1px;\n"
"border-color: rgba(120, 120, 120, 100);\n"
"border-top:none;\n"
"border-right: none;\n"
"border-left:none;\n"
"padding-bottom:5px;")

        self.gridLayout_10.addWidget(self.label_3, 0, 0, 1, 1)

        self.teDescription = QTextEdit(self.Description)
        self.teDescription.setObjectName(u"teDescription")
        self.teDescription.setEnabled(True)
        sizePolicy2.setHeightForWidth(self.teDescription.sizePolicy().hasHeightForWidth())
        self.teDescription.setSizePolicy(sizePolicy2)
        self.teDescription.setMinimumSize(QSize(320, 0))
        self.teDescription.setStyleSheet(u"")
        self.teDescription.setFrameShape(QFrame.NoFrame)
        self.teDescription.setFrameShadow(QFrame.Plain)
        self.teDescription.setSizeAdjustPolicy(QAbstractScrollArea.AdjustToContents)
        self.teDescription.setReadOnly(True)

        self.gridLayout_10.addWidget(self.teDescription, 1, 0, 1, 1)


        self.detailView.addWidget(self.Description, 4, 3, 1, 1)

        self.coverWidget = QWidget(self.InfoPanel)
        self.coverWidget.setObjectName(u"coverWidget")
        sizePolicy.setHeightForWidth(self.coverWidget.sizePolicy().hasHeightForWidth())
        self.coverWidget.setSizePolicy(sizePolicy)
        self.coverWidget.setMinimumSize(QSize(110, 160))
        self.coverWidget.setMaximumSize(QSize(110, 160))
        self.coverWidget.setStyleSheet(u"background:transparent;")
        self.gridLayout_7 = QGridLayout(self.coverWidget)
        self.gridLayout_7.setSpacing(0)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.gridLayout_7.setContentsMargins(0, 0, 0, 0)
        self.coverImage = QLabel(self.coverWidget)
        self.coverImage.setObjectName(u"coverImage")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy4.setHorizontalStretch(100)
        sizePolicy4.setVerticalStretch(150)
        sizePolicy4.setHeightForWidth(self.coverImage.sizePolicy().hasHeightForWidth())
        self.coverImage.setSizePolicy(sizePolicy4)
        self.coverImage.setMinimumSize(QSize(100, 150))
        self.coverImage.setStyleSheet(u"background:transparent;")

        self.gridLayout_7.addWidget(self.coverImage, 1, 0, 1, 1)


        self.detailView.addWidget(self.coverWidget, 4, 1, 1, 1)

        self.horizontalSpacer_21 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.detailView.addItem(self.horizontalSpacer_21, 4, 2, 1, 1)

        self.horizontalSpacer_2 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.detailView.addItem(self.horizontalSpacer_2, 4, 6, 1, 1)


        self.gridLayout_8.addLayout(self.detailView, 0, 0, 1, 1)


        self.gridLayout_12.addWidget(self.InfoPanel, 0, 0, 1, 2)

        self.SpaceWidget = QWidget(self.bannerDetailFrame)
        self.SpaceWidget.setObjectName(u"SpaceWidget")

        self.gridLayout_12.addWidget(self.SpaceWidget, 4, 0, 1, 1)

        self.previewList = QListView(self.bannerDetailFrame)
        self.previewList.setObjectName(u"previewList")
        self.previewList.setStyleSheet(u"border-radius: 2px;")
        self.previewList.setSizeAdjustPolicy(QAbstractScrollArea.AdjustToContents)
        self.previewList.setFlow(QListView.LeftToRight)
        self.previewList.setProperty("isWrapping", True)
        self.previewList.setResizeMode(QListView.Adjust)
        self.previewList.setSpacing(5)
        self.previewList.setViewMode(QListView.ListMode)
        self.previewList.setUniformItemSizes(True)
        self.previewList.setItemAlignment(Qt.AlignCenter)

        self.gridLayout_12.addWidget(self.previewList, 2, 0, 1, 2)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.MinimumExpanding)

        self.gridLayout_12.addItem(self.verticalSpacer, 3, 0, 1, 1)


        self.gridLayout.addWidget(self.bannerDetailFrame, 2, 0, 1, 2)


        self.gridLayout_2.addWidget(self.bannerFrame, 0, 0, 1, 1)


        self.retranslateUi(GameWidget)

        QMetaObject.connectSlotsByName(GameWidget)
    # setupUi

    def retranslateUi(self, GameWidget):
        GameWidget.setWindowTitle(QCoreApplication.translate("GameWidget", u"Form", None))
        self.tbSelectVersion.setText(QCoreApplication.translate("GameWidget", u"...", None))
        self.lbTotalPlaytime.setText(QCoreApplication.translate("GameWidget", u"3 Hours", None))
        self.label.setText(QCoreApplication.translate("GameWidget", u"PLAY TIME", None))
        self.btnInfo.setText("")
        self.btnPlay.setText(QCoreApplication.translate("GameWidget", u"   PLAY", None))
        self.lbLastPlayed.setText(QCoreApplication.translate("GameWidget", u"Jan 27 2023", None))
        self.label_16.setText(QCoreApplication.translate("GameWidget", u"LAST PLAYED", None))
        self.lbStorageUsed.setText(QCoreApplication.translate("GameWidget", u"0GB", None))
        self.label_2.setText(QCoreApplication.translate("GameWidget", u"STORAGE USED", None))
        self.btnManageRecord.setText("")
        self.btnFavorite.setText("")
        self.label_5.setText(QCoreApplication.translate("GameWidget", u"Links", None))
        self.label_10.setText(QCoreApplication.translate("GameWidget", u"Details", None))
        self.label_3.setText(QCoreApplication.translate("GameWidget", u"Description", None))
        self.coverImage.setText("")
    # retranslateUi

