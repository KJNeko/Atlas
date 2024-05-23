# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'RecordEditor.ui'
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
from PySide6.QtWidgets import (QAbstractItemView, QApplication, QDialog, QFrame,
    QGridLayout, QLabel, QLineEdit, QListView,
    QListWidget, QListWidgetItem, QPushButton, QSizePolicy,
    QSpacerItem, QSplitter, QTabWidget, QTextEdit,
    QVBoxLayout, QWidget)

from ui.views.ImageView.hpp import ImageView
from ui.views.VersionView.hpp import VersionView

class Ui_RecordEditor(object):
    def setupUi(self, RecordEditor):
        if not RecordEditor.objectName():
            RecordEditor.setObjectName(u"RecordEditor")
        RecordEditor.resize(1588, 881)
        RecordEditor.setAcceptDrops(False)
        self.gridLayout_5 = QGridLayout(RecordEditor)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setHorizontalSpacing(0)
        self.gridLayout_5.setVerticalSpacing(6)
        self.gridLayout_5.setContentsMargins(0, 9, 0, 9)
        self.tabWidget = QTabWidget(RecordEditor)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tabWidget.setTabPosition(QTabWidget.North)
        self.tabWidget.setUsesScrollButtons(False)
        self.recordTab = QWidget()
        self.recordTab.setObjectName(u"recordTab")
        self.verticalLayout_4 = QVBoxLayout(self.recordTab)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.frame_4 = QFrame(self.recordTab)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setFrameShape(QFrame.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Raised)
        self.gridLayout_3 = QGridLayout(self.frame_4)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.engineLineEdit = QLineEdit(self.frame_4)
        self.engineLineEdit.setObjectName(u"engineLineEdit")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.engineLineEdit.sizePolicy().hasHeightForWidth())
        self.engineLineEdit.setSizePolicy(sizePolicy)
        self.engineLineEdit.setReadOnly(True)

        self.gridLayout_3.addWidget(self.engineLineEdit, 2, 1, 1, 1)

        self.titleLabel = QLabel(self.frame_4)
        self.titleLabel.setObjectName(u"titleLabel")

        self.gridLayout_3.addWidget(self.titleLabel, 0, 0, 1, 1)

        self.engineLabel = QLabel(self.frame_4)
        self.engineLabel.setObjectName(u"engineLabel")

        self.gridLayout_3.addWidget(self.engineLabel, 2, 0, 1, 1)

        self.creatorLabel = QLabel(self.frame_4)
        self.creatorLabel.setObjectName(u"creatorLabel")

        self.gridLayout_3.addWidget(self.creatorLabel, 1, 0, 1, 1)

        self.creatorLineEdit = QLineEdit(self.frame_4)
        self.creatorLineEdit.setObjectName(u"creatorLineEdit")
        sizePolicy.setHeightForWidth(self.creatorLineEdit.sizePolicy().hasHeightForWidth())
        self.creatorLineEdit.setSizePolicy(sizePolicy)
        self.creatorLineEdit.setReadOnly(True)

        self.gridLayout_3.addWidget(self.creatorLineEdit, 1, 1, 1, 1)

        self.titleLineEdit = QLineEdit(self.frame_4)
        self.titleLineEdit.setObjectName(u"titleLineEdit")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy1.setHorizontalStretch(10)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.titleLineEdit.sizePolicy().hasHeightForWidth())
        self.titleLineEdit.setSizePolicy(sizePolicy1)
        self.titleLineEdit.setReadOnly(True)

        self.gridLayout_3.addWidget(self.titleLineEdit, 0, 1, 1, 1)

        self.btnChangeTitle = QPushButton(self.frame_4)
        self.btnChangeTitle.setObjectName(u"btnChangeTitle")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Fixed)
        sizePolicy2.setHorizontalStretch(1)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.btnChangeTitle.sizePolicy().hasHeightForWidth())
        self.btnChangeTitle.setSizePolicy(sizePolicy2)

        self.gridLayout_3.addWidget(self.btnChangeTitle, 0, 2, 1, 1)

        self.btnChangeCreator = QPushButton(self.frame_4)
        self.btnChangeCreator.setObjectName(u"btnChangeCreator")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Fixed)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.btnChangeCreator.sizePolicy().hasHeightForWidth())
        self.btnChangeCreator.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.btnChangeCreator, 1, 2, 1, 1)

        self.btnChangeEngine = QPushButton(self.frame_4)
        self.btnChangeEngine.setObjectName(u"btnChangeEngine")
        sizePolicy3.setHeightForWidth(self.btnChangeEngine.sizePolicy().hasHeightForWidth())
        self.btnChangeEngine.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.btnChangeEngine, 2, 2, 1, 1)


        self.verticalLayout.addWidget(self.frame_4)

        self.frame_3 = QFrame(self.recordTab)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.frame_3)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.label = QLabel(self.frame_3)
        self.label.setObjectName(u"label")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy4.setHorizontalStretch(0)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy4)

        self.verticalLayout_3.addWidget(self.label)

        self.gameText = QTextEdit(self.frame_3)
        self.gameText.setObjectName(u"gameText")

        self.verticalLayout_3.addWidget(self.gameText)

        self.btnApplyDesc = QPushButton(self.frame_3)
        self.btnApplyDesc.setObjectName(u"btnApplyDesc")

        self.verticalLayout_3.addWidget(self.btnApplyDesc)


        self.verticalLayout.addWidget(self.frame_3)


        self.verticalLayout_4.addLayout(self.verticalLayout)

        self.tabWidget.addTab(self.recordTab, "")
        self.imageTab = QWidget()
        self.imageTab.setObjectName(u"imageTab")
        self.verticalLayout_2 = QVBoxLayout(self.imageTab)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.splitter = QSplitter(self.imageTab)
        self.splitter.setObjectName(u"splitter")
        self.splitter.setOrientation(Qt.Vertical)
        self.frame = QFrame(self.splitter)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.gridLayout_2 = QGridLayout(self.frame)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.bannerPreview = QLabel(self.frame)
        self.bannerPreview.setObjectName(u"bannerPreview")
        sizePolicy5 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy5.setHorizontalStretch(0)
        sizePolicy5.setVerticalStretch(0)
        sizePolicy5.setHeightForWidth(self.bannerPreview.sizePolicy().hasHeightForWidth())
        self.bannerPreview.setSizePolicy(sizePolicy5)
        self.bannerPreview.setMinimumSize(QSize(0, 150))
        self.bannerPreview.setAcceptDrops(True)
        self.bannerPreview.setScaledContents(False)
        self.bannerPreview.setAlignment(Qt.AlignCenter)

        self.gridLayout_2.addWidget(self.bannerPreview, 2, 0, 1, 2)

        self.bannerHelpText = QLabel(self.frame)
        self.bannerHelpText.setObjectName(u"bannerHelpText")
        sizePolicy4.setHeightForWidth(self.bannerHelpText.sizePolicy().hasHeightForWidth())
        self.bannerHelpText.setSizePolicy(sizePolicy4)

        self.gridLayout_2.addWidget(self.bannerHelpText, 0, 0, 1, 2)

        self.btnSetBanner = QPushButton(self.frame)
        self.btnSetBanner.setObjectName(u"btnSetBanner")

        self.gridLayout_2.addWidget(self.btnSetBanner, 3, 0, 1, 2)

        self.splitter.addWidget(self.frame)
        self.frame_2 = QFrame(self.splitter)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.gridLayout = QGridLayout(self.frame_2)
        self.gridLayout.setObjectName(u"gridLayout")
        self.btnAddPreviews = QPushButton(self.frame_2)
        self.btnAddPreviews.setObjectName(u"btnAddPreviews")

        self.gridLayout.addWidget(self.btnAddPreviews, 2, 0, 1, 1)

        self.btnRemovePreviews = QPushButton(self.frame_2)
        self.btnRemovePreviews.setObjectName(u"btnRemovePreviews")

        self.gridLayout.addWidget(self.btnRemovePreviews, 2, 1, 1, 1)

        self.previewHelpText = QLabel(self.frame_2)
        self.previewHelpText.setObjectName(u"previewHelpText")

        self.gridLayout.addWidget(self.previewHelpText, 0, 0, 1, 2)

        self.previewList = ImageView(self.frame_2)
        self.previewList.setObjectName(u"previewList")
        self.previewList.setAcceptDrops(False)
        self.previewList.setDragEnabled(True)
        self.previewList.setDragDropOverwriteMode(False)
        self.previewList.setDragDropMode(QAbstractItemView.InternalMove)
        self.previewList.setDefaultDropAction(Qt.MoveAction)
        self.previewList.setSelectionMode(QAbstractItemView.ExtendedSelection)
        self.previewList.setFlow(QListView.LeftToRight)
        self.previewList.setProperty("isWrapping", True)
        self.previewList.setResizeMode(QListView.Adjust)
        self.previewList.setUniformItemSizes(True)
        self.previewList.setSelectionRectVisible(True)

        self.gridLayout.addWidget(self.previewList, 1, 0, 1, 2)

        self.splitter.addWidget(self.frame_2)

        self.verticalLayout_2.addWidget(self.splitter)

        self.tabWidget.addTab(self.imageTab, "")
        self.versionTab = QWidget()
        self.versionTab.setObjectName(u"versionTab")
        self.gridLayout_4 = QGridLayout(self.versionTab)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.activeVersion = VersionView(self.versionTab)
        self.activeVersion.setObjectName(u"activeVersion")

        self.gridLayout_4.addWidget(self.activeVersion, 0, 3, 1, 2)

        self.btnDeleteVersion = QPushButton(self.versionTab)
        self.btnDeleteVersion.setObjectName(u"btnDeleteVersion")

        self.gridLayout_4.addWidget(self.btnDeleteVersion, 2, 0, 1, 1)

        self.btnAddVersion = QPushButton(self.versionTab)
        self.btnAddVersion.setObjectName(u"btnAddVersion")

        self.gridLayout_4.addWidget(self.btnAddVersion, 2, 1, 1, 1)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_4.addItem(self.horizontalSpacer, 2, 3, 1, 1)

        self.versionList = QListWidget(self.versionTab)
        self.versionList.setObjectName(u"versionList")
        self.versionList.setMinimumSize(QSize(200, 0))
        self.versionList.setMaximumSize(QSize(200, 16777215))
        self.versionList.setAutoFillBackground(False)
        self.versionList.setFrameShape(QFrame.StyledPanel)

        self.gridLayout_4.addWidget(self.versionList, 0, 0, 1, 2)

        self.tabWidget.addTab(self.versionTab, "")
        self.tagTab = QWidget()
        self.tagTab.setObjectName(u"tagTab")
        self.gridLayout_6 = QGridLayout(self.tagTab)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.processedTagsList = QListWidget(self.tagTab)
        self.processedTagsList.setObjectName(u"processedTagsList")

        self.gridLayout_6.addWidget(self.processedTagsList, 1, 0, 3, 1)

        self.tagEdit = QLineEdit(self.tagTab)
        self.tagEdit.setObjectName(u"tagEdit")

        self.gridLayout_6.addWidget(self.tagEdit, 3, 1, 1, 1)

        self.label_3 = QLabel(self.tagTab)
        self.label_3.setObjectName(u"label_3")
        sizePolicy4.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy4)

        self.gridLayout_6.addWidget(self.label_3, 0, 0, 1, 1)

        self.label_2 = QLabel(self.tagTab)
        self.label_2.setObjectName(u"label_2")
        sizePolicy4.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy4)

        self.gridLayout_6.addWidget(self.label_2, 0, 1, 1, 1)

        self.userTagsList = QListWidget(self.tagTab)
        self.userTagsList.setObjectName(u"userTagsList")

        self.gridLayout_6.addWidget(self.userTagsList, 1, 1, 2, 1)

        self.tabWidget.addTab(self.tagTab, "")

        self.gridLayout_5.addWidget(self.tabWidget, 0, 0, 1, 4)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_5.addItem(self.horizontalSpacer_2, 3, 0, 1, 1)

        self.btnClose = QPushButton(RecordEditor)
        self.btnClose.setObjectName(u"btnClose")

        self.gridLayout_5.addWidget(self.btnClose, 3, 2, 1, 1)

        self.horizontalSpacer_3 = QSpacerItem(20, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_5.addItem(self.horizontalSpacer_3, 3, 3, 1, 1)

        self.verticalSpacer = QSpacerItem(20, 30, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Maximum)

        self.gridLayout_5.addItem(self.verticalSpacer, 3, 1, 1, 1)


        self.retranslateUi(RecordEditor)

        self.tabWidget.setCurrentIndex(3)


        QMetaObject.connectSlotsByName(RecordEditor)
    # setupUi

    def retranslateUi(self, RecordEditor):
        RecordEditor.setWindowTitle(QCoreApplication.translate("RecordEditor", u"RecordEditor", None))
        self.titleLabel.setText(QCoreApplication.translate("RecordEditor", u"Title:", None))
        self.engineLabel.setText(QCoreApplication.translate("RecordEditor", u"Engine:", None))
        self.creatorLabel.setText(QCoreApplication.translate("RecordEditor", u"Creator:", None))
        self.btnChangeTitle.setText(QCoreApplication.translate("RecordEditor", u"Change Title", None))
        self.btnChangeCreator.setText(QCoreApplication.translate("RecordEditor", u"Change Creator", None))
        self.btnChangeEngine.setText(QCoreApplication.translate("RecordEditor", u"Change Engine", None))
        self.label.setText(QCoreApplication.translate("RecordEditor", u"Game Description:", None))
        self.btnApplyDesc.setText(QCoreApplication.translate("RecordEditor", u"Apply Description", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.recordTab), QCoreApplication.translate("RecordEditor", u"Record Information", None))
        self.bannerPreview.setText(QCoreApplication.translate("RecordEditor", u"BANNER PLACEHOLDER", None))
        self.bannerHelpText.setText(QCoreApplication.translate("RecordEditor", u"Drag or press \"Set banner\"", None))
        self.btnSetBanner.setText(QCoreApplication.translate("RecordEditor", u"Set Banner", None))
        self.btnAddPreviews.setText(QCoreApplication.translate("RecordEditor", u"Add preview(s)", None))
        self.btnRemovePreviews.setText(QCoreApplication.translate("RecordEditor", u"Remove Preview(s)", None))
        self.previewHelpText.setText(QCoreApplication.translate("RecordEditor", u"Drag images into this window to add\n"
"Select and hit 'delete' (keyboard or physical button) to remove", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.imageTab), QCoreApplication.translate("RecordEditor", u"Banner/Previews", None))
        self.btnDeleteVersion.setText(QCoreApplication.translate("RecordEditor", u"Delete Version", None))
        self.btnAddVersion.setText(QCoreApplication.translate("RecordEditor", u"Add version", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.versionTab), QCoreApplication.translate("RecordEditor", u"Versions", None))
        self.label_3.setText(QCoreApplication.translate("RecordEditor", u"Full tags", None))
        self.label_2.setText(QCoreApplication.translate("RecordEditor", u"User tags", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tagTab), QCoreApplication.translate("RecordEditor", u"Tags", None))
        self.btnClose.setText(QCoreApplication.translate("RecordEditor", u"Close", None))
    # retranslateUi

