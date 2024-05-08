# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.7.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QAbstractItemView, QApplication, QFrame, QGridLayout,
    QHBoxLayout, QHeaderView, QLabel, QLineEdit,
    QListView, QMainWindow, QMenu, QMenuBar,
    QPushButton, QScrollArea, QSizePolicy, QSpacerItem,
    QStackedWidget, QStatusBar, QToolButton, QTreeView,
    QWidget)

from ui.widgets.games.ui_GameWidget import Ui_GameWidget
import rc_resources

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.setWindowModality(Qt.NonModal)
        MainWindow.resize(1043, 788)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setMinimumSize(QSize(820, 600))
        MainWindow.setStyleSheet(u"")
        self.actionManage = QAction(MainWindow)
        self.actionManage.setObjectName(u"actionManage")
        self.actionOptions = QAction(MainWindow)
        self.actionOptions.setObjectName(u"actionOptions")
        self.actionOptions.setShortcutContext(Qt.WidgetShortcut)
        self.actionExit = QAction(MainWindow)
        self.actionExit.setObjectName(u"actionExit")
        self.actionListView = QAction(MainWindow)
        self.actionListView.setObjectName(u"actionListView")
        self.actionBannerView = QAction(MainWindow)
        self.actionBannerView.setObjectName(u"actionBannerView")
        self.actionCoverView = QAction(MainWindow)
        self.actionCoverView.setObjectName(u"actionCoverView")
        self.actionArrangeBy = QAction(MainWindow)
        self.actionArrangeBy.setObjectName(u"actionArrangeBy")
        self.actionUpdates = QAction(MainWindow)
        self.actionUpdates.setObjectName(u"actionUpdates")
        self.actionAboutAtlas = QAction(MainWindow)
        self.actionAboutAtlas.setObjectName(u"actionAboutAtlas")
        self.actionAboutQt = QAction(MainWindow)
        self.actionAboutQt.setObjectName(u"actionAboutQt")
        self.actionViewFileHistory = QAction(MainWindow)
        self.actionViewFileHistory.setObjectName(u"actionViewFileHistory")
        self.actionSimpleImporter = QAction(MainWindow)
        self.actionSimpleImporter.setObjectName(u"actionSimpleImporter")
        self.actionBulkImporter = QAction(MainWindow)
        self.actionBulkImporter.setObjectName(u"actionBulkImporter")
        self.actionGameListImporter = QAction(MainWindow)
        self.actionGameListImporter.setObjectName(u"actionGameListImporter")
        self.actionSingleImporter = QAction(MainWindow)
        self.actionSingleImporter.setObjectName(u"actionSingleImporter")
        self.actionExtractionImporter = QAction(MainWindow)
        self.actionExtractionImporter.setObjectName(u"actionExtractionImporter")
        self.actionShowConsole = QAction(MainWindow)
        self.actionShowConsole.setObjectName(u"actionShowConsole")
        self.actionConsoleWindow = QAction(MainWindow)
        self.actionConsoleWindow.setObjectName(u"actionConsoleWindow")
        self.actionUpdateMeta = QAction(MainWindow)
        self.actionUpdateMeta.setObjectName(u"actionUpdateMeta")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.centralwidget.sizePolicy().hasHeightForWidth())
        self.centralwidget.setSizePolicy(sizePolicy1)
        self.centralwidget.setStyleSheet(u"")
        self.gridLayout = QGridLayout(self.centralwidget)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.NavFrame = QFrame(self.centralwidget)
        self.NavFrame.setObjectName(u"NavFrame")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Preferred)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.NavFrame.sizePolicy().hasHeightForWidth())
        self.NavFrame.setSizePolicy(sizePolicy2)
        self.NavFrame.setMinimumSize(QSize(250, 0))
        self.NavFrame.setMaximumSize(QSize(250, 16777215))
        self.NavFrame.setStyleSheet(u"")
        self.NavFrame.setFrameShape(QFrame.NoFrame)
        self.NavFrame.setFrameShadow(QFrame.Plain)
        self.NavFrame.setLineWidth(0)
        self.gridLayout_4 = QGridLayout(self.NavFrame)
        self.gridLayout_4.setSpacing(0)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.gridLayout_4.setContentsMargins(0, -1, 0, 0)
        self.verticalSpacer = QSpacerItem(20, 4, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)

        self.gridLayout_4.addItem(self.verticalSpacer, 3, 1, 1, 1)

        self.HomeFrame = QFrame(self.NavFrame)
        self.HomeFrame.setObjectName(u"HomeFrame")
        self.HomeFrame.setStyleSheet(u"border:0px solid black;")
        self.gridLayout_7 = QGridLayout(self.HomeFrame)
        self.gridLayout_7.setSpacing(0)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.gridLayout_7.setContentsMargins(0, 0, 0, 0)
        self.SearchBox = QLineEdit(self.HomeFrame)
        self.SearchBox.setObjectName(u"SearchBox")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Fixed)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.SearchBox.sizePolicy().hasHeightForWidth())
        self.SearchBox.setSizePolicy(sizePolicy3)
        self.SearchBox.setMinimumSize(QSize(50, 32))
        font = QFont()
        font.setPointSize(12)
        self.SearchBox.setFont(font)
        self.SearchBox.setStyleSheet(u"background-color: rgba(0, 0, 0,40);\n"
"border:none;\n"
"border-top-right-radius:0px;\n"
"border-bottom-right-radius:0px;\n"
"border-bottom-left-radius:0px;\n"
"border-top-left-radius:0px;")
        self.SearchBox.setFrame(False)

        self.gridLayout_7.addWidget(self.SearchBox, 0, 2, 1, 1)

        self.SearchIcon = QLabel(self.HomeFrame)
        self.SearchIcon.setObjectName(u"SearchIcon")
        sizePolicy2.setHeightForWidth(self.SearchIcon.sizePolicy().hasHeightForWidth())
        self.SearchIcon.setSizePolicy(sizePolicy2)
        self.SearchIcon.setMinimumSize(QSize(36, 0))
        self.SearchIcon.setMaximumSize(QSize(32, 32))
        self.SearchIcon.setAutoFillBackground(False)
        self.SearchIcon.setStyleSheet(u"background-color: rgba(0, 0, 0,40);\n"
"border:none;\n"
"margin-left:4px;\n"
"border-top-left-radius:5px;\n"
"border-bottom-left-radius:5px;")
        self.SearchIcon.setLineWidth(0)
        self.SearchIcon.setPixmap(QPixmap(u":/images/assets/search.svg"))
        self.SearchIcon.setScaledContents(True)
        self.SearchIcon.setMargin(6)
        self.SearchIcon.setIndent(-1)

        self.gridLayout_7.addWidget(self.SearchIcon, 0, 1, 1, 1)

        self.homeButton = QToolButton(self.HomeFrame)
        self.homeButton.setObjectName(u"homeButton")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy4.setHorizontalStretch(0)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.homeButton.sizePolicy().hasHeightForWidth())
        self.homeButton.setSizePolicy(sizePolicy4)
        self.homeButton.setMinimumSize(QSize(40, 40))
        font1 = QFont()
        font1.setFamilies([u"Segoe UI"])
        font1.setPointSize(19)
        self.homeButton.setFont(font1)
        self.homeButton.setStyleSheet(u"border-radius:none;\n"
"border:0px solid black;\n"
"background-color: transparent;\n"
"margin-left: 2px;")
        icon = QIcon()
        icon.addFile(u":/images/assets/Atlas_logo_v2.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.homeButton.setIcon(icon)
        self.homeButton.setIconSize(QSize(40, 40))
        self.homeButton.setToolButtonStyle(Qt.ToolButtonIconOnly)

        self.gridLayout_7.addWidget(self.homeButton, 0, 0, 1, 1)

        self.btnFilter = QToolButton(self.HomeFrame)
        self.btnFilter.setObjectName(u"btnFilter")
        self.btnFilter.setMinimumSize(QSize(32, 32))
        self.btnFilter.setStyleSheet(u"background-color: rgba(0, 0, 0,40);\n"
"border:none;\n"
"border-top-right-radius:5px;\n"
"border-bottom-right-radius:5px;\n"
"border-bottom-left-radius:0px;\n"
"border-top-left-radius:0px;")
        icon1 = QIcon()
        icon1.addFile(u":/images/assets/setting_filter.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnFilter.setIcon(icon1)
        self.btnFilter.setIconSize(QSize(25, 25))

        self.gridLayout_7.addWidget(self.btnFilter, 0, 3, 1, 1)

        self.horizontalSpacer_4 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_7.addItem(self.horizontalSpacer_4, 0, 4, 1, 1)


        self.gridLayout_4.addWidget(self.HomeFrame, 2, 1, 1, 1)

        self.gamesTree = QTreeView(self.NavFrame)
        self.gamesTree.setObjectName(u"gamesTree")
        self.gamesTree.setStyleSheet(u"")

        self.gridLayout_4.addWidget(self.gamesTree, 4, 0, 1, 3)


        self.gridLayout.addWidget(self.NavFrame, 2, 0, 1, 1)

        self.StatusFrame = QFrame(self.centralwidget)
        self.StatusFrame.setObjectName(u"StatusFrame")
        self.StatusFrame.setMinimumSize(QSize(0, 36))
        self.StatusFrame.setMaximumSize(QSize(16777215, 36))
        self.StatusFrame.setFrameShape(QFrame.NoFrame)
        self.StatusFrame.setFrameShadow(QFrame.Plain)
        self.horizontalLayout = QHBoxLayout(self.StatusFrame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.btnAddGame = QPushButton(self.StatusFrame)
        self.btnAddGame.setObjectName(u"btnAddGame")
        sizePolicy5 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)
        sizePolicy5.setHorizontalStretch(0)
        sizePolicy5.setVerticalStretch(0)
        sizePolicy5.setHeightForWidth(self.btnAddGame.sizePolicy().hasHeightForWidth())
        self.btnAddGame.setSizePolicy(sizePolicy5)
        self.btnAddGame.setMinimumSize(QSize(100, 25))
        self.btnAddGame.setMaximumSize(QSize(100, 16777215))
        icon2 = QIcon()
        icon2.addFile(u":/images/assets/addgame.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.btnAddGame.setIcon(icon2)

        self.horizontalLayout.addWidget(self.btnAddGame)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer_2)

        self.GamesInstalled = QLabel(self.StatusFrame)
        self.GamesInstalled.setObjectName(u"GamesInstalled")

        self.horizontalLayout.addWidget(self.GamesInstalled)

        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer_3)

        self.btnLog = QPushButton(self.StatusFrame)
        self.btnLog.setObjectName(u"btnLog")
        self.btnLog.setMinimumSize(QSize(100, 25))

        self.horizontalLayout.addWidget(self.btnLog)


        self.gridLayout.addWidget(self.StatusFrame, 3, 0, 1, 4)

        self.NavTop = QWidget(self.centralwidget)
        self.NavTop.setObjectName(u"NavTop")
        self.NavTop.setMinimumSize(QSize(0, 30))
        self.NavTop.setMaximumSize(QSize(16777215, 40))
        self.NavTop.setStyleSheet(u"background-color:rgba(50,50,50,100);")
        self.gridLayout_6 = QGridLayout(self.NavTop)
        self.gridLayout_6.setSpacing(0)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.gridLayout_6.setContentsMargins(0, 0, 0, 0)
        self.homeButton_top = QToolButton(self.NavTop)
        self.homeButton_top.setObjectName(u"homeButton_top")
        sizePolicy4.setHeightForWidth(self.homeButton_top.sizePolicy().hasHeightForWidth())
        self.homeButton_top.setSizePolicy(sizePolicy4)
        self.homeButton_top.setMinimumSize(QSize(40, 40))
        self.homeButton_top.setFont(font1)
        self.homeButton_top.setStyleSheet(u"border-radius:none;\n"
"background-color: transparent;\n"
"margin-left: 2px;")
        self.homeButton_top.setIcon(icon)
        self.homeButton_top.setIconSize(QSize(40, 40))
        self.homeButton_top.setToolButtonStyle(Qt.ToolButtonIconOnly)

        self.gridLayout_6.addWidget(self.homeButton_top, 0, 0, 1, 1)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_6.addItem(self.horizontalSpacer, 0, 3, 1, 1)

        self.label_2 = QLabel(self.NavTop)
        self.label_2.setObjectName(u"label_2")
        sizePolicy2.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy2)
        self.label_2.setMinimumSize(QSize(36, 0))
        self.label_2.setMaximumSize(QSize(32, 32))
        self.label_2.setStyleSheet(u"background-color: rgba(0, 0, 0,40);\n"
"border:none;\n"
"margin-left:4px;\n"
"border-top-left-radius:5px;\n"
"border-bottom-left-radius:5px;")
        self.label_2.setLineWidth(0)
        self.label_2.setPixmap(QPixmap(u":/images/assets/search.svg"))
        self.label_2.setScaledContents(True)
        self.label_2.setMargin(6)

        self.gridLayout_6.addWidget(self.label_2, 0, 1, 1, 1)

        self.lineEdit = QLineEdit(self.NavTop)
        self.lineEdit.setObjectName(u"lineEdit")
        self.lineEdit.setMinimumSize(QSize(101, 32))
        self.lineEdit.setMaximumSize(QSize(171, 32))
        self.lineEdit.setStyleSheet(u"background-color: rgba(0, 0, 0,40);\n"
"border:none;\n"
"border-top-right-radius:5px;\n"
"border-bottom-right-radius:5px;\n"
"border-bottom-left-radius:0px;\n"
"border-top-left-radius:0px;\n"
"margin-right:5px;")

        self.gridLayout_6.addWidget(self.lineEdit, 0, 2, 1, 1)


        self.gridLayout.addWidget(self.NavTop, 0, 0, 1, 3)

        self.stackedWidget = QStackedWidget(self.centralwidget)
        self.stackedWidget.setObjectName(u"stackedWidget")
        sizePolicy1.setHeightForWidth(self.stackedWidget.sizePolicy().hasHeightForWidth())
        self.stackedWidget.setSizePolicy(sizePolicy1)
        self.bv = QWidget()
        self.bv.setObjectName(u"bv")
        sizePolicy1.setHeightForWidth(self.bv.sizePolicy().hasHeightForWidth())
        self.bv.setSizePolicy(sizePolicy1)
        self.bv.setStyleSheet(u"")
        self.gridLayout_5 = QGridLayout(self.bv)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setVerticalSpacing(0)
        self.gridLayout_5.setContentsMargins(0, 0, 0, 0)
        self.recordView = QListView(self.bv)
        self.recordView.setObjectName(u"recordView")
        self.recordView.setStyleSheet(u"")
        self.recordView.setAutoScroll(False)
        self.recordView.setAutoScrollMargin(10)
        self.recordView.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel)
        self.recordView.setHorizontalScrollMode(QAbstractItemView.ScrollPerPixel)
        self.recordView.setMovement(QListView.Free)
        self.recordView.setFlow(QListView.LeftToRight)
        self.recordView.setProperty("isWrapping", True)
        self.recordView.setResizeMode(QListView.Adjust)
        self.recordView.setLayoutMode(QListView.SinglePass)
        self.recordView.setUniformItemSizes(True)
        self.recordView.setBatchSize(100)

        self.gridLayout_5.addWidget(self.recordView, 0, 0, 1, 1)

        self.stackedWidget.addWidget(self.bv)
        self.cv = QWidget()
        self.cv.setObjectName(u"cv")
        self.gridLayout_2 = QGridLayout(self.cv)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.scrollArea = QScrollArea(self.cv)
        self.scrollArea.setObjectName(u"scrollArea")
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents = QWidget()
        self.scrollAreaWidgetContents.setObjectName(u"scrollAreaWidgetContents")
        self.scrollAreaWidgetContents.setGeometry(QRect(0, 0, 791, 667))
        self.gridLayout_3 = QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.gridLayout_3.setHorizontalSpacing(0)
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.detailedRecordView = Ui_GameWidget(self.scrollAreaWidgetContents)
        self.detailedRecordView.setObjectName(u"detailedRecordView")

        self.gridLayout_3.addWidget(self.detailedRecordView, 0, 0, 1, 1)

        self.scrollArea.setWidget(self.scrollAreaWidgetContents)

        self.gridLayout_2.addWidget(self.scrollArea, 0, 0, 1, 1)

        self.stackedWidget.addWidget(self.cv)

        self.gridLayout.addWidget(self.stackedWidget, 2, 2, 1, 1)

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusBar = QStatusBar(MainWindow)
        self.statusBar.setObjectName(u"statusBar")
        self.statusBar.setSizeGripEnabled(True)
        MainWindow.setStatusBar(self.statusBar)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1043, 22))
        self.menubar.setAutoFillBackground(False)
        self.menubar.setStyleSheet(u"")
        self.menuFile = QMenu(self.menubar)
        self.menuFile.setObjectName(u"menuFile")
        self.menuFile.setStyleSheet(u"")
        self.menuView = QMenu(self.menubar)
        self.menuView.setObjectName(u"menuView")
        self.menuGames = QMenu(self.menubar)
        self.menuGames.setObjectName(u"menuGames")
        self.menuImport = QMenu(self.menuGames)
        self.menuImport.setObjectName(u"menuImport")
        self.menuDownload = QMenu(self.menuGames)
        self.menuDownload.setObjectName(u"menuDownload")
        self.menuHelp = QMenu(self.menubar)
        self.menuHelp.setObjectName(u"menuHelp")
        MainWindow.setMenuBar(self.menubar)

        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuView.menuAction())
        self.menubar.addAction(self.menuGames.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())
        self.menuFile.addAction(self.actionExit)
        self.menuView.addAction(self.actionListView)
        self.menuView.addAction(self.actionBannerView)
        self.menuView.addAction(self.actionCoverView)
        self.menuView.addSeparator()
        self.menuView.addAction(self.actionArrangeBy)
        self.menuGames.addAction(self.menuImport.menuAction())
        self.menuGames.addAction(self.actionManage)
        self.menuGames.addAction(self.menuDownload.menuAction())
        self.menuGames.addAction(self.actionOptions)
        self.menuGames.addAction(self.actionConsoleWindow)
        self.menuImport.addAction(self.actionSimpleImporter)
        self.menuImport.addAction(self.actionBulkImporter)
        self.menuImport.addAction(self.actionGameListImporter)
        self.menuImport.addAction(self.actionSingleImporter)
        self.menuImport.addAction(self.actionExtractionImporter)
        self.menuDownload.addAction(self.actionUpdateMeta)
        self.menuHelp.addSeparator()
        self.menuHelp.addSeparator()
        self.menuHelp.addAction(self.actionUpdates)
        self.menuHelp.addSeparator()
        self.menuHelp.addAction(self.actionAboutAtlas)
        self.menuHelp.addAction(self.actionAboutQt)
        self.menuHelp.addAction(self.actionViewFileHistory)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"ATLAS", None))
        self.actionManage.setText(QCoreApplication.translate("MainWindow", u"Mange", None))
        self.actionOptions.setText(QCoreApplication.translate("MainWindow", u"Options", None))
        self.actionExit.setText(QCoreApplication.translate("MainWindow", u"Exit", None))
        self.actionListView.setText(QCoreApplication.translate("MainWindow", u"List View", None))
        self.actionBannerView.setText(QCoreApplication.translate("MainWindow", u"Banner View", None))
        self.actionCoverView.setText(QCoreApplication.translate("MainWindow", u"Cover View", None))
        self.actionArrangeBy.setText(QCoreApplication.translate("MainWindow", u"Arrange By", None))
        self.actionUpdates.setText(QCoreApplication.translate("MainWindow", u"Check for Updates", None))
        self.actionAboutAtlas.setText(QCoreApplication.translate("MainWindow", u"About Atlas", None))
        self.actionAboutQt.setText(QCoreApplication.translate("MainWindow", u"About Qt", None))
        self.actionViewFileHistory.setText(QCoreApplication.translate("MainWindow", u"View file history", None))
        self.actionSimpleImporter.setText(QCoreApplication.translate("MainWindow", u"Simple", None))
        self.actionBulkImporter.setText(QCoreApplication.translate("MainWindow", u"Bulk Wizard", None))
        self.actionGameListImporter.setText(QCoreApplication.translate("MainWindow", u"GameList", None))
        self.actionSingleImporter.setText(QCoreApplication.translate("MainWindow", u"Single", None))
        self.actionExtractionImporter.setText(QCoreApplication.translate("MainWindow", u"Extraction Wizard", None))
        self.actionShowConsole.setText(QCoreApplication.translate("MainWindow", u"Show", None))
        self.actionConsoleWindow.setText(QCoreApplication.translate("MainWindow", u"Console Window", None))
        self.actionUpdateMeta.setText(QCoreApplication.translate("MainWindow", u"Update game metadata", None))
        self.SearchBox.setText("")
        self.SearchIcon.setText("")
        self.homeButton.setText("")
        self.btnFilter.setText(QCoreApplication.translate("MainWindow", u"...", None))
        self.btnAddGame.setText(QCoreApplication.translate("MainWindow", u"Add Game", None))
        self.GamesInstalled.setText(QCoreApplication.translate("MainWindow", u"0 Games Installed", None))
        self.btnLog.setText(QCoreApplication.translate("MainWindow", u"Show Log", None))
        self.homeButton_top.setText("")
        self.label_2.setText("")
        self.menuFile.setTitle(QCoreApplication.translate("MainWindow", u"File", None))
        self.menuView.setTitle(QCoreApplication.translate("MainWindow", u"View", None))
        self.menuGames.setTitle(QCoreApplication.translate("MainWindow", u"Tools", None))
        self.menuImport.setTitle(QCoreApplication.translate("MainWindow", u"Import", None))
        self.menuDownload.setTitle(QCoreApplication.translate("MainWindow", u"Download", None))
        self.menuHelp.setTitle(QCoreApplication.translate("MainWindow", u"Help", None))
    # retranslateUi

