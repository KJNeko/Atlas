# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'SettingsDialog.ui'
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
from PySide6.QtWidgets import (QAbstractItemView, QApplication, QCheckBox, QComboBox,
    QDialog, QFrame, QGridLayout, QGroupBox,
    QHBoxLayout, QLabel, QLineEdit, QListView,
    QPushButton, QSizePolicy, QSpacerItem, QSpinBox,
    QStackedWidget, QToolButton, QVBoxLayout, QWidget)

class Ui_SettingsDialog(object):
    def setupUi(self, SettingsDialog):
        if not SettingsDialog.objectName():
            SettingsDialog.setObjectName(u"SettingsDialog")
        SettingsDialog.resize(1263, 892)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(SettingsDialog.sizePolicy().hasHeightForWidth())
        SettingsDialog.setSizePolicy(sizePolicy)
        SettingsDialog.setMinimumSize(QSize(1263, 892))
        SettingsDialog.setMaximumSize(QSize(1263, 892))
        SettingsDialog.setStyleSheet(u"")
        self.gridLayout = QGridLayout(SettingsDialog)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.NavBar = QWidget(SettingsDialog)
        self.NavBar.setObjectName(u"NavBar")
        self.NavBar.setMinimumSize(QSize(0, 50))
        self.NavBar.setStyleSheet(u"QWidget#NavBar{\n"
"border-style: outset;\n"
"border-width: 1px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-left:none;\n"
"border-right:none;\n"
"border-bottom:none;}")
        self.horizontalLayout = QHBoxLayout(self.NavBar)
        self.horizontalLayout.setSpacing(6)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer)

        self.applySettings = QPushButton(self.NavBar)
        self.applySettings.setObjectName(u"applySettings")

        self.horizontalLayout.addWidget(self.applySettings)

        self.cancelSettings = QPushButton(self.NavBar)
        self.cancelSettings.setObjectName(u"cancelSettings")

        self.horizontalLayout.addWidget(self.cancelSettings)

        self.horizontalSpacer_2 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer_2)


        self.gridLayout.addWidget(self.NavBar, 4, 0, 1, 4)

        self.Header = QWidget(SettingsDialog)
        self.Header.setObjectName(u"Header")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.Header.sizePolicy().hasHeightForWidth())
        self.Header.setSizePolicy(sizePolicy1)
        self.Header.setMinimumSize(QSize(0, 35))
        self.Header.setMaximumSize(QSize(16777215, 30))
        self.gridLayout_17 = QGridLayout(self.Header)
        self.gridLayout_17.setObjectName(u"gridLayout_17")
        self.lblSettingsHeader = QLabel(self.Header)
        self.lblSettingsHeader.setObjectName(u"lblSettingsHeader")
        sizePolicy1.setHeightForWidth(self.lblSettingsHeader.sizePolicy().hasHeightForWidth())
        self.lblSettingsHeader.setSizePolicy(sizePolicy1)
        self.lblSettingsHeader.setMinimumSize(QSize(0, 15))
        font = QFont()
        font.setPointSize(10)
        font.setBold(False)
        self.lblSettingsHeader.setFont(font)

        self.gridLayout_17.addWidget(self.lblSettingsHeader, 0, 0, 1, 1)


        self.gridLayout.addWidget(self.Header, 2, 2, 1, 1)

        self.SettingsMenu = QWidget(SettingsDialog)
        self.SettingsMenu.setObjectName(u"SettingsMenu")
        self.SettingsMenu.setMinimumSize(QSize(151, 0))
        self.SettingsMenu.setStyleSheet(u"")
        self.gridLayout_5 = QGridLayout(self.SettingsMenu)
        self.gridLayout_5.setSpacing(0)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setContentsMargins(0, 0, 0, 0)
        self.lblSettings = QLabel(self.SettingsMenu)
        self.lblSettings.setObjectName(u"lblSettings")
        font1 = QFont()
        font1.setPointSize(12)
        self.lblSettings.setFont(font1)
        self.lblSettings.setAlignment(Qt.AlignCenter)

        self.gridLayout_5.addWidget(self.lblSettings, 1, 0, 1, 1)

        self.settingsButtons = QWidget(self.SettingsMenu)
        self.settingsButtons.setObjectName(u"settingsButtons")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.settingsButtons.sizePolicy().hasHeightForWidth())
        self.settingsButtons.setSizePolicy(sizePolicy2)
        self.settingsButtons.setMinimumSize(QSize(0, 30))
        self.verticalLayout = QVBoxLayout(self.settingsButtons)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, -1, 0, 0)
        self.btnGeneral = QPushButton(self.settingsButtons)
        self.btnGeneral.setObjectName(u"btnGeneral")
        self.btnGeneral.setMinimumSize(QSize(0, 30))
        self.btnGeneral.setCheckable(True)

        self.verticalLayout.addWidget(self.btnGeneral)

        self.btnInterface = QPushButton(self.settingsButtons)
        self.btnInterface.setObjectName(u"btnInterface")
        self.btnInterface.setMinimumSize(QSize(0, 30))
        self.btnInterface.setCheckable(True)

        self.verticalLayout.addWidget(self.btnInterface)

        self.btnUilayout = QPushButton(self.settingsButtons)
        self.btnUilayout.setObjectName(u"btnUilayout")
        self.btnUilayout.setMinimumSize(QSize(0, 30))
        self.btnUilayout.setCheckable(True)

        self.verticalLayout.addWidget(self.btnUilayout)

        self.btnPaths = QPushButton(self.settingsButtons)
        self.btnPaths.setObjectName(u"btnPaths")
        self.btnPaths.setMinimumSize(QSize(0, 30))
        self.btnPaths.setCheckable(True)

        self.verticalLayout.addWidget(self.btnPaths)

        self.btnThreading = QPushButton(self.settingsButtons)
        self.btnThreading.setObjectName(u"btnThreading")
        self.btnThreading.setMinimumSize(QSize(0, 30))
        self.btnThreading.setCheckable(True)

        self.verticalLayout.addWidget(self.btnThreading)

        self.btnExpFeatures = QPushButton(self.settingsButtons)
        self.btnExpFeatures.setObjectName(u"btnExpFeatures")
        self.btnExpFeatures.setEnabled(False)
        self.btnExpFeatures.setMinimumSize(QSize(0, 30))
        self.btnExpFeatures.setCheckable(True)

        self.verticalLayout.addWidget(self.btnExpFeatures)

        self.verticalSpacer_2 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer_2)


        self.gridLayout_5.addWidget(self.settingsButtons, 2, 0, 1, 1)

        self.verticalSpacer_3 = QSpacerItem(20, 10, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)

        self.gridLayout_5.addItem(self.verticalSpacer_3, 0, 0, 1, 1)


        self.gridLayout.addWidget(self.SettingsMenu, 0, 0, 4, 2)

        self.stackedWidget = QStackedWidget(SettingsDialog)
        self.stackedWidget.setObjectName(u"stackedWidget")
        sizePolicy2.setHeightForWidth(self.stackedWidget.sizePolicy().hasHeightForWidth())
        self.stackedWidget.setSizePolicy(sizePolicy2)
        self.stackedWidget.setStyleSheet(u"QStackedWidget#StackedWidget{\n"
"border-style: outset;\n"
"border-width: 1px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-left:none;\n"
"border-bottom:none;\n"
"border-right:none;\n"
"};\n"
"")
        self.settingsGeneral = QWidget()
        self.settingsGeneral.setObjectName(u"settingsGeneral")
        self.gridLayout_7 = QGridLayout(self.settingsGeneral)
        self.gridLayout_7.setSpacing(0)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.verticalWidget = QWidget(self.settingsGeneral)
        self.verticalWidget.setObjectName(u"verticalWidget")
        self.gridLayout_6 = QGridLayout(self.verticalWidget)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.checkBox_4 = QCheckBox(self.verticalWidget)
        self.checkBox_4.setObjectName(u"checkBox_4")
        self.checkBox_4.setEnabled(False)

        self.gridLayout_6.addWidget(self.checkBox_4, 7, 0, 1, 1)

        self.setting_general_systemtray = QCheckBox(self.verticalWidget)
        self.setting_general_systemtray.setObjectName(u"setting_general_systemtray")
        self.setting_general_systemtray.setEnabled(False)

        self.gridLayout_6.addWidget(self.setting_general_systemtray, 5, 0, 1, 1)

        self.checkBox_3 = QCheckBox(self.verticalWidget)
        self.checkBox_3.setObjectName(u"checkBox_3")
        self.checkBox_3.setEnabled(False)

        self.gridLayout_6.addWidget(self.checkBox_3, 6, 0, 1, 1)

        self.checkBox = QCheckBox(self.verticalWidget)
        self.checkBox.setObjectName(u"checkBox")
        self.checkBox.setEnabled(False)

        self.gridLayout_6.addWidget(self.checkBox, 8, 0, 1, 1)


        self.gridLayout_7.addWidget(self.verticalWidget, 2, 1, 1, 1)

        self.verticalSpacer_6 = QSpacerItem(20, 10, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)

        self.gridLayout_7.addItem(self.verticalSpacer_6, 0, 1, 1, 1)

        self.verticalSpacer_7 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_7.addItem(self.verticalSpacer_7, 3, 1, 1, 1)

        self.horizontalSpacer_4 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_7.addItem(self.horizontalSpacer_4, 2, 2, 1, 1)

        self.horizontalSpacer_5 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_7.addItem(self.horizontalSpacer_5, 2, 0, 1, 1)

        self.widget = QWidget(self.settingsGeneral)
        self.widget.setObjectName(u"widget")
        self.gridLayout_8 = QGridLayout(self.widget)
        self.gridLayout_8.setObjectName(u"gridLayout_8")
        self.label_7 = QLabel(self.widget)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setMaximumSize(QSize(100, 16777215))

        self.gridLayout_8.addWidget(self.label_7, 0, 0, 1, 1)

        self.label_10 = QLabel(self.widget)
        self.label_10.setObjectName(u"label_10")

        self.gridLayout_8.addWidget(self.label_10, 1, 0, 1, 1)

        self.setting_general_startup = QComboBox(self.widget)
        self.setting_general_startup.addItem("")
        self.setting_general_startup.addItem("")
        self.setting_general_startup.addItem("")
        self.setting_general_startup.setObjectName(u"setting_general_startup")
        self.setting_general_startup.setEnabled(False)
        self.setting_general_startup.setMaximumSize(QSize(150, 16777215))

        self.gridLayout_8.addWidget(self.setting_general_startup, 0, 1, 1, 1)

        self.setting_general_close = QComboBox(self.widget)
        self.setting_general_close.addItem("")
        self.setting_general_close.addItem("")
        self.setting_general_close.setObjectName(u"setting_general_close")
        self.setting_general_close.setEnabled(False)
        self.setting_general_close.setMinimumSize(QSize(150, 0))
        self.setting_general_close.setMaximumSize(QSize(200, 16777215))

        self.gridLayout_8.addWidget(self.setting_general_close, 1, 1, 1, 1)

        self.horizontalSpacer_6 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_8.addItem(self.horizontalSpacer_6, 1, 2, 1, 1)

        self.label_48 = QLabel(self.widget)
        self.label_48.setObjectName(u"label_48")

        self.gridLayout_8.addWidget(self.label_48, 2, 0, 1, 1)

        self.settings_general_update_channel = QComboBox(self.widget)
        self.settings_general_update_channel.addItem("")
        self.settings_general_update_channel.addItem("")
        self.settings_general_update_channel.addItem("")
        self.settings_general_update_channel.setObjectName(u"settings_general_update_channel")

        self.gridLayout_8.addWidget(self.settings_general_update_channel, 2, 1, 1, 1)


        self.gridLayout_7.addWidget(self.widget, 1, 1, 1, 1)

        self.stackedWidget.addWidget(self.settingsGeneral)
        self.settingsAppearence = QWidget()
        self.settingsAppearence.setObjectName(u"settingsAppearence")
        self.gridLayout_2 = QGridLayout(self.settingsAppearence)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.FONTS = QWidget(self.settingsAppearence)
        self.FONTS.setObjectName(u"FONTS")
        self.gridLayout_12 = QGridLayout(self.FONTS)
        self.gridLayout_12.setObjectName(u"gridLayout_12")
        self.lbSampleText = QLabel(self.FONTS)
        self.lbSampleText.setObjectName(u"lbSampleText")

        self.gridLayout_12.addWidget(self.lbSampleText, 1, 7, 1, 1)

        self.settings_app_fontsize = QSpinBox(self.FONTS)
        self.settings_app_fontsize.setObjectName(u"settings_app_fontsize")

        self.gridLayout_12.addWidget(self.settings_app_fontsize, 1, 5, 1, 1)

        self.label_41 = QLabel(self.FONTS)
        self.label_41.setObjectName(u"label_41")

        self.gridLayout_12.addWidget(self.label_41, 1, 4, 1, 1)

        self.horizontalSpacer_12 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_12.addItem(self.horizontalSpacer_12, 1, 8, 1, 1)

        self.settings_app_font = QComboBox(self.FONTS)
        self.settings_app_font.setObjectName(u"settings_app_font")
        self.settings_app_font.setMinimumSize(QSize(200, 0))

        self.gridLayout_12.addWidget(self.settings_app_font, 1, 2, 1, 1)

        self.label_40 = QLabel(self.FONTS)
        self.label_40.setObjectName(u"label_40")

        self.gridLayout_12.addWidget(self.label_40, 0, 0, 2, 2)

        self.horizontalSpacer_13 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_12.addItem(self.horizontalSpacer_13, 1, 3, 1, 1)

        self.horizontalSpacer_14 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_12.addItem(self.horizontalSpacer_14, 1, 6, 1, 1)


        self.gridLayout_2.addWidget(self.FONTS, 3, 0, 1, 1)

        self.bottom_spacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_2.addItem(self.bottom_spacer, 7, 0, 1, 1)

        self.MENU_LAYOUT = QWidget(self.settingsAppearence)
        self.MENU_LAYOUT.setObjectName(u"MENU_LAYOUT")
        self.gridLayout_11 = QGridLayout(self.MENU_LAYOUT)
        self.gridLayout_11.setObjectName(u"gridLayout_11")

        self.gridLayout_2.addWidget(self.MENU_LAYOUT, 5, 0, 1, 1)

        self.MENU_UI = QWidget(self.settingsAppearence)
        self.MENU_UI.setObjectName(u"MENU_UI")
        self.gridLayout_9 = QGridLayout(self.MENU_UI)
        self.gridLayout_9.setObjectName(u"gridLayout_9")
        self.label_38 = QLabel(self.MENU_UI)
        self.label_38.setObjectName(u"label_38")

        self.gridLayout_9.addWidget(self.label_38, 0, 9, 1, 1)

        self.horizontalSpacer_8 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_9.addItem(self.horizontalSpacer_8, 0, 11, 1, 1)

        self.horizontalSpacer_7 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_9.addItem(self.horizontalSpacer_7, 0, 2, 1, 1)

        self.label_31 = QLabel(self.MENU_UI)
        self.label_31.setObjectName(u"label_31")

        self.gridLayout_9.addWidget(self.label_31, 0, 6, 1, 1)

        self.label_29 = QLabel(self.MENU_UI)
        self.label_29.setObjectName(u"label_29")

        self.gridLayout_9.addWidget(self.label_29, 0, 0, 1, 1)

        self.horizontalSpacer_9 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_9.addItem(self.horizontalSpacer_9, 0, 5, 1, 1)

        self.setting_app_navbar = QComboBox(self.MENU_UI)
        self.setting_app_navbar.addItem("")
        self.setting_app_navbar.addItem("")
        self.setting_app_navbar.setObjectName(u"setting_app_navbar")
        self.setting_app_navbar.setEnabled(False)
        self.setting_app_navbar.setMinimumSize(QSize(100, 0))
        self.setting_app_navbar.setMaximumSize(QSize(100, 16777215))

        self.gridLayout_9.addWidget(self.setting_app_navbar, 0, 4, 1, 1)

        self.setting_app_gameslist = QComboBox(self.MENU_UI)
        self.setting_app_gameslist.addItem("")
        self.setting_app_gameslist.addItem("")
        self.setting_app_gameslist.setObjectName(u"setting_app_gameslist")
        self.setting_app_gameslist.setEnabled(False)
        self.setting_app_gameslist.setMinimumSize(QSize(100, 0))
        self.setting_app_gameslist.setMaximumSize(QSize(100, 16777215))

        self.gridLayout_9.addWidget(self.setting_app_gameslist, 0, 7, 1, 1)

        self.horizontalSpacer_10 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_9.addItem(self.horizontalSpacer_10, 0, 8, 1, 1)

        self.setting_app_menubar = QComboBox(self.MENU_UI)
        self.setting_app_menubar.addItem("")
        self.setting_app_menubar.addItem("")
        self.setting_app_menubar.setObjectName(u"setting_app_menubar")
        self.setting_app_menubar.setEnabled(False)
        self.setting_app_menubar.setMinimumSize(QSize(100, 0))
        self.setting_app_menubar.setMaximumSize(QSize(1090, 16777215))

        self.gridLayout_9.addWidget(self.setting_app_menubar, 0, 1, 1, 1)

        self.label_30 = QLabel(self.MENU_UI)
        self.label_30.setObjectName(u"label_30")

        self.gridLayout_9.addWidget(self.label_30, 0, 3, 1, 1)

        self.setting_app_footer = QComboBox(self.MENU_UI)
        self.setting_app_footer.addItem("")
        self.setting_app_footer.addItem("")
        self.setting_app_footer.setObjectName(u"setting_app_footer")
        self.setting_app_footer.setEnabled(False)
        self.setting_app_footer.setMinimumSize(QSize(100, 0))
        self.setting_app_footer.setMaximumSize(QSize(100, 16777215))

        self.gridLayout_9.addWidget(self.setting_app_footer, 0, 10, 1, 1)


        self.gridLayout_2.addWidget(self.MENU_UI, 4, 0, 1, 1)

        self.THEMES = QWidget(self.settingsAppearence)
        self.THEMES.setObjectName(u"THEMES")
        self.gridLayout_10 = QGridLayout(self.THEMES)
        self.gridLayout_10.setObjectName(u"gridLayout_10")
        self.themeLabel = QLabel(self.THEMES)
        self.themeLabel.setObjectName(u"themeLabel")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Preferred)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.themeLabel.sizePolicy().hasHeightForWidth())
        self.themeLabel.setSizePolicy(sizePolicy3)

        self.gridLayout_10.addWidget(self.themeLabel, 0, 0, 1, 1)

        self.theme_box = QComboBox(self.THEMES)
        self.theme_box.setObjectName(u"theme_box")
        self.theme_box.setMinimumSize(QSize(200, 0))

        self.gridLayout_10.addWidget(self.theme_box, 0, 1, 1, 1)

        self.settings_app_use_system_theme = QCheckBox(self.THEMES)
        self.settings_app_use_system_theme.setObjectName(u"settings_app_use_system_theme")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Fixed)
        sizePolicy4.setHorizontalStretch(0)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.settings_app_use_system_theme.sizePolicy().hasHeightForWidth())
        self.settings_app_use_system_theme.setSizePolicy(sizePolicy4)

        self.gridLayout_10.addWidget(self.settings_app_use_system_theme, 0, 2, 1, 1)

        self.horizontalSpacer_11 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_10.addItem(self.horizontalSpacer_11, 0, 3, 1, 1)


        self.gridLayout_2.addWidget(self.THEMES, 2, 0, 1, 1)

        self.widget_4 = QWidget(self.settingsAppearence)
        self.widget_4.setObjectName(u"widget_4")
        self.gridLayout_14 = QGridLayout(self.widget_4)
        self.gridLayout_14.setObjectName(u"gridLayout_14")
        self.toolButton = QToolButton(self.widget_4)
        self.toolButton.setObjectName(u"toolButton")
        self.toolButton.setMinimumSize(QSize(400, 400))
        self.toolButton.setStyleSheet(u"background-color:transparent;")
        icon = QIcon()
        icon.addFile(u":/Layouts/assets/layouts/0000.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.toolButton.setIcon(icon)
        self.toolButton.setIconSize(QSize(800, 400))

        self.gridLayout_14.addWidget(self.toolButton, 0, 0, 1, 1)


        self.gridLayout_2.addWidget(self.widget_4, 6, 0, 1, 1)

        self.stackedWidget.addWidget(self.settingsAppearence)
        self.settingsGridView = QWidget()
        self.settingsGridView.setObjectName(u"settingsGridView")
        self.gridLayout_4 = QGridLayout(self.settingsGridView)
        self.gridLayout_4.setSpacing(6)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.gridLayout_4.setContentsMargins(2, 2, 2, 2)
        self.label_17 = QLabel(self.settingsGridView)
        self.label_17.setObjectName(u"label_17")
        self.label_17.setMaximumSize(QSize(16777215, 20))

        self.gridLayout_4.addWidget(self.label_17, 2, 1, 1, 1)

        self.groupBox_3 = QGroupBox(self.settingsGridView)
        self.groupBox_3.setObjectName(u"groupBox_3")
        sizePolicy5 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy5.setHorizontalStretch(0)
        sizePolicy5.setVerticalStretch(0)
        sizePolicy5.setHeightForWidth(self.groupBox_3.sizePolicy().hasHeightForWidth())
        self.groupBox_3.setSizePolicy(sizePolicy5)
        self.groupBox_3.setMinimumSize(QSize(0, 66))
        self.groupBox_3.setMaximumSize(QSize(16777215, 66))
        self.gridLayout_38 = QGridLayout(self.groupBox_3)
        self.gridLayout_38.setSpacing(6)
        self.gridLayout_38.setObjectName(u"gridLayout_38")
        self.gridLayout_38.setContentsMargins(9, 9, 9, 9)
        self.cb_ui_element = QComboBox(self.groupBox_3)
        self.cb_ui_element.addItem("")
        self.cb_ui_element.addItem("")
        self.cb_ui_element.setObjectName(u"cb_ui_element")

        self.gridLayout_38.addWidget(self.cb_ui_element, 1, 0, 1, 1)


        self.gridLayout_4.addWidget(self.groupBox_3, 2, 2, 1, 1)

        self.horizontalSpacer_30 = QSpacerItem(5, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_4.addItem(self.horizontalSpacer_30, 3, 11, 1, 1)

        self.QFrame_Bottom_Right_Sub = QFrame(self.settingsGridView)
        self.QFrame_Bottom_Right_Sub.setObjectName(u"QFrame_Bottom_Right_Sub")
        self.QFrame_Bottom_Right_Sub.setMaximumSize(QSize(16777215, 334))
        self.QFrame_Bottom_Right_Sub.setFrameShape(QFrame.StyledPanel)
        self.QFrame_Bottom_Right_Sub.setFrameShadow(QFrame.Raised)
        self.gridLayout_41 = QGridLayout(self.QFrame_Bottom_Right_Sub)
        self.gridLayout_41.setObjectName(u"gridLayout_41")
        self.gridLayout_41.setHorizontalSpacing(0)
        self.gridLayout_41.setVerticalSpacing(6)
        self.gridLayout_41.setContentsMargins(0, 0, 0, 0)
        self.Overlay_Settings = QGroupBox(self.QFrame_Bottom_Right_Sub)
        self.Overlay_Settings.setObjectName(u"Overlay_Settings")
        self.gridLayout_29 = QGridLayout(self.Overlay_Settings)
        self.gridLayout_29.setObjectName(u"gridLayout_29")
        self.gridLayout_29.setContentsMargins(-1, 15, -1, -1)
        self.frame_3 = QFrame(self.Overlay_Settings)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.gridLayout_26 = QGridLayout(self.frame_3)
        self.gridLayout_26.setSpacing(0)
        self.gridLayout_26.setObjectName(u"gridLayout_26")
        self.gridLayout_26.setContentsMargins(0, 0, 0, 0)
        self.horizontalSpacer_66 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_26.addItem(self.horizontalSpacer_66, 1, 5, 1, 1)

        self.settings_ui_bottom_overlay_height = QSpinBox(self.frame_3)
        self.settings_ui_bottom_overlay_height.setObjectName(u"settings_ui_bottom_overlay_height")
        self.settings_ui_bottom_overlay_height.setMaximum(999)

        self.gridLayout_26.addWidget(self.settings_ui_bottom_overlay_height, 1, 4, 1, 1)

        self.horizontalSpacer_53 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_26.addItem(self.horizontalSpacer_53, 1, 3, 1, 1)

        self.label_27 = QLabel(self.frame_3)
        self.label_27.setObjectName(u"label_27")

        self.gridLayout_26.addWidget(self.label_27, 1, 2, 1, 1)

        self.horizontalSpacer_51 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_26.addItem(self.horizontalSpacer_51, 1, 1, 1, 1)

        self.settings_ui_enable_bottom_overlay = QCheckBox(self.frame_3)
        self.settings_ui_enable_bottom_overlay.setObjectName(u"settings_ui_enable_bottom_overlay")

        self.gridLayout_26.addWidget(self.settings_ui_enable_bottom_overlay, 1, 0, 1, 1)

        self.pb_bottom_overlay_bcolor = QPushButton(self.frame_3)
        self.pb_bottom_overlay_bcolor.setObjectName(u"pb_bottom_overlay_bcolor")
        self.pb_bottom_overlay_bcolor.setMinimumSize(QSize(120, 0))

        self.gridLayout_26.addWidget(self.pb_bottom_overlay_bcolor, 1, 6, 1, 1)

        self.horizontalSpacer_52 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_26.addItem(self.horizontalSpacer_52, 1, 7, 1, 1)


        self.gridLayout_29.addWidget(self.frame_3, 1, 0, 1, 1)

        self.frame = QFrame(self.Overlay_Settings)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.gridLayout_24 = QGridLayout(self.frame)
        self.gridLayout_24.setSpacing(0)
        self.gridLayout_24.setObjectName(u"gridLayout_24")
        self.gridLayout_24.setContentsMargins(0, 0, 0, 0)
        self.pb_top_overlay_bcolor = QPushButton(self.frame)
        self.pb_top_overlay_bcolor.setObjectName(u"pb_top_overlay_bcolor")
        self.pb_top_overlay_bcolor.setMinimumSize(QSize(120, 0))

        self.gridLayout_24.addWidget(self.pb_top_overlay_bcolor, 0, 6, 1, 1)

        self.horizontalSpacer_50 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_24.addItem(self.horizontalSpacer_50, 0, 1, 1, 1)

        self.settings_ui_top_overlay_height = QSpinBox(self.frame)
        self.settings_ui_top_overlay_height.setObjectName(u"settings_ui_top_overlay_height")
        sizePolicy6 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)
        sizePolicy6.setHorizontalStretch(0)
        sizePolicy6.setVerticalStretch(0)
        sizePolicy6.setHeightForWidth(self.settings_ui_top_overlay_height.sizePolicy().hasHeightForWidth())
        self.settings_ui_top_overlay_height.setSizePolicy(sizePolicy6)
        self.settings_ui_top_overlay_height.setMaximum(999)
        self.settings_ui_top_overlay_height.setValue(0)

        self.gridLayout_24.addWidget(self.settings_ui_top_overlay_height, 0, 4, 1, 1)

        self.label_24 = QLabel(self.frame)
        self.label_24.setObjectName(u"label_24")

        self.gridLayout_24.addWidget(self.label_24, 0, 2, 1, 1)

        self.horizontalSpacer_67 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_24.addItem(self.horizontalSpacer_67, 0, 5, 1, 1)

        self.horizontalSpacer_65 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_24.addItem(self.horizontalSpacer_65, 0, 3, 1, 1)

        self.settings_ui_enable_top_overlay = QCheckBox(self.frame)
        self.settings_ui_enable_top_overlay.setObjectName(u"settings_ui_enable_top_overlay")

        self.gridLayout_24.addWidget(self.settings_ui_enable_top_overlay, 0, 0, 1, 1)

        self.horizontalSpacer_26 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_24.addItem(self.horizontalSpacer_26, 0, 7, 1, 1)


        self.gridLayout_29.addWidget(self.frame, 0, 0, 1, 1)

        self.frame_2 = QFrame(self.Overlay_Settings)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.gridLayout_25 = QGridLayout(self.frame_2)
        self.gridLayout_25.setSpacing(0)
        self.gridLayout_25.setObjectName(u"gridLayout_25")
        self.gridLayout_25.setContentsMargins(0, 0, 0, 0)
        self.label_67 = QLabel(self.frame_2)
        self.label_67.setObjectName(u"label_67")

        self.gridLayout_25.addWidget(self.label_67, 0, 1, 1, 1)

        self.horizontalSpacer_58 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_25.addItem(self.horizontalSpacer_58, 0, 2, 1, 1)

        self.horizontalSpacer_57 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_25.addItem(self.horizontalSpacer_57, 0, 6, 1, 1)

        self.settings_ui_overlay_layout = QComboBox(self.frame_2)
        self.settings_ui_overlay_layout.addItem("")
        self.settings_ui_overlay_layout.addItem("")
        self.settings_ui_overlay_layout.setObjectName(u"settings_ui_overlay_layout")
        self.settings_ui_overlay_layout.setMinimumSize(QSize(60, 0))

        self.gridLayout_25.addWidget(self.settings_ui_overlay_layout, 0, 3, 1, 1)


        self.gridLayout_29.addWidget(self.frame_2, 2, 0, 1, 1)


        self.gridLayout_41.addWidget(self.Overlay_Settings, 0, 0, 1, 1)

        self.Font_Settings = QGroupBox(self.QFrame_Bottom_Right_Sub)
        self.Font_Settings.setObjectName(u"Font_Settings")
        self.gridLayout_45 = QGridLayout(self.Font_Settings)
        self.gridLayout_45.setObjectName(u"gridLayout_45")
        self.frame_4 = QFrame(self.Font_Settings)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setFrameShape(QFrame.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Raised)
        self.gridLayout_27 = QGridLayout(self.frame_4)
        self.gridLayout_27.setSpacing(0)
        self.gridLayout_27.setObjectName(u"gridLayout_27")
        self.gridLayout_27.setContentsMargins(0, 0, 0, 0)
        self.label_32 = QLabel(self.frame_4)
        self.label_32.setObjectName(u"label_32")

        self.gridLayout_27.addWidget(self.label_32, 0, 0, 1, 1)

        self.horizontalSpacer_28 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_27.addItem(self.horizontalSpacer_28, 0, 1, 1, 1)

        self.settings_ui_font = QComboBox(self.frame_4)
        self.settings_ui_font.setObjectName(u"settings_ui_font")
        sizePolicy7 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Fixed)
        sizePolicy7.setHorizontalStretch(1)
        sizePolicy7.setVerticalStretch(0)
        sizePolicy7.setHeightForWidth(self.settings_ui_font.sizePolicy().hasHeightForWidth())
        self.settings_ui_font.setSizePolicy(sizePolicy7)

        self.gridLayout_27.addWidget(self.settings_ui_font, 0, 2, 1, 1)

        self.horizontalSpacer_27 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_27.addItem(self.horizontalSpacer_27, 0, 3, 1, 1)

        self.settings_ui_font_italic = QCheckBox(self.frame_4)
        self.settings_ui_font_italic.setObjectName(u"settings_ui_font_italic")

        self.gridLayout_27.addWidget(self.settings_ui_font_italic, 0, 6, 1, 1)

        self.settings_ui_font_bold = QCheckBox(self.frame_4)
        self.settings_ui_font_bold.setObjectName(u"settings_ui_font_bold")

        self.gridLayout_27.addWidget(self.settings_ui_font_bold, 0, 4, 1, 1)

        self.horizontalSpacer_70 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_27.addItem(self.horizontalSpacer_70, 0, 5, 1, 1)


        self.gridLayout_45.addWidget(self.frame_4, 0, 0, 1, 1)

        self.frame_5 = QFrame(self.Font_Settings)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setFrameShape(QFrame.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Raised)
        self.gridLayout_28 = QGridLayout(self.frame_5)
        self.gridLayout_28.setSpacing(0)
        self.gridLayout_28.setObjectName(u"gridLayout_28")
        self.gridLayout_28.setContentsMargins(0, 0, 0, 0)
        self.label_19 = QLabel(self.frame_5)
        self.label_19.setObjectName(u"label_19")
        sizePolicy8 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Preferred)
        sizePolicy8.setHorizontalStretch(0)
        sizePolicy8.setVerticalStretch(0)
        sizePolicy8.setHeightForWidth(self.label_19.sizePolicy().hasHeightForWidth())
        self.label_19.setSizePolicy(sizePolicy8)

        self.gridLayout_28.addWidget(self.label_19, 1, 4, 1, 1)

        self.horizontalSpacer_29 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_28.addItem(self.horizontalSpacer_29, 1, 1, 1, 1)

        self.horizontalSpacer_22 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_28.addItem(self.horizontalSpacer_22, 1, 9, 1, 1)

        self.settings_ui_font_shadow = QCheckBox(self.frame_5)
        self.settings_ui_font_shadow.setObjectName(u"settings_ui_font_shadow")

        self.gridLayout_28.addWidget(self.settings_ui_font_shadow, 1, 8, 1, 1)

        self.horizontalSpacer_60 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_28.addItem(self.horizontalSpacer_60, 1, 3, 1, 1)

        self.horizontalSpacer_61 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_28.addItem(self.horizontalSpacer_61, 1, 5, 1, 1)

        self.settings_ui_title_font_size = QSpinBox(self.frame_5)
        self.settings_ui_title_font_size.setObjectName(u"settings_ui_title_font_size")
        self.settings_ui_title_font_size.setMaximum(30)

        self.gridLayout_28.addWidget(self.settings_ui_title_font_size, 1, 6, 1, 1)

        self.settings_ui_font_size = QSpinBox(self.frame_5)
        self.settings_ui_font_size.setObjectName(u"settings_ui_font_size")
        sizePolicy4.setHeightForWidth(self.settings_ui_font_size.sizePolicy().hasHeightForWidth())
        self.settings_ui_font_size.setSizePolicy(sizePolicy4)
        self.settings_ui_font_size.setMaximum(30)
        self.settings_ui_font_size.setValue(2)

        self.gridLayout_28.addWidget(self.settings_ui_font_size, 1, 2, 1, 1)

        self.label_22 = QLabel(self.frame_5)
        self.label_22.setObjectName(u"label_22")
        sizePolicy9 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy9.setHorizontalStretch(0)
        sizePolicy9.setVerticalStretch(0)
        sizePolicy9.setHeightForWidth(self.label_22.sizePolicy().hasHeightForWidth())
        self.label_22.setSizePolicy(sizePolicy9)

        self.gridLayout_28.addWidget(self.label_22, 1, 0, 1, 1)

        self.horizontalSpacer_69 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_28.addItem(self.horizontalSpacer_69, 1, 7, 1, 1)


        self.gridLayout_45.addWidget(self.frame_5, 1, 0, 1, 1)

        self.frame_7 = QFrame(self.Font_Settings)
        self.frame_7.setObjectName(u"frame_7")
        self.frame_7.setFrameShape(QFrame.StyledPanel)
        self.frame_7.setFrameShadow(QFrame.Raised)
        self.gridLayout_31 = QGridLayout(self.frame_7)
        self.gridLayout_31.setSpacing(0)
        self.gridLayout_31.setObjectName(u"gridLayout_31")
        self.gridLayout_31.setContentsMargins(0, 0, 0, 0)
        self.settings_ui_bounding_box = QSpinBox(self.frame_7)
        self.settings_ui_bounding_box.setObjectName(u"settings_ui_bounding_box")

        self.gridLayout_31.addWidget(self.settings_ui_bounding_box, 0, 2, 1, 1)

        self.label_52 = QLabel(self.frame_7)
        self.label_52.setObjectName(u"label_52")

        self.gridLayout_31.addWidget(self.label_52, 0, 0, 1, 1)

        self.label_54 = QLabel(self.frame_7)
        self.label_54.setObjectName(u"label_54")

        self.gridLayout_31.addWidget(self.label_54, 0, 4, 1, 1)

        self.settings_ui_corner_radius = QSpinBox(self.frame_7)
        self.settings_ui_corner_radius.setObjectName(u"settings_ui_corner_radius")

        self.gridLayout_31.addWidget(self.settings_ui_corner_radius, 0, 6, 1, 1)

        self.horizontalSpacer_43 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_31.addItem(self.horizontalSpacer_43, 0, 1, 1, 1)

        self.horizontalSpacer_56 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_31.addItem(self.horizontalSpacer_56, 0, 5, 1, 1)

        self.horizontalSpacer_54 = QSpacerItem(5, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_31.addItem(self.horizontalSpacer_54, 0, 3, 1, 1)

        self.horizontalSpacer_59 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_31.addItem(self.horizontalSpacer_59, 0, 7, 1, 1)


        self.gridLayout_45.addWidget(self.frame_7, 2, 0, 1, 1)


        self.gridLayout_41.addWidget(self.Font_Settings, 1, 0, 1, 1)


        self.gridLayout_4.addWidget(self.QFrame_Bottom_Right_Sub, 5, 2, 1, 1)

        self.groupBox_2 = QGroupBox(self.settingsGridView)
        self.groupBox_2.setObjectName(u"groupBox_2")
        sizePolicy2.setHeightForWidth(self.groupBox_2.sizePolicy().hasHeightForWidth())
        self.groupBox_2.setSizePolicy(sizePolicy2)
        self.groupBox_2.setMinimumSize(QSize(150, 334))
        self.groupBox_2.setMaximumSize(QSize(16777215, 334))
        self.gridLayout_40 = QGridLayout(self.groupBox_2)
        self.gridLayout_40.setObjectName(u"gridLayout_40")
        self.pb_views_bcolor = QPushButton(self.groupBox_2)
        self.pb_views_bcolor.setObjectName(u"pb_views_bcolor")
        self.pb_views_bcolor.setEnabled(True)
        sizePolicy10 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Fixed)
        sizePolicy10.setHorizontalStretch(0)
        sizePolicy10.setVerticalStretch(0)
        sizePolicy10.setHeightForWidth(self.pb_views_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_views_bcolor.setSizePolicy(sizePolicy10)
        self.pb_views_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_views_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_views_bcolor, 11, 6, 1, 1)

        self.settings_ui_creator_default = QCheckBox(self.groupBox_2)
        self.settings_ui_creator_default.setObjectName(u"settings_ui_creator_default")
        self.settings_ui_creator_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_creator_default, 2, 5, 1, 1)

        self.settings_ui_version_align = QComboBox(self.groupBox_2)
        self.settings_ui_version_align.addItem("")
        self.settings_ui_version_align.addItem("")
        self.settings_ui_version_align.addItem("")
        self.settings_ui_version_align.setObjectName(u"settings_ui_version_align")
        sizePolicy10.setHeightForWidth(self.settings_ui_version_align.sizePolicy().hasHeightForWidth())
        self.settings_ui_version_align.setSizePolicy(sizePolicy10)

        self.gridLayout_40.addWidget(self.settings_ui_version_align, 3, 4, 1, 1)

        self.pb_title_bcolor = QPushButton(self.groupBox_2)
        self.pb_title_bcolor.setObjectName(u"pb_title_bcolor")
        sizePolicy6.setHeightForWidth(self.pb_title_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_title_bcolor.setSizePolicy(sizePolicy6)
        self.pb_title_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_title_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_title_bcolor, 1, 6, 1, 1)

        self.pb_status_fcolor = QPushButton(self.groupBox_2)
        self.pb_status_fcolor.setObjectName(u"pb_status_fcolor")
        self.pb_status_fcolor.setEnabled(True)
        self.pb_status_fcolor.setMinimumSize(QSize(30, 0))
        self.pb_status_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_status_fcolor, 8, 7, 1, 1)

        self.settings_ui_status_default = QCheckBox(self.groupBox_2)
        self.settings_ui_status_default.setObjectName(u"settings_ui_status_default")
        self.settings_ui_status_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_status_default, 8, 5, 1, 1)

        self.settings_ui_views_align = QComboBox(self.groupBox_2)
        self.settings_ui_views_align.addItem("")
        self.settings_ui_views_align.addItem("")
        self.settings_ui_views_align.addItem("")
        self.settings_ui_views_align.setObjectName(u"settings_ui_views_align")

        self.gridLayout_40.addWidget(self.settings_ui_views_align, 11, 4, 1, 1)

        self.settings_ui_gametype_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_gametype_enable.setObjectName(u"settings_ui_gametype_enable")

        self.gridLayout_40.addWidget(self.settings_ui_gametype_enable, 9, 1, 1, 1)

        self.settings_ui_engine_y = QSpinBox(self.groupBox_2)
        self.settings_ui_engine_y.setObjectName(u"settings_ui_engine_y")
        self.settings_ui_engine_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_engine_y, 4, 3, 1, 1)

        self.label_36 = QLabel(self.groupBox_2)
        self.label_36.setObjectName(u"label_36")
        self.label_36.setAlignment(Qt.AlignCenter)

        self.gridLayout_40.addWidget(self.label_36, 0, 5, 1, 3)

        self.label_35 = QLabel(self.groupBox_2)
        self.label_35.setObjectName(u"label_35")

        self.gridLayout_40.addWidget(self.label_35, 12, 0, 1, 1)

        self.settings_ui_favorite_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_favorite_enable.setObjectName(u"settings_ui_favorite_enable")
        self.settings_ui_favorite_enable.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_enable, 13, 1, 1, 1)

        self.settings_ui_title_link = QComboBox(self.groupBox_2)
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.addItem("")
        self.settings_ui_title_link.setObjectName(u"settings_ui_title_link")
        self.settings_ui_title_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_title_link, 1, 10, 1, 1)

        self.setting_ui_updateicon_link = QComboBox(self.groupBox_2)
        self.setting_ui_updateicon_link.setObjectName(u"setting_ui_updateicon_link")
        self.setting_ui_updateicon_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.setting_ui_updateicon_link, 14, 10, 1, 1)

        self.settings_ui_updateicon_y = QSpinBox(self.groupBox_2)
        self.settings_ui_updateicon_y.setObjectName(u"settings_ui_updateicon_y")
        self.settings_ui_updateicon_y.setEnabled(False)
        self.settings_ui_updateicon_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_updateicon_y, 14, 3, 1, 1)

        self.settings_ui_title_default = QCheckBox(self.groupBox_2)
        self.settings_ui_title_default.setObjectName(u"settings_ui_title_default")

        self.gridLayout_40.addWidget(self.settings_ui_title_default, 1, 5, 1, 1)

        self.pb_updateicon_fcolor = QPushButton(self.groupBox_2)
        self.pb_updateicon_fcolor.setObjectName(u"pb_updateicon_fcolor")
        self.pb_updateicon_fcolor.setEnabled(False)
        self.pb_updateicon_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_updateicon_fcolor, 14, 7, 1, 1)

        self.settings_ui_engine_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_engine_enable.setObjectName(u"settings_ui_engine_enable")
        self.settings_ui_engine_enable.setChecked(False)

        self.gridLayout_40.addWidget(self.settings_ui_engine_enable, 4, 1, 1, 1)

        self.settings_ui_gametype_align = QComboBox(self.groupBox_2)
        self.settings_ui_gametype_align.addItem("")
        self.settings_ui_gametype_align.addItem("")
        self.settings_ui_gametype_align.addItem("")
        self.settings_ui_gametype_align.setObjectName(u"settings_ui_gametype_align")

        self.gridLayout_40.addWidget(self.settings_ui_gametype_align, 9, 4, 1, 1)

        self.settings_ui_views_y = QSpinBox(self.groupBox_2)
        self.settings_ui_views_y.setObjectName(u"settings_ui_views_y")
        self.settings_ui_views_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_views_y, 11, 3, 1, 1)

        self.label_16 = QLabel(self.groupBox_2)
        self.label_16.setObjectName(u"label_16")

        self.gridLayout_40.addWidget(self.label_16, 2, 0, 1, 1)

        self.settings_ui_creator_y = QSpinBox(self.groupBox_2)
        self.settings_ui_creator_y.setObjectName(u"settings_ui_creator_y")
        self.settings_ui_creator_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_creator_y, 2, 3, 1, 1)

        self.settings_ui_updateicon_align = QComboBox(self.groupBox_2)
        self.settings_ui_updateicon_align.addItem("")
        self.settings_ui_updateicon_align.addItem("")
        self.settings_ui_updateicon_align.addItem("")
        self.settings_ui_updateicon_align.setObjectName(u"settings_ui_updateicon_align")
        self.settings_ui_updateicon_align.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_updateicon_align, 14, 4, 1, 1)

        self.label_20 = QLabel(self.groupBox_2)
        self.label_20.setObjectName(u"label_20")
        self.label_20.setAlignment(Qt.AlignCenter)

        self.gridLayout_40.addWidget(self.label_20, 0, 3, 1, 1)

        self.pb_version_fcolor = QPushButton(self.groupBox_2)
        self.pb_version_fcolor.setObjectName(u"pb_version_fcolor")
        self.pb_version_fcolor.setEnabled(True)
        self.pb_version_fcolor.setMinimumSize(QSize(30, 0))
        self.pb_version_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_version_fcolor, 3, 7, 1, 1)

        self.pb_favorite_fcolor = QPushButton(self.groupBox_2)
        self.pb_favorite_fcolor.setObjectName(u"pb_favorite_fcolor")
        self.pb_favorite_fcolor.setEnabled(False)
        self.pb_favorite_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_favorite_fcolor, 13, 7, 1, 1)

        self.settings_ui_rating_x = QSpinBox(self.groupBox_2)
        self.settings_ui_rating_x.setObjectName(u"settings_ui_rating_x")
        self.settings_ui_rating_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_rating_x, 10, 2, 1, 1)

        self.label_34 = QLabel(self.groupBox_2)
        self.label_34.setObjectName(u"label_34")

        self.gridLayout_40.addWidget(self.label_34, 11, 0, 1, 1)

        self.settings_ui_status_y = QSpinBox(self.groupBox_2)
        self.settings_ui_status_y.setObjectName(u"settings_ui_status_y")
        self.settings_ui_status_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_status_y, 8, 3, 1, 1)

        self.pb_creator_fcolor = QPushButton(self.groupBox_2)
        self.pb_creator_fcolor.setObjectName(u"pb_creator_fcolor")
        self.pb_creator_fcolor.setEnabled(True)
        self.pb_creator_fcolor.setMinimumSize(QSize(30, 0))
        self.pb_creator_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_creator_fcolor, 2, 7, 1, 1)

        self.settings_ui_gametype_default = QCheckBox(self.groupBox_2)
        self.settings_ui_gametype_default.setObjectName(u"settings_ui_gametype_default")
        self.settings_ui_gametype_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_gametype_default, 9, 5, 1, 1)

        self.settings_ui_version_link = QComboBox(self.groupBox_2)
        self.settings_ui_version_link.addItem("")
        self.settings_ui_version_link.addItem("")
        self.settings_ui_version_link.setObjectName(u"settings_ui_version_link")
        self.settings_ui_version_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_version_link, 3, 10, 1, 1)

        self.settings_ui_rating_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_rating_enable.setObjectName(u"settings_ui_rating_enable")

        self.gridLayout_40.addWidget(self.settings_ui_rating_enable, 10, 1, 1, 1)

        self.settings_ui_likes_default = QCheckBox(self.groupBox_2)
        self.settings_ui_likes_default.setObjectName(u"settings_ui_likes_default")
        self.settings_ui_likes_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_likes_default, 12, 5, 1, 1)

        self.settings_ui_views_x = QSpinBox(self.groupBox_2)
        self.settings_ui_views_x.setObjectName(u"settings_ui_views_x")
        self.settings_ui_views_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_views_x, 11, 2, 1, 1)

        self.settings_ui_gametype_x = QSpinBox(self.groupBox_2)
        self.settings_ui_gametype_x.setObjectName(u"settings_ui_gametype_x")
        self.settings_ui_gametype_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_gametype_x, 9, 2, 1, 1)

        self.settings_ui_favorite_y = QSpinBox(self.groupBox_2)
        self.settings_ui_favorite_y.setObjectName(u"settings_ui_favorite_y")
        self.settings_ui_favorite_y.setEnabled(True)
        self.settings_ui_favorite_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_y, 13, 3, 1, 1)

        self.settings_ui_creator_x = QSpinBox(self.groupBox_2)
        self.settings_ui_creator_x.setObjectName(u"settings_ui_creator_x")
        self.settings_ui_creator_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_creator_x, 2, 2, 1, 1)

        self.settings_ui_engine_default = QCheckBox(self.groupBox_2)
        self.settings_ui_engine_default.setObjectName(u"settings_ui_engine_default")

        self.gridLayout_40.addWidget(self.settings_ui_engine_default, 4, 5, 1, 1)

        self.label_15 = QLabel(self.groupBox_2)
        self.label_15.setObjectName(u"label_15")

        self.gridLayout_40.addWidget(self.label_15, 3, 0, 1, 1)

        self.settings_ui_likes_x = QSpinBox(self.groupBox_2)
        self.settings_ui_likes_x.setObjectName(u"settings_ui_likes_x")
        self.settings_ui_likes_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_likes_x, 12, 2, 1, 1)

        self.pb_status_bcolor = QPushButton(self.groupBox_2)
        self.pb_status_bcolor.setObjectName(u"pb_status_bcolor")
        self.pb_status_bcolor.setEnabled(True)
        self.pb_status_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_status_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_status_bcolor, 8, 6, 1, 1)

        self.settings_ui_version_default = QCheckBox(self.groupBox_2)
        self.settings_ui_version_default.setObjectName(u"settings_ui_version_default")

        self.gridLayout_40.addWidget(self.settings_ui_version_default, 3, 5, 1, 1)

        self.settings_ui_title_x = QSpinBox(self.groupBox_2)
        self.settings_ui_title_x.setObjectName(u"settings_ui_title_x")
        self.settings_ui_title_x.setMinimumSize(QSize(45, 0))
        self.settings_ui_title_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_title_x, 1, 2, 1, 1)

        self.settings_ui_version_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_version_enable.setObjectName(u"settings_ui_version_enable")
        self.settings_ui_version_enable.setChecked(False)

        self.gridLayout_40.addWidget(self.settings_ui_version_enable, 3, 1, 1, 1)

        self.settings_ui_favorite_default = QCheckBox(self.groupBox_2)
        self.settings_ui_favorite_default.setObjectName(u"settings_ui_favorite_default")
        self.settings_ui_favorite_default.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_default, 13, 5, 1, 1)

        self.label_49 = QLabel(self.groupBox_2)
        self.label_49.setObjectName(u"label_49")
        self.label_49.setAlignment(Qt.AlignCenter)

        self.gridLayout_40.addWidget(self.label_49, 0, 10, 1, 1)

        self.settings_ui_updateicon_x = QSpinBox(self.groupBox_2)
        self.settings_ui_updateicon_x.setObjectName(u"settings_ui_updateicon_x")
        self.settings_ui_updateicon_x.setEnabled(False)
        self.settings_ui_updateicon_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_updateicon_x, 14, 2, 1, 1)

        self.settings_ui_likes_link = QComboBox(self.groupBox_2)
        self.settings_ui_likes_link.addItem("")
        self.settings_ui_likes_link.addItem("")
        self.settings_ui_likes_link.addItem("")
        self.settings_ui_likes_link.addItem("")
        self.settings_ui_likes_link.addItem("")
        self.settings_ui_likes_link.setObjectName(u"settings_ui_likes_link")
        self.settings_ui_likes_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_likes_link, 12, 10, 1, 1)

        self.settings_ui_views_link = QComboBox(self.groupBox_2)
        self.settings_ui_views_link.addItem("")
        self.settings_ui_views_link.addItem("")
        self.settings_ui_views_link.addItem("")
        self.settings_ui_views_link.addItem("")
        self.settings_ui_views_link.addItem("")
        self.settings_ui_views_link.setObjectName(u"settings_ui_views_link")
        self.settings_ui_views_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_views_link, 11, 10, 1, 1)

        self.pb_creator_bcolor = QPushButton(self.groupBox_2)
        self.pb_creator_bcolor.setObjectName(u"pb_creator_bcolor")
        self.pb_creator_bcolor.setEnabled(True)
        self.pb_creator_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_creator_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_creator_bcolor, 2, 6, 1, 1)

        self.settings_ui_views_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_views_enable.setObjectName(u"settings_ui_views_enable")

        self.gridLayout_40.addWidget(self.settings_ui_views_enable, 11, 1, 1, 1)

        self.settings_ui_likes_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_likes_enable.setObjectName(u"settings_ui_likes_enable")

        self.gridLayout_40.addWidget(self.settings_ui_likes_enable, 12, 1, 1, 1)

        self.pb_engine_bcolor = QPushButton(self.groupBox_2)
        self.pb_engine_bcolor.setObjectName(u"pb_engine_bcolor")
        self.pb_engine_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_engine_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_engine_bcolor, 4, 6, 1, 1)

        self.label_79 = QLabel(self.groupBox_2)
        self.label_79.setObjectName(u"label_79")
        self.label_79.setAlignment(Qt.AlignCenter)

        self.gridLayout_40.addWidget(self.label_79, 0, 2, 1, 1)

        self.settings_ui_status_x = QSpinBox(self.groupBox_2)
        self.settings_ui_status_x.setObjectName(u"settings_ui_status_x")
        self.settings_ui_status_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_status_x, 8, 2, 1, 1)

        self.settings_ui_version_x = QSpinBox(self.groupBox_2)
        self.settings_ui_version_x.setObjectName(u"settings_ui_version_x")
        self.settings_ui_version_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_version_x, 3, 2, 1, 1)

        self.pb_gametype_fcolor = QPushButton(self.groupBox_2)
        self.pb_gametype_fcolor.setObjectName(u"pb_gametype_fcolor")
        self.pb_gametype_fcolor.setEnabled(True)
        self.pb_gametype_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_gametype_fcolor, 9, 7, 1, 1)

        self.settings_ui_title_align = QComboBox(self.groupBox_2)
        self.settings_ui_title_align.addItem("")
        self.settings_ui_title_align.addItem("")
        self.settings_ui_title_align.addItem("")
        self.settings_ui_title_align.setObjectName(u"settings_ui_title_align")
        sizePolicy10.setHeightForWidth(self.settings_ui_title_align.sizePolicy().hasHeightForWidth())
        self.settings_ui_title_align.setSizePolicy(sizePolicy10)

        self.gridLayout_40.addWidget(self.settings_ui_title_align, 1, 4, 1, 1)

        self.settings_ui_updateicon_default = QCheckBox(self.groupBox_2)
        self.settings_ui_updateicon_default.setObjectName(u"settings_ui_updateicon_default")
        self.settings_ui_updateicon_default.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_updateicon_default, 14, 5, 1, 1)

        self.settings_ui_engine_link = QComboBox(self.groupBox_2)
        self.settings_ui_engine_link.setObjectName(u"settings_ui_engine_link")
        self.settings_ui_engine_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_engine_link, 4, 10, 1, 1)

        self.settings_ui_updateicon_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_updateicon_enable.setObjectName(u"settings_ui_updateicon_enable")
        self.settings_ui_updateicon_enable.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_updateicon_enable, 14, 1, 1, 1)

        self.label_64 = QLabel(self.groupBox_2)
        self.label_64.setObjectName(u"label_64")
        self.label_64.setEnabled(False)

        self.gridLayout_40.addWidget(self.label_64, 14, 0, 1, 1)

        self.pb_likes_bcolor = QPushButton(self.groupBox_2)
        self.pb_likes_bcolor.setObjectName(u"pb_likes_bcolor")
        self.pb_likes_bcolor.setEnabled(True)
        sizePolicy10.setHeightForWidth(self.pb_likes_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_likes_bcolor.setSizePolicy(sizePolicy10)
        self.pb_likes_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_likes_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_likes_bcolor, 12, 6, 1, 1)

        self.horizontalSpacer_68 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_40.addItem(self.horizontalSpacer_68, 3, 11, 1, 1)

        self.label_37 = QLabel(self.groupBox_2)
        self.label_37.setObjectName(u"label_37")
        self.label_37.setEnabled(True)

        self.gridLayout_40.addWidget(self.label_37, 13, 0, 1, 1)

        self.settings_ui_title_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_title_enable.setObjectName(u"settings_ui_title_enable")
        self.settings_ui_title_enable.setChecked(False)

        self.gridLayout_40.addWidget(self.settings_ui_title_enable, 1, 1, 1, 1)

        self.pb_version_bcolor = QPushButton(self.groupBox_2)
        self.pb_version_bcolor.setObjectName(u"pb_version_bcolor")
        self.pb_version_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_version_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_version_bcolor, 3, 6, 1, 1)

        self.settings_ui_rating_align = QComboBox(self.groupBox_2)
        self.settings_ui_rating_align.addItem("")
        self.settings_ui_rating_align.addItem("")
        self.settings_ui_rating_align.addItem("")
        self.settings_ui_rating_align.setObjectName(u"settings_ui_rating_align")

        self.gridLayout_40.addWidget(self.settings_ui_rating_align, 10, 4, 1, 1)

        self.label_13 = QLabel(self.groupBox_2)
        self.label_13.setObjectName(u"label_13")
        self.label_13.setMouseTracking(True)

        self.gridLayout_40.addWidget(self.label_13, 1, 0, 1, 1)

        self.settings_ui_rating_y = QSpinBox(self.groupBox_2)
        self.settings_ui_rating_y.setObjectName(u"settings_ui_rating_y")
        self.settings_ui_rating_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_rating_y, 10, 3, 1, 1)

        self.pb_likes_fcolor = QPushButton(self.groupBox_2)
        self.pb_likes_fcolor.setObjectName(u"pb_likes_fcolor")
        self.pb_likes_fcolor.setEnabled(True)
        self.pb_likes_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_likes_fcolor, 12, 7, 1, 1)

        self.settings_ui_likes_align = QComboBox(self.groupBox_2)
        self.settings_ui_likes_align.addItem("")
        self.settings_ui_likes_align.addItem("")
        self.settings_ui_likes_align.setObjectName(u"settings_ui_likes_align")

        self.gridLayout_40.addWidget(self.settings_ui_likes_align, 12, 4, 1, 1)

        self.label_33 = QLabel(self.groupBox_2)
        self.label_33.setObjectName(u"label_33")

        self.gridLayout_40.addWidget(self.label_33, 10, 0, 1, 1)

        self.settings_ui_version_y = QSpinBox(self.groupBox_2)
        self.settings_ui_version_y.setObjectName(u"settings_ui_version_y")
        self.settings_ui_version_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_version_y, 3, 3, 1, 1)

        self.settings_ui_favorite_align = QComboBox(self.groupBox_2)
        self.settings_ui_favorite_align.addItem("")
        self.settings_ui_favorite_align.addItem("")
        self.settings_ui_favorite_align.addItem("")
        self.settings_ui_favorite_align.setObjectName(u"settings_ui_favorite_align")
        self.settings_ui_favorite_align.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_align, 13, 4, 1, 1)

        self.pb_rating_fcolor = QPushButton(self.groupBox_2)
        self.pb_rating_fcolor.setObjectName(u"pb_rating_fcolor")
        self.pb_rating_fcolor.setEnabled(True)
        self.pb_rating_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_rating_fcolor, 10, 7, 1, 1)

        self.settings_ui_creator_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_creator_enable.setObjectName(u"settings_ui_creator_enable")

        self.gridLayout_40.addWidget(self.settings_ui_creator_enable, 2, 1, 1, 1)

        self.settings_ui_rating_default = QCheckBox(self.groupBox_2)
        self.settings_ui_rating_default.setObjectName(u"settings_ui_rating_default")
        self.settings_ui_rating_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_rating_default, 10, 5, 1, 1)

        self.settings_ui_gametype_y = QSpinBox(self.groupBox_2)
        self.settings_ui_gametype_y.setObjectName(u"settings_ui_gametype_y")
        self.settings_ui_gametype_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_gametype_y, 9, 3, 1, 1)

        self.pb_favorite_bcolor = QPushButton(self.groupBox_2)
        self.pb_favorite_bcolor.setObjectName(u"pb_favorite_bcolor")
        self.pb_favorite_bcolor.setEnabled(False)
        sizePolicy10.setHeightForWidth(self.pb_favorite_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_favorite_bcolor.setSizePolicy(sizePolicy10)
        self.pb_favorite_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_favorite_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_favorite_bcolor, 13, 6, 1, 1)

        self.settings_ui_engine_align = QComboBox(self.groupBox_2)
        self.settings_ui_engine_align.addItem("")
        self.settings_ui_engine_align.addItem("")
        self.settings_ui_engine_align.addItem("")
        self.settings_ui_engine_align.setObjectName(u"settings_ui_engine_align")
        sizePolicy10.setHeightForWidth(self.settings_ui_engine_align.sizePolicy().hasHeightForWidth())
        self.settings_ui_engine_align.setSizePolicy(sizePolicy10)

        self.gridLayout_40.addWidget(self.settings_ui_engine_align, 4, 4, 1, 1)

        self.settings_ui_favorite_x = QSpinBox(self.groupBox_2)
        self.settings_ui_favorite_x.setObjectName(u"settings_ui_favorite_x")
        self.settings_ui_favorite_x.setEnabled(True)
        self.settings_ui_favorite_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_x, 13, 2, 1, 1)

        self.settings_ui_status_enable = QCheckBox(self.groupBox_2)
        self.settings_ui_status_enable.setObjectName(u"settings_ui_status_enable")

        self.gridLayout_40.addWidget(self.settings_ui_status_enable, 8, 1, 1, 1)

        self.settings_ui_status_link = QComboBox(self.groupBox_2)
        self.settings_ui_status_link.addItem("")
        self.settings_ui_status_link.addItem("")
        self.settings_ui_status_link.addItem("")
        self.settings_ui_status_link.addItem("")
        self.settings_ui_status_link.addItem("")
        self.settings_ui_status_link.setObjectName(u"settings_ui_status_link")
        self.settings_ui_status_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_status_link, 8, 10, 1, 1)

        self.settings_ui_rating_link = QComboBox(self.groupBox_2)
        self.settings_ui_rating_link.addItem("")
        self.settings_ui_rating_link.addItem("")
        self.settings_ui_rating_link.addItem("")
        self.settings_ui_rating_link.addItem("")
        self.settings_ui_rating_link.addItem("")
        self.settings_ui_rating_link.setObjectName(u"settings_ui_rating_link")
        self.settings_ui_rating_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_rating_link, 10, 10, 1, 1)

        self.settings_ui_engine_x = QSpinBox(self.groupBox_2)
        self.settings_ui_engine_x.setObjectName(u"settings_ui_engine_x")
        self.settings_ui_engine_x.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_engine_x, 4, 2, 1, 1)

        self.settings_ui_favorite_link = QComboBox(self.groupBox_2)
        self.settings_ui_favorite_link.addItem("")
        self.settings_ui_favorite_link.addItem("")
        self.settings_ui_favorite_link.addItem("")
        self.settings_ui_favorite_link.addItem("")
        self.settings_ui_favorite_link.addItem("")
        self.settings_ui_favorite_link.setObjectName(u"settings_ui_favorite_link")
        self.settings_ui_favorite_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_favorite_link, 13, 10, 1, 1)

        self.label_14 = QLabel(self.groupBox_2)
        self.label_14.setObjectName(u"label_14")

        self.gridLayout_40.addWidget(self.label_14, 4, 0, 1, 1)

        self.pb_views_fcolor = QPushButton(self.groupBox_2)
        self.pb_views_fcolor.setObjectName(u"pb_views_fcolor")
        self.pb_views_fcolor.setEnabled(True)
        self.pb_views_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_views_fcolor, 11, 7, 1, 1)

        self.settings_ui_likes_y = QSpinBox(self.groupBox_2)
        self.settings_ui_likes_y.setObjectName(u"settings_ui_likes_y")
        self.settings_ui_likes_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_likes_y, 12, 3, 1, 1)

        self.settings_ui_gametype_link = QComboBox(self.groupBox_2)
        self.settings_ui_gametype_link.addItem("")
        self.settings_ui_gametype_link.addItem("")
        self.settings_ui_gametype_link.addItem("")
        self.settings_ui_gametype_link.addItem("")
        self.settings_ui_gametype_link.addItem("")
        self.settings_ui_gametype_link.setObjectName(u"settings_ui_gametype_link")
        self.settings_ui_gametype_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_gametype_link, 9, 10, 1, 1)

        self.settings_ui_status_align = QComboBox(self.groupBox_2)
        self.settings_ui_status_align.addItem("")
        self.settings_ui_status_align.addItem("")
        self.settings_ui_status_align.addItem("")
        self.settings_ui_status_align.setObjectName(u"settings_ui_status_align")

        self.gridLayout_40.addWidget(self.settings_ui_status_align, 8, 4, 1, 1)

        self.label_23 = QLabel(self.groupBox_2)
        self.label_23.setObjectName(u"label_23")
        self.label_23.setAlignment(Qt.AlignCenter)

        self.gridLayout_40.addWidget(self.label_23, 0, 4, 1, 1)

        self.pb_rating_bcolor = QPushButton(self.groupBox_2)
        self.pb_rating_bcolor.setObjectName(u"pb_rating_bcolor")
        self.pb_rating_bcolor.setEnabled(True)
        sizePolicy10.setHeightForWidth(self.pb_rating_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_rating_bcolor.setSizePolicy(sizePolicy10)
        self.pb_rating_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_rating_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_rating_bcolor, 10, 6, 1, 1)

        self.label_71 = QLabel(self.groupBox_2)
        self.label_71.setObjectName(u"label_71")

        self.gridLayout_40.addWidget(self.label_71, 9, 0, 1, 1)

        self.label_70 = QLabel(self.groupBox_2)
        self.label_70.setObjectName(u"label_70")

        self.gridLayout_40.addWidget(self.label_70, 8, 0, 1, 1)

        self.settings_ui_creator_align = QComboBox(self.groupBox_2)
        self.settings_ui_creator_align.addItem("")
        self.settings_ui_creator_align.addItem("")
        self.settings_ui_creator_align.addItem("")
        self.settings_ui_creator_align.setObjectName(u"settings_ui_creator_align")
        sizePolicy10.setHeightForWidth(self.settings_ui_creator_align.sizePolicy().hasHeightForWidth())
        self.settings_ui_creator_align.setSizePolicy(sizePolicy10)

        self.gridLayout_40.addWidget(self.settings_ui_creator_align, 2, 4, 1, 1)

        self.settings_ui_title_y = QSpinBox(self.groupBox_2)
        self.settings_ui_title_y.setObjectName(u"settings_ui_title_y")
        self.settings_ui_title_y.setMinimumSize(QSize(45, 0))
        self.settings_ui_title_y.setMaximum(999)

        self.gridLayout_40.addWidget(self.settings_ui_title_y, 1, 3, 1, 1)

        self.settings_ui_views_default = QCheckBox(self.groupBox_2)
        self.settings_ui_views_default.setObjectName(u"settings_ui_views_default")
        self.settings_ui_views_default.setEnabled(True)

        self.gridLayout_40.addWidget(self.settings_ui_views_default, 11, 5, 1, 1)

        self.pb_title_fcolor = QPushButton(self.groupBox_2)
        self.pb_title_fcolor.setObjectName(u"pb_title_fcolor")
        self.pb_title_fcolor.setEnabled(True)
        self.pb_title_fcolor.setMinimumSize(QSize(30, 0))
        self.pb_title_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_title_fcolor, 1, 7, 1, 1)

        self.pb_gametype_bcolor = QPushButton(self.groupBox_2)
        self.pb_gametype_bcolor.setObjectName(u"pb_gametype_bcolor")
        self.pb_gametype_bcolor.setEnabled(True)
        sizePolicy10.setHeightForWidth(self.pb_gametype_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_gametype_bcolor.setSizePolicy(sizePolicy10)
        self.pb_gametype_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_gametype_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_gametype_bcolor, 9, 6, 1, 1)

        self.pb_engine_fcolor = QPushButton(self.groupBox_2)
        self.pb_engine_fcolor.setObjectName(u"pb_engine_fcolor")
        self.pb_engine_fcolor.setEnabled(True)
        self.pb_engine_fcolor.setMinimumSize(QSize(30, 0))
        self.pb_engine_fcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_engine_fcolor, 4, 7, 1, 1)

        self.pb_updateicon_bcolor = QPushButton(self.groupBox_2)
        self.pb_updateicon_bcolor.setObjectName(u"pb_updateicon_bcolor")
        self.pb_updateicon_bcolor.setEnabled(False)
        sizePolicy10.setHeightForWidth(self.pb_updateicon_bcolor.sizePolicy().hasHeightForWidth())
        self.pb_updateicon_bcolor.setSizePolicy(sizePolicy10)
        self.pb_updateicon_bcolor.setMinimumSize(QSize(30, 0))
        self.pb_updateicon_bcolor.setMaximumSize(QSize(30, 16777215))

        self.gridLayout_40.addWidget(self.pb_updateicon_bcolor, 14, 6, 1, 1)

        self.settings_ui_creator_link = QComboBox(self.groupBox_2)
        self.settings_ui_creator_link.addItem("")
        self.settings_ui_creator_link.addItem("")
        self.settings_ui_creator_link.setObjectName(u"settings_ui_creator_link")
        self.settings_ui_creator_link.setEnabled(False)

        self.gridLayout_40.addWidget(self.settings_ui_creator_link, 2, 10, 1, 1)


        self.gridLayout_4.addWidget(self.groupBox_2, 5, 1, 1, 1)

        self.verticalSpacer_4 = QSpacerItem(20, 5, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Maximum)

        self.gridLayout_4.addItem(self.verticalSpacer_4, 6, 1, 1, 1)

        self.grid_preview = QListView(self.settingsGridView)
        self.grid_preview.setObjectName(u"grid_preview")
        sizePolicy2.setHeightForWidth(self.grid_preview.sizePolicy().hasHeightForWidth())
        self.grid_preview.setSizePolicy(sizePolicy2)
        self.grid_preview.setMinimumSize(QSize(680, 379))
        self.grid_preview.setMaximumSize(QSize(16777215, 379))
        self.grid_preview.setMouseTracking(True)
        self.grid_preview.setLayoutDirection(Qt.LeftToRight)
        self.grid_preview.setStyleSheet(u"border:none;\n"
"background-color: rgb(180,180,180);\n"
"padding:10x;")
        self.grid_preview.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel)
        self.grid_preview.setFlow(QListView.LeftToRight)
        self.grid_preview.setProperty("isWrapping", True)
        self.grid_preview.setItemAlignment(Qt.AlignCenter|Qt.AlignHCenter|Qt.AlignJustify|Qt.AlignVCenter)

        self.gridLayout_4.addWidget(self.grid_preview, 3, 1, 1, 1)

        self.horizontalSpacer_3 = QSpacerItem(5, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_4.addItem(self.horizontalSpacer_3, 3, 0, 1, 1)

        self.QFrame_Right_Sub = QFrame(self.settingsGridView)
        self.QFrame_Right_Sub.setObjectName(u"QFrame_Right_Sub")
        sizePolicy2.setHeightForWidth(self.QFrame_Right_Sub.sizePolicy().hasHeightForWidth())
        self.QFrame_Right_Sub.setSizePolicy(sizePolicy2)
        self.QFrame_Right_Sub.setMinimumSize(QSize(0, 366))
        self.QFrame_Right_Sub.setMaximumSize(QSize(16777215, 379))
        self.QFrame_Right_Sub.setFrameShape(QFrame.StyledPanel)
        self.QFrame_Right_Sub.setFrameShadow(QFrame.Raised)
        self.gridLayout_35 = QGridLayout(self.QFrame_Right_Sub)
        self.gridLayout_35.setObjectName(u"gridLayout_35")
        self.gridLayout_35.setHorizontalSpacing(0)
        self.gridLayout_35.setVerticalSpacing(6)
        self.gridLayout_35.setContentsMargins(0, 0, 0, 0)
        self.Presets = QGroupBox(self.QFrame_Right_Sub)
        self.Presets.setObjectName(u"Presets")
        sizePolicy5.setHeightForWidth(self.Presets.sizePolicy().hasHeightForWidth())
        self.Presets.setSizePolicy(sizePolicy5)
        self.Presets.setMinimumSize(QSize(0, 66))
        self.Presets.setMaximumSize(QSize(16777215, 66))
        self.gridLayout_44 = QGridLayout(self.Presets)
        self.gridLayout_44.setSpacing(6)
        self.gridLayout_44.setObjectName(u"gridLayout_44")
        self.gridLayout_44.setContentsMargins(9, 9, 9, 9)
        self.settings_ui_preset = QComboBox(self.Presets)
        self.settings_ui_preset.addItem("")
        self.settings_ui_preset.setObjectName(u"settings_ui_preset")

        self.gridLayout_44.addWidget(self.settings_ui_preset, 1, 0, 1, 1)

        self.pb_preset_save = QPushButton(self.Presets)
        self.pb_preset_save.setObjectName(u"pb_preset_save")
        sizePolicy4.setHeightForWidth(self.pb_preset_save.sizePolicy().hasHeightForWidth())
        self.pb_preset_save.setSizePolicy(sizePolicy4)
        self.pb_preset_save.setMaximumSize(QSize(80, 16777215))

        self.gridLayout_44.addWidget(self.pb_preset_save, 1, 1, 1, 1)

        self.pb_preset_load = QPushButton(self.Presets)
        self.pb_preset_load.setObjectName(u"pb_preset_load")
        sizePolicy4.setHeightForWidth(self.pb_preset_load.sizePolicy().hasHeightForWidth())
        self.pb_preset_load.setSizePolicy(sizePolicy4)
        self.pb_preset_load.setMaximumSize(QSize(80, 16777215))

        self.gridLayout_44.addWidget(self.pb_preset_load, 1, 2, 1, 1)


        self.gridLayout_35.addWidget(self.Presets, 0, 0, 1, 1)

        self.BannerSettings = QGroupBox(self.QFrame_Right_Sub)
        self.BannerSettings.setObjectName(u"BannerSettings")
        sizePolicy11 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy11.setHorizontalStretch(0)
        sizePolicy11.setVerticalStretch(0)
        sizePolicy11.setHeightForWidth(self.BannerSettings.sizePolicy().hasHeightForWidth())
        self.BannerSettings.setSizePolicy(sizePolicy11)
        self.BannerSettings.setMinimumSize(QSize(0, 197))
        self.gridLayout_43 = QGridLayout(self.BannerSettings)
        self.gridLayout_43.setObjectName(u"gridLayout_43")
        self.frame_9 = QFrame(self.BannerSettings)
        self.frame_9.setObjectName(u"frame_9")
        sizePolicy1.setHeightForWidth(self.frame_9.sizePolicy().hasHeightForWidth())
        self.frame_9.setSizePolicy(sizePolicy1)
        self.frame_9.setFrameShape(QFrame.StyledPanel)
        self.frame_9.setFrameShadow(QFrame.Raised)
        self.gridLayout_33 = QGridLayout(self.frame_9)
        self.gridLayout_33.setSpacing(0)
        self.gridLayout_33.setObjectName(u"gridLayout_33")
        self.gridLayout_33.setContentsMargins(0, 0, 0, 0)
        self.settings_banner_locksize = QCheckBox(self.frame_9)
        self.settings_banner_locksize.setObjectName(u"settings_banner_locksize")

        self.gridLayout_33.addWidget(self.settings_banner_locksize, 1, 9, 1, 1)

        self.horizontalSpacer_40 = QSpacerItem(5, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_33.addItem(self.horizontalSpacer_40, 1, 6, 1, 1)

        self.horizontalSpacer_39 = QSpacerItem(5, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_33.addItem(self.horizontalSpacer_39, 1, 4, 1, 1)

        self.label_8 = QLabel(self.frame_9)
        self.label_8.setObjectName(u"label_8")

        self.gridLayout_33.addWidget(self.label_8, 1, 1, 1, 1)

        self.settings_ui_bannersize_x = QSpinBox(self.frame_9)
        self.settings_ui_bannersize_x.setObjectName(u"settings_ui_bannersize_x")
        sizePolicy4.setHeightForWidth(self.settings_ui_bannersize_x.sizePolicy().hasHeightForWidth())
        self.settings_ui_bannersize_x.setSizePolicy(sizePolicy4)
        self.settings_ui_bannersize_x.setMinimum(1)
        self.settings_ui_bannersize_x.setMaximum(1500)
        self.settings_ui_bannersize_x.setValue(460)

        self.gridLayout_33.addWidget(self.settings_ui_bannersize_x, 1, 3, 1, 1)

        self.horizontalSpacer_41 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_33.addItem(self.horizontalSpacer_41, 1, 8, 1, 1)

        self.horizontalSpacer_32 = QSpacerItem(40, 20, QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_33.addItem(self.horizontalSpacer_32, 1, 10, 1, 1)

        self.label_9 = QLabel(self.frame_9)
        self.label_9.setObjectName(u"label_9")
        sizePolicy3.setHeightForWidth(self.label_9.sizePolicy().hasHeightForWidth())
        self.label_9.setSizePolicy(sizePolicy3)

        self.gridLayout_33.addWidget(self.label_9, 1, 5, 1, 1)

        self.settings_ui_bannersize_y = QSpinBox(self.frame_9)
        self.settings_ui_bannersize_y.setObjectName(u"settings_ui_bannersize_y")
        sizePolicy4.setHeightForWidth(self.settings_ui_bannersize_y.sizePolicy().hasHeightForWidth())
        self.settings_ui_bannersize_y.setSizePolicy(sizePolicy4)
        self.settings_ui_bannersize_y.setMinimum(1)
        self.settings_ui_bannersize_y.setMaximum(1500)
        self.settings_ui_bannersize_y.setValue(215)

        self.gridLayout_33.addWidget(self.settings_ui_bannersize_y, 1, 7, 1, 1)

        self.horizontalSpacer_38 = QSpacerItem(15, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_33.addItem(self.horizontalSpacer_38, 1, 2, 1, 1)


        self.gridLayout_43.addWidget(self.frame_9, 0, 0, 1, 1)

        self.frame_12 = QFrame(self.BannerSettings)
        self.frame_12.setObjectName(u"frame_12")
        sizePolicy1.setHeightForWidth(self.frame_12.sizePolicy().hasHeightForWidth())
        self.frame_12.setSizePolicy(sizePolicy1)
        self.frame_12.setFrameShape(QFrame.StyledPanel)
        self.frame_12.setFrameShadow(QFrame.Raised)
        self.gridLayout_36 = QGridLayout(self.frame_12)
        self.gridLayout_36.setSpacing(0)
        self.gridLayout_36.setObjectName(u"gridLayout_36")
        self.gridLayout_36.setContentsMargins(0, 0, 0, 0)
        self.pb_hover_color = QPushButton(self.frame_12)
        self.pb_hover_color.setObjectName(u"pb_hover_color")
        self.pb_hover_color.setEnabled(False)
        self.pb_hover_color.setMinimumSize(QSize(120, 0))

        self.gridLayout_36.addWidget(self.pb_hover_color, 0, 0, 1, 1)

        self.horizontalSpacer_44 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_36.addItem(self.horizontalSpacer_44, 0, 2, 1, 1)

        self.pb_background_color = QPushButton(self.frame_12)
        self.pb_background_color.setObjectName(u"pb_background_color")
        self.pb_background_color.setEnabled(False)
        sizePolicy6.setHeightForWidth(self.pb_background_color.sizePolicy().hasHeightForWidth())
        self.pb_background_color.setSizePolicy(sizePolicy6)
        self.pb_background_color.setMinimumSize(QSize(120, 0))

        self.gridLayout_36.addWidget(self.pb_background_color, 0, 1, 1, 1)


        self.gridLayout_43.addWidget(self.frame_12, 2, 0, 1, 1)

        self.frame_13 = QFrame(self.BannerSettings)
        self.frame_13.setObjectName(u"frame_13")
        sizePolicy1.setHeightForWidth(self.frame_13.sizePolicy().hasHeightForWidth())
        self.frame_13.setSizePolicy(sizePolicy1)
        self.frame_13.setFrameShape(QFrame.StyledPanel)
        self.frame_13.setFrameShadow(QFrame.Raised)
        self.gridLayout_37 = QGridLayout(self.frame_13)
        self.gridLayout_37.setSpacing(0)
        self.gridLayout_37.setObjectName(u"gridLayout_37")
        self.gridLayout_37.setContentsMargins(0, 0, 0, 0)
        self.horizontalSpacer_33 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_37.addItem(self.horizontalSpacer_33, 1, 1, 1, 1)

        self.pb_border_color = QPushButton(self.frame_13)
        self.pb_border_color.setObjectName(u"pb_border_color")
        self.pb_border_color.setEnabled(False)
        self.pb_border_color.setMinimumSize(QSize(120, 0))

        self.gridLayout_37.addWidget(self.pb_border_color, 1, 2, 1, 1)

        self.settings_ui_enable_banner_border = QCheckBox(self.frame_13)
        self.settings_ui_enable_banner_border.setObjectName(u"settings_ui_enable_banner_border")

        self.gridLayout_37.addWidget(self.settings_ui_enable_banner_border, 1, 0, 1, 1)

        self.horizontalSpacer_45 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_37.addItem(self.horizontalSpacer_45, 1, 3, 1, 1)


        self.gridLayout_43.addWidget(self.frame_13, 3, 0, 1, 1)

        self.frame_10 = QFrame(self.BannerSettings)
        self.frame_10.setObjectName(u"frame_10")
        sizePolicy1.setHeightForWidth(self.frame_10.sizePolicy().hasHeightForWidth())
        self.frame_10.setSizePolicy(sizePolicy1)
        self.frame_10.setFrameShape(QFrame.StyledPanel)
        self.frame_10.setFrameShadow(QFrame.Raised)
        self.gridLayout_34 = QGridLayout(self.frame_10)
        self.gridLayout_34.setSpacing(0)
        self.gridLayout_34.setObjectName(u"gridLayout_34")
        self.gridLayout_34.setContentsMargins(0, 0, 0, 0)
        self.label_12 = QLabel(self.frame_10)
        self.label_12.setObjectName(u"label_12")

        self.gridLayout_34.addWidget(self.label_12, 0, 0, 1, 1)

        self.horizontalSpacer_37 = QSpacerItem(5, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_34.addItem(self.horizontalSpacer_37, 0, 1, 1, 1)

        self.settings_ui_centeritems = QCheckBox(self.frame_10)
        self.settings_ui_centeritems.setObjectName(u"settings_ui_centeritems")
        self.settings_ui_centeritems.setEnabled(False)

        self.gridLayout_34.addWidget(self.settings_ui_centeritems, 0, 4, 1, 1)

        self.settings_ui_banner_spacing = QSpinBox(self.frame_10)
        self.settings_ui_banner_spacing.setObjectName(u"settings_ui_banner_spacing")
        sizePolicy4.setHeightForWidth(self.settings_ui_banner_spacing.sizePolicy().hasHeightForWidth())
        self.settings_ui_banner_spacing.setSizePolicy(sizePolicy4)
        self.settings_ui_banner_spacing.setMaximum(5000)
        self.settings_ui_banner_spacing.setValue(2)

        self.gridLayout_34.addWidget(self.settings_ui_banner_spacing, 0, 2, 1, 1)

        self.horizontalSpacer_34 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_34.addItem(self.horizontalSpacer_34, 0, 3, 1, 1)

        self.horizontalSpacer_49 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_34.addItem(self.horizontalSpacer_49, 0, 5, 1, 1)


        self.gridLayout_43.addWidget(self.frame_10, 1, 0, 1, 1)


        self.gridLayout_35.addWidget(self.BannerSettings, 1, 0, 1, 1)

        self.ImageSettings = QGroupBox(self.QFrame_Right_Sub)
        self.ImageSettings.setObjectName(u"ImageSettings")
        sizePolicy11.setHeightForWidth(self.ImageSettings.sizePolicy().hasHeightForWidth())
        self.ImageSettings.setSizePolicy(sizePolicy11)
        self.gridLayout_39 = QGridLayout(self.ImageSettings)
        self.gridLayout_39.setObjectName(u"gridLayout_39")
        self.frame_6 = QFrame(self.ImageSettings)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setFrameShape(QFrame.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Raised)
        self.gridLayout_30 = QGridLayout(self.frame_6)
        self.gridLayout_30.setSpacing(0)
        self.gridLayout_30.setObjectName(u"gridLayout_30")
        self.gridLayout_30.setContentsMargins(0, 0, 0, 0)
        self.horizontalSpacer_24 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_30.addItem(self.horizontalSpacer_24, 1, 7, 1, 1)

        self.label_18 = QLabel(self.frame_6)
        self.label_18.setObjectName(u"label_18")

        self.gridLayout_30.addWidget(self.label_18, 1, 4, 1, 1)

        self.horizontalSpacer_55 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_30.addItem(self.horizontalSpacer_55, 1, 3, 1, 1)

        self.settings_ui_image_layout = QComboBox(self.frame_6)
        self.settings_ui_image_layout.addItem("")
        self.settings_ui_image_layout.addItem("")
        self.settings_ui_image_layout.addItem("")
        self.settings_ui_image_layout.addItem("")
        self.settings_ui_image_layout.setObjectName(u"settings_ui_image_layout")
        self.settings_ui_image_layout.setMinimumSize(QSize(100, 0))

        self.gridLayout_30.addWidget(self.settings_ui_image_layout, 1, 2, 1, 1)

        self.settings_ui_blur_type = QComboBox(self.frame_6)
        self.settings_ui_blur_type.addItem("")
        self.settings_ui_blur_type.addItem("")
        self.settings_ui_blur_type.setObjectName(u"settings_ui_blur_type")

        self.gridLayout_30.addWidget(self.settings_ui_blur_type, 1, 6, 1, 1)

        self.label_11 = QLabel(self.frame_6)
        self.label_11.setObjectName(u"label_11")
        sizePolicy9.setHeightForWidth(self.label_11.sizePolicy().hasHeightForWidth())
        self.label_11.setSizePolicy(sizePolicy9)

        self.gridLayout_30.addWidget(self.label_11, 1, 0, 1, 1)

        self.horizontalSpacer_21 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_30.addItem(self.horizontalSpacer_21, 1, 1, 1, 1)

        self.horizontalSpacer_62 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_30.addItem(self.horizontalSpacer_62, 1, 5, 1, 1)


        self.gridLayout_39.addWidget(self.frame_6, 0, 0, 1, 1)

        self.frame_8 = QFrame(self.ImageSettings)
        self.frame_8.setObjectName(u"frame_8")
        self.frame_8.setFrameShape(QFrame.StyledPanel)
        self.frame_8.setFrameShadow(QFrame.Raised)
        self.gridLayout_32 = QGridLayout(self.frame_8)
        self.gridLayout_32.setSpacing(0)
        self.gridLayout_32.setObjectName(u"gridLayout_32")
        self.gridLayout_32.setContentsMargins(0, 0, 0, 0)
        self.label_26 = QLabel(self.frame_8)
        self.label_26.setObjectName(u"label_26")

        self.gridLayout_32.addWidget(self.label_26, 0, 4, 1, 1)

        self.horizontalSpacer_64 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_32.addItem(self.horizontalSpacer_64, 0, 5, 1, 1)

        self.settings_ui_feather_radius = QSpinBox(self.frame_8)
        self.settings_ui_feather_radius.setObjectName(u"settings_ui_feather_radius")
        sizePolicy4.setHeightForWidth(self.settings_ui_feather_radius.sizePolicy().hasHeightForWidth())
        self.settings_ui_feather_radius.setSizePolicy(sizePolicy4)
        self.settings_ui_feather_radius.setMaximum(100)
        self.settings_ui_feather_radius.setValue(2)

        self.gridLayout_32.addWidget(self.settings_ui_feather_radius, 0, 6, 1, 1)

        self.horizontalSpacer_42 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_32.addItem(self.horizontalSpacer_42, 0, 3, 1, 1)

        self.settings_ui_blur_strength = QSpinBox(self.frame_8)
        self.settings_ui_blur_strength.setObjectName(u"settings_ui_blur_strength")
        sizePolicy4.setHeightForWidth(self.settings_ui_blur_strength.sizePolicy().hasHeightForWidth())
        self.settings_ui_blur_strength.setSizePolicy(sizePolicy4)
        self.settings_ui_blur_strength.setMaximum(100)
        self.settings_ui_blur_strength.setValue(2)

        self.gridLayout_32.addWidget(self.settings_ui_blur_strength, 0, 2, 1, 1)

        self.label_21 = QLabel(self.frame_8)
        self.label_21.setObjectName(u"label_21")

        self.gridLayout_32.addWidget(self.label_21, 0, 0, 1, 1)

        self.horizontalSpacer_63 = QSpacerItem(10, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.gridLayout_32.addItem(self.horizontalSpacer_63, 0, 1, 1, 1)

        self.horizontalSpacer_36 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_32.addItem(self.horizontalSpacer_36, 0, 7, 1, 1)


        self.gridLayout_39.addWidget(self.frame_8, 2, 0, 1, 1)


        self.gridLayout_35.addWidget(self.ImageSettings, 2, 0, 1, 1)


        self.gridLayout_4.addWidget(self.QFrame_Right_Sub, 3, 2, 1, 1)

        self.stackedWidget.addWidget(self.settingsGridView)
        self.settingsPaths = QWidget()
        self.settingsPaths.setObjectName(u"settingsPaths")
        self.gridLayout_3 = QGridLayout(self.settingsPaths)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.settings_paths_database = QLineEdit(self.settingsPaths)
        self.settings_paths_database.setObjectName(u"settings_paths_database")
        sizePolicy6.setHeightForWidth(self.settings_paths_database.sizePolicy().hasHeightForWidth())
        self.settings_paths_database.setSizePolicy(sizePolicy6)
        self.settings_paths_database.setReadOnly(True)

        self.gridLayout_3.addWidget(self.settings_paths_database, 7, 1, 1, 1)

        self.label_2 = QLabel(self.settingsPaths)
        self.label_2.setObjectName(u"label_2")
        sizePolicy3.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.label_2, 4, 0, 1, 1)

        self.gamesSizeLabel = QLabel(self.settingsPaths)
        self.gamesSizeLabel.setObjectName(u"gamesSizeLabel")
        sizePolicy3.setHeightForWidth(self.gamesSizeLabel.sizePolicy().hasHeightForWidth())
        self.gamesSizeLabel.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.gamesSizeLabel, 4, 3, 1, 1)

        self.label_3 = QLabel(self.settingsPaths)
        self.label_3.setObjectName(u"label_3")
        sizePolicy3.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.label_3, 7, 0, 1, 1)

        self.label_5 = QLabel(self.settingsPaths)
        self.label_5.setObjectName(u"label_5")
        sizePolicy5.setHeightForWidth(self.label_5.sizePolicy().hasHeightForWidth())
        self.label_5.setSizePolicy(sizePolicy5)

        self.gridLayout_3.addWidget(self.label_5, 3, 1, 1, 1)

        self.settings_paths_images = QLineEdit(self.settingsPaths)
        self.settings_paths_images.setObjectName(u"settings_paths_images")
        sizePolicy6.setHeightForWidth(self.settings_paths_images.sizePolicy().hasHeightForWidth())
        self.settings_paths_images.setSizePolicy(sizePolicy6)

        self.gridLayout_3.addWidget(self.settings_paths_images, 2, 1, 1, 1)

        self.settings_paths_games = QLineEdit(self.settingsPaths)
        self.settings_paths_games.setObjectName(u"settings_paths_games")
        sizePolicy6.setHeightForWidth(self.settings_paths_games.sizePolicy().hasHeightForWidth())
        self.settings_paths_games.setSizePolicy(sizePolicy6)

        self.gridLayout_3.addWidget(self.settings_paths_games, 4, 1, 1, 1)

        self.imagesSizeLabel = QLabel(self.settingsPaths)
        self.imagesSizeLabel.setObjectName(u"imagesSizeLabel")
        sizePolicy3.setHeightForWidth(self.imagesSizeLabel.sizePolicy().hasHeightForWidth())
        self.imagesSizeLabel.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.imagesSizeLabel, 2, 3, 1, 1)

        self.label_6 = QLabel(self.settingsPaths)
        self.label_6.setObjectName(u"label_6")
        sizePolicy5.setHeightForWidth(self.label_6.sizePolicy().hasHeightForWidth())
        self.label_6.setSizePolicy(sizePolicy5)
        self.label_6.setTextFormat(Qt.AutoText)
        self.label_6.setMargin(0)
        self.label_6.setIndent(-1)

        self.gridLayout_3.addWidget(self.label_6, 5, 1, 1, 1)

        self.label_4 = QLabel(self.settingsPaths)
        self.label_4.setObjectName(u"label_4")
        sizePolicy12 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Maximum)
        sizePolicy12.setHorizontalStretch(0)
        sizePolicy12.setVerticalStretch(0)
        sizePolicy12.setHeightForWidth(self.label_4.sizePolicy().hasHeightForWidth())
        self.label_4.setSizePolicy(sizePolicy12)

        self.gridLayout_3.addWidget(self.label_4, 1, 1, 1, 1)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_3.addItem(self.verticalSpacer, 8, 1, 1, 1)

        self.label = QLabel(self.settingsPaths)
        self.label.setObjectName(u"label")
        sizePolicy3.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.label, 2, 0, 1, 1)

        self.canonicalPathLabel = QLabel(self.settingsPaths)
        self.canonicalPathLabel.setObjectName(u"canonicalPathLabel")

        self.gridLayout_3.addWidget(self.canonicalPathLabel, 0, 0, 1, 1)

        self.canonicalPath = QLineEdit(self.settingsPaths)
        self.canonicalPath.setObjectName(u"canonicalPath")
        self.canonicalPath.setReadOnly(True)

        self.gridLayout_3.addWidget(self.canonicalPath, 0, 1, 1, 1)

        self.databaseSizeLabel = QLabel(self.settingsPaths)
        self.databaseSizeLabel.setObjectName(u"databaseSizeLabel")
        sizePolicy3.setHeightForWidth(self.databaseSizeLabel.sizePolicy().hasHeightForWidth())
        self.databaseSizeLabel.setSizePolicy(sizePolicy3)

        self.gridLayout_3.addWidget(self.databaseSizeLabel, 7, 3, 1, 1)

        self.setPreviewPath = QPushButton(self.settingsPaths)
        self.setPreviewPath.setObjectName(u"setPreviewPath")
        sizePolicy4.setHeightForWidth(self.setPreviewPath.sizePolicy().hasHeightForWidth())
        self.setPreviewPath.setSizePolicy(sizePolicy4)
        self.setPreviewPath.setMaximumSize(QSize(24, 24))

        self.gridLayout_3.addWidget(self.setPreviewPath, 2, 2, 1, 1)

        self.setGamePath = QPushButton(self.settingsPaths)
        self.setGamePath.setObjectName(u"setGamePath")
        sizePolicy4.setHeightForWidth(self.setGamePath.sizePolicy().hasHeightForWidth())
        self.setGamePath.setSizePolicy(sizePolicy4)
        self.setGamePath.setMaximumSize(QSize(24, 24))

        self.gridLayout_3.addWidget(self.setGamePath, 4, 2, 1, 1)

        self.stackedWidget.addWidget(self.settingsPaths)
        self.settingsThreading = QWidget()
        self.settingsThreading.setObjectName(u"settingsThreading")
        self.gridLayout_13 = QGridLayout(self.settingsThreading)
        self.gridLayout_13.setObjectName(u"gridLayout_13")
        self.verticalSpacer_10 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_13.addItem(self.verticalSpacer_10, 2, 0, 1, 1)

        self.ThreadingWidget = QWidget(self.settingsThreading)
        self.ThreadingWidget.setObjectName(u"ThreadingWidget")
        self.gridLayout_22 = QGridLayout(self.ThreadingWidget)
        self.gridLayout_22.setObjectName(u"gridLayout_22")
        self.widget_8 = QWidget(self.ThreadingWidget)
        self.widget_8.setObjectName(u"widget_8")
        self.gridLayout_20 = QGridLayout(self.widget_8)
        self.gridLayout_20.setObjectName(u"gridLayout_20")
        self.label_45 = QLabel(self.widget_8)
        self.label_45.setObjectName(u"label_45")

        self.gridLayout_20.addWidget(self.label_45, 0, 0, 1, 1)

        self.settings_threads_import_threads = QSpinBox(self.widget_8)
        self.settings_threads_import_threads.setObjectName(u"settings_threads_import_threads")
        self.settings_threads_import_threads.setMinimumSize(QSize(50, 0))
        self.settings_threads_import_threads.setMaximumSize(QSize(50, 16777215))
        self.settings_threads_import_threads.setMinimum(1)
        self.settings_threads_import_threads.setMaximum(32)

        self.gridLayout_20.addWidget(self.settings_threads_import_threads, 0, 2, 1, 1)

        self.horizontalSpacer_23 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_20.addItem(self.horizontalSpacer_23, 0, 1, 1, 1)


        self.gridLayout_22.addWidget(self.widget_8, 2, 0, 1, 1)

        self.widget_9 = QWidget(self.ThreadingWidget)
        self.widget_9.setObjectName(u"widget_9")
        self.gridLayout_21 = QGridLayout(self.widget_9)
        self.gridLayout_21.setObjectName(u"gridLayout_21")
        self.settings_threads_image_import_threads = QSpinBox(self.widget_9)
        self.settings_threads_image_import_threads.setObjectName(u"settings_threads_image_import_threads")
        self.settings_threads_image_import_threads.setMinimumSize(QSize(50, 0))
        self.settings_threads_image_import_threads.setMaximumSize(QSize(50, 16777215))
        self.settings_threads_image_import_threads.setMinimum(1)
        self.settings_threads_image_import_threads.setMaximum(32)

        self.gridLayout_21.addWidget(self.settings_threads_image_import_threads, 0, 2, 1, 1)

        self.horizontalSpacer_25 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_21.addItem(self.horizontalSpacer_25, 0, 1, 1, 1)

        self.label_46 = QLabel(self.widget_9)
        self.label_46.setObjectName(u"label_46")

        self.gridLayout_21.addWidget(self.label_46, 0, 0, 1, 1)


        self.gridLayout_22.addWidget(self.widget_9, 3, 0, 1, 1)

        self.widget_7 = QWidget(self.ThreadingWidget)
        self.widget_7.setObjectName(u"widget_7")
        self.gridLayout_19 = QGridLayout(self.widget_7)
        self.gridLayout_19.setObjectName(u"gridLayout_19")
        self.horizontalSpacer_20 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_19.addItem(self.horizontalSpacer_20, 0, 1, 1, 1)

        self.settings_threads_import_pre_loader_threads = QSpinBox(self.widget_7)
        self.settings_threads_import_pre_loader_threads.setObjectName(u"settings_threads_import_pre_loader_threads")
        self.settings_threads_import_pre_loader_threads.setMinimumSize(QSize(50, 0))
        self.settings_threads_import_pre_loader_threads.setMaximumSize(QSize(50, 16777215))
        self.settings_threads_import_pre_loader_threads.setMinimum(1)
        self.settings_threads_import_pre_loader_threads.setMaximum(32)

        self.gridLayout_19.addWidget(self.settings_threads_import_pre_loader_threads, 0, 2, 1, 1)

        self.label_44 = QLabel(self.widget_7)
        self.label_44.setObjectName(u"label_44")

        self.gridLayout_19.addWidget(self.label_44, 0, 0, 1, 1)


        self.gridLayout_22.addWidget(self.widget_7, 1, 0, 1, 1)

        self.widget_6 = QWidget(self.ThreadingWidget)
        self.widget_6.setObjectName(u"widget_6")
        self.gridLayout_18 = QGridLayout(self.widget_6)
        self.gridLayout_18.setObjectName(u"gridLayout_18")
        self.horizontalSpacer_18 = QSpacerItem(10, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.gridLayout_18.addItem(self.horizontalSpacer_18, 0, 1, 1, 1)

        self.label_43 = QLabel(self.widget_6)
        self.label_43.setObjectName(u"label_43")

        self.gridLayout_18.addWidget(self.label_43, 0, 0, 1, 1)

        self.settings_threads_image_loader_threads = QSpinBox(self.widget_6)
        self.settings_threads_image_loader_threads.setObjectName(u"settings_threads_image_loader_threads")
        self.settings_threads_image_loader_threads.setMinimumSize(QSize(50, 0))
        self.settings_threads_image_loader_threads.setMaximumSize(QSize(50, 16777215))
        self.settings_threads_image_loader_threads.setMinimum(1)
        self.settings_threads_image_loader_threads.setMaximum(32)

        self.gridLayout_18.addWidget(self.settings_threads_image_loader_threads, 0, 2, 1, 1)


        self.gridLayout_22.addWidget(self.widget_6, 0, 0, 1, 1)

        self.horizontalSpacer_19 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout_22.addItem(self.horizontalSpacer_19, 1, 1, 1, 1)


        self.gridLayout_13.addWidget(self.ThreadingWidget, 1, 0, 1, 1)

        self.ThreadingDetails = QWidget(self.settingsThreading)
        self.ThreadingDetails.setObjectName(u"ThreadingDetails")
        self.gridLayout_23 = QGridLayout(self.ThreadingDetails)
        self.gridLayout_23.setObjectName(u"gridLayout_23")
        self.label_47 = QLabel(self.ThreadingDetails)
        self.label_47.setObjectName(u"label_47")
        self.label_47.setWordWrap(True)

        self.gridLayout_23.addWidget(self.label_47, 0, 0, 1, 1)


        self.gridLayout_13.addWidget(self.ThreadingDetails, 0, 0, 1, 1)

        self.stackedWidget.addWidget(self.settingsThreading)
        self.settingsExperimental = QWidget()
        self.settingsExperimental.setObjectName(u"settingsExperimental")
        self.settingsExperimental.setEnabled(False)
        self.gridLayout_16 = QGridLayout(self.settingsExperimental)
        self.gridLayout_16.setObjectName(u"gridLayout_16")
        self.gridLayout_15 = QGridLayout()
        self.gridLayout_15.setObjectName(u"gridLayout_15")
        self.cbExpFindAtlData = QCheckBox(self.settingsExperimental)
        self.cbExpFindAtlData.setObjectName(u"cbExpFindAtlData")

        self.gridLayout_15.addWidget(self.cbExpFindAtlData, 1, 0, 1, 1)

        self.label_42 = QLabel(self.settingsExperimental)
        self.label_42.setObjectName(u"label_42")

        self.gridLayout_15.addWidget(self.label_42, 3, 0, 1, 1)

        self.cbExpBannerDownload = QCheckBox(self.settingsExperimental)
        self.cbExpBannerDownload.setObjectName(u"cbExpBannerDownload")
        self.cbExpBannerDownload.setEnabled(False)

        self.gridLayout_15.addWidget(self.cbExpBannerDownload, 4, 0, 1, 1)

        self.label_39 = QLabel(self.settingsExperimental)
        self.label_39.setObjectName(u"label_39")

        self.gridLayout_15.addWidget(self.label_39, 0, 0, 1, 1)

        self.widget_5 = QWidget(self.settingsExperimental)
        self.widget_5.setObjectName(u"widget_5")
        self.horizontalLayout_2 = QHBoxLayout(self.widget_5)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.cbExpThumb = QCheckBox(self.widget_5)
        self.cbExpThumb.setObjectName(u"cbExpThumb")

        self.horizontalLayout_2.addWidget(self.cbExpThumb)

        self.horizontalSpacer_16 = QSpacerItem(30, 20, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_16)

        self.label_28 = QLabel(self.widget_5)
        self.label_28.setObjectName(u"label_28")

        self.horizontalLayout_2.addWidget(self.label_28)

        self.horizontalSpacer_17 = QSpacerItem(5, 20, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_17)

        self.spExpThumbRadius = QSpinBox(self.widget_5)
        self.spExpThumbRadius.setObjectName(u"spExpThumbRadius")

        self.horizontalLayout_2.addWidget(self.spExpThumbRadius)

        self.horizontalSpacer_15 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_15)


        self.gridLayout_15.addWidget(self.widget_5, 5, 0, 1, 1)

        self.verticalSpacer_8 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_15.addItem(self.verticalSpacer_8, 6, 0, 1, 1)

        self.verticalSpacer_9 = QSpacerItem(5, 5, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)

        self.gridLayout_15.addItem(self.verticalSpacer_9, 2, 0, 1, 1)


        self.gridLayout_16.addLayout(self.gridLayout_15, 0, 0, 1, 1)

        self.stackedWidget.addWidget(self.settingsExperimental)

        self.gridLayout.addWidget(self.stackedWidget, 3, 2, 1, 1)


        self.retranslateUi(SettingsDialog)

        self.stackedWidget.setCurrentIndex(2)


        QMetaObject.connectSlotsByName(SettingsDialog)
    # setupUi

    def retranslateUi(self, SettingsDialog):
        SettingsDialog.setWindowTitle(QCoreApplication.translate("SettingsDialog", u"Settings", None))
        self.applySettings.setText(QCoreApplication.translate("SettingsDialog", u"Apply", None))
        self.cancelSettings.setText(QCoreApplication.translate("SettingsDialog", u"Cancel", None))
        self.lblSettingsHeader.setText(QCoreApplication.translate("SettingsDialog", u"GENERAL", None))
        self.lblSettings.setText(QCoreApplication.translate("SettingsDialog", u"ATLAS SETTINGS", None))
        self.btnGeneral.setText(QCoreApplication.translate("SettingsDialog", u"General", None))
        self.btnInterface.setText(QCoreApplication.translate("SettingsDialog", u"Interface", None))
        self.btnUilayout.setStyleSheet("")
        self.btnUilayout.setText(QCoreApplication.translate("SettingsDialog", u"UI Layout", None))
        self.btnPaths.setText(QCoreApplication.translate("SettingsDialog", u"Paths", None))
        self.btnThreading.setText(QCoreApplication.translate("SettingsDialog", u"Threading", None))
        self.btnExpFeatures.setText(QCoreApplication.translate("SettingsDialog", u"Experimental Features", None))
        self.checkBox_4.setText(QCoreApplication.translate("SettingsDialog", u"Launch in Fullscreen Mode", None))
        self.setting_general_systemtray.setText(QCoreApplication.translate("SettingsDialog", u"Minimize Atlas to system tray when the application is closed", None))
        self.checkBox_3.setText(QCoreApplication.translate("SettingsDialog", u"Update install size of games on libary update", None))
        self.checkBox.setText(QCoreApplication.translate("SettingsDialog", u"Use fuzzy matching in name filter", None))
        self.label_7.setText(QCoreApplication.translate("SettingsDialog", u"When game starts:", None))
        self.label_10.setText(QCoreApplication.translate("SettingsDialog", u"After game closes:", None))
        self.setting_general_startup.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Do Nothing", None))
        self.setting_general_startup.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Minimize", None))
        self.setting_general_startup.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Close", None))

        self.setting_general_close.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Do Nothing", None))
        self.setting_general_close.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Restore Window", None))

        self.label_48.setText(QCoreApplication.translate("SettingsDialog", u"Update Channel", None))
        self.settings_general_update_channel.setItemText(0, QCoreApplication.translate("SettingsDialog", u"stable", None))
        self.settings_general_update_channel.setItemText(1, QCoreApplication.translate("SettingsDialog", u"nightly", None))
        self.settings_general_update_channel.setItemText(2, QCoreApplication.translate("SettingsDialog", u"off", None))

        self.lbSampleText.setText(QCoreApplication.translate("SettingsDialog", u"Sample Text : 123456 : ^&*(!", None))
        self.label_41.setText(QCoreApplication.translate("SettingsDialog", u"Font Size", None))
        self.label_40.setText(QCoreApplication.translate("SettingsDialog", u"Application Font", None))
        self.label_38.setText(QCoreApplication.translate("SettingsDialog", u"Footer", None))
        self.label_31.setText(QCoreApplication.translate("SettingsDialog", u"Games List", None))
        self.label_29.setText(QCoreApplication.translate("SettingsDialog", u"Menu Bar", None))
        self.setting_app_navbar.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.setting_app_navbar.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Top", None))

        self.setting_app_gameslist.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Enabled", None))
        self.setting_app_gameslist.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Disabled", None))

        self.setting_app_menubar.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Enabled", None))
        self.setting_app_menubar.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Disabled", None))

        self.label_30.setText(QCoreApplication.translate("SettingsDialog", u"Navigation Bar", None))
        self.setting_app_footer.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Enabled", None))
        self.setting_app_footer.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Disabled", None))

        self.themeLabel.setText(QCoreApplication.translate("SettingsDialog", u"Theme:", None))
        self.settings_app_use_system_theme.setText(QCoreApplication.translate("SettingsDialog", u"Use system theme", None))
        self.toolButton.setText(QCoreApplication.translate("SettingsDialog", u"...", None))
        self.label_17.setText(QCoreApplication.translate("SettingsDialog", u"The default Banner Image size is 537px x 251px. All setting  below will be used for each Grid Capsule Layout. ", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("SettingsDialog", u"Select UI Element to Change", None))
        self.cb_ui_element.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Banner View", None))
        self.cb_ui_element.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Game Detail View", None))

        self.Overlay_Settings.setTitle(QCoreApplication.translate("SettingsDialog", u"Overlay Settings", None))
        self.label_27.setText(QCoreApplication.translate("SettingsDialog", u"Height:", None))
        self.settings_ui_enable_bottom_overlay.setText(QCoreApplication.translate("SettingsDialog", u"Overlay Bottom", None))
        self.pb_bottom_overlay_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"Background Color", None))
        self.pb_top_overlay_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"Background Color", None))
        self.label_24.setText(QCoreApplication.translate("SettingsDialog", u"Height:", None))
        self.settings_ui_enable_top_overlay.setText(QCoreApplication.translate("SettingsDialog", u"Overlay Top", None))
        self.label_67.setText(QCoreApplication.translate("SettingsDialog", u"Layout", None))
        self.settings_ui_overlay_layout.setItemText(0, QCoreApplication.translate("SettingsDialog", u"On Top", None))
        self.settings_ui_overlay_layout.setItemText(1, QCoreApplication.translate("SettingsDialog", u"In Line", None))

        self.Font_Settings.setTitle(QCoreApplication.translate("SettingsDialog", u"Font Settings", None))
        self.label_32.setText(QCoreApplication.translate("SettingsDialog", u"Font", None))
        self.settings_ui_font_italic.setText(QCoreApplication.translate("SettingsDialog", u"Italic", None))
        self.settings_ui_font_bold.setText(QCoreApplication.translate("SettingsDialog", u"Bold", None))
        self.label_19.setText(QCoreApplication.translate("SettingsDialog", u"Title Font Size", None))
        self.settings_ui_font_shadow.setText(QCoreApplication.translate("SettingsDialog", u"Text Shadow", None))
        self.label_22.setText(QCoreApplication.translate("SettingsDialog", u"Font Size", None))
        self.label_52.setText(QCoreApplication.translate("SettingsDialog", u"Bounding Box Padding:", None))
        self.label_54.setText(QCoreApplication.translate("SettingsDialog", u"Corner Radius:", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("SettingsDialog", u"Overlay Details", None))
        self.pb_views_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_creator_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_version_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_version_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_version_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.pb_title_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.pb_status_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_status_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_views_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_views_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_views_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.settings_ui_gametype_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.label_36.setText(QCoreApplication.translate("SettingsDialog", u"Item Color Settings", None))
        self.label_35.setText(QCoreApplication.translate("SettingsDialog", u"Likes", None))
        self.settings_ui_favorite_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_title_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Title", None))
        self.settings_ui_title_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Creator", None))
        self.settings_ui_title_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Version", None))
        self.settings_ui_title_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"Engine", None))
        self.settings_ui_title_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"Status", None))
        self.settings_ui_title_link.setItemText(5, QCoreApplication.translate("SettingsDialog", u"Game Type", None))
        self.settings_ui_title_link.setItemText(6, QCoreApplication.translate("SettingsDialog", u"Rating", None))
        self.settings_ui_title_link.setItemText(7, QCoreApplication.translate("SettingsDialog", u"Views", None))
        self.settings_ui_title_link.setItemText(8, QCoreApplication.translate("SettingsDialog", u"Likes", None))
        self.settings_ui_title_link.setItemText(9, QCoreApplication.translate("SettingsDialog", u"Favorite", None))

        self.settings_ui_title_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.pb_updateicon_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_engine_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_gametype_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_gametype_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_gametype_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.label_16.setText(QCoreApplication.translate("SettingsDialog", u"Creator", None))
        self.settings_ui_updateicon_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_updateicon_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_updateicon_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.label_20.setText(QCoreApplication.translate("SettingsDialog", u"Y Axis", None))
        self.pb_version_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.pb_favorite_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.label_34.setText(QCoreApplication.translate("SettingsDialog", u"Views", None))
        self.pb_creator_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_gametype_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_version_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Static", None))
        self.settings_ui_version_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Relatvie", None))

        self.settings_ui_rating_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_likes_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_engine_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.label_15.setText(QCoreApplication.translate("SettingsDialog", u"Version", None))
        self.pb_status_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_version_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_version_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_favorite_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.label_49.setText(QCoreApplication.translate("SettingsDialog", u"Link", None))
        self.settings_ui_likes_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_likes_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_likes_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_likes_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_likes_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.settings_ui_views_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_views_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_views_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_views_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_views_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.pb_creator_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_views_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_likes_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.pb_engine_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.label_79.setText(QCoreApplication.translate("SettingsDialog", u"X Axis", None))
        self.pb_gametype_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_title_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_title_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_title_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.settings_ui_updateicon_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.settings_ui_updateicon_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.label_64.setText(QCoreApplication.translate("SettingsDialog", u"Update Icon", None))
        self.pb_likes_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.label_37.setText(QCoreApplication.translate("SettingsDialog", u"Favorite", None))
        self.settings_ui_title_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.pb_version_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_rating_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_rating_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_rating_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.label_13.setText(QCoreApplication.translate("SettingsDialog", u"Game Title", None))
        self.pb_likes_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_likes_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_likes_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))

        self.label_33.setText(QCoreApplication.translate("SettingsDialog", u"Rating", None))
        self.settings_ui_favorite_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_favorite_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_favorite_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.pb_rating_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_creator_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_rating_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.pb_favorite_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_engine_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_engine_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_engine_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.settings_ui_status_enable.setText(QCoreApplication.translate("SettingsDialog", u"Enable", None))
        self.settings_ui_status_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_status_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_status_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_status_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_status_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.settings_ui_rating_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_rating_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_rating_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_rating_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_rating_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.settings_ui_favorite_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_favorite_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_favorite_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_favorite_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_favorite_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.label_14.setText(QCoreApplication.translate("SettingsDialog", u"Engine", None))
        self.pb_views_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.settings_ui_gametype_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"none", None))
        self.settings_ui_gametype_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"title", None))
        self.settings_ui_gametype_link.setItemText(2, QCoreApplication.translate("SettingsDialog", u"engine", None))
        self.settings_ui_gametype_link.setItemText(3, QCoreApplication.translate("SettingsDialog", u"version", None))
        self.settings_ui_gametype_link.setItemText(4, QCoreApplication.translate("SettingsDialog", u"creator", None))

        self.settings_ui_status_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_status_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_status_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.label_23.setText(QCoreApplication.translate("SettingsDialog", u"Alignment", None))
        self.pb_rating_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.label_71.setText(QCoreApplication.translate("SettingsDialog", u"Game Type", None))
        self.label_70.setText(QCoreApplication.translate("SettingsDialog", u"Status", None))
        self.settings_ui_creator_align.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Left", None))
        self.settings_ui_creator_align.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Right", None))
        self.settings_ui_creator_align.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Center", None))

        self.settings_ui_views_default.setText(QCoreApplication.translate("SettingsDialog", u"Default", None))
        self.pb_title_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.pb_gametype_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.pb_engine_fcolor.setText(QCoreApplication.translate("SettingsDialog", u"fgc", None))
        self.pb_updateicon_bcolor.setText(QCoreApplication.translate("SettingsDialog", u"bgc", None))
        self.settings_ui_creator_link.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Static", None))
        self.settings_ui_creator_link.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Relative", None))

        self.Presets.setTitle(QCoreApplication.translate("SettingsDialog", u"Select Preset", None))
        self.settings_ui_preset.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Atlas Default", None))

        self.pb_preset_save.setText(QCoreApplication.translate("SettingsDialog", u"Load", None))
        self.pb_preset_load.setText(QCoreApplication.translate("SettingsDialog", u"Save", None))
        self.BannerSettings.setTitle(QCoreApplication.translate("SettingsDialog", u"Banner Settings", None))
        self.settings_banner_locksize.setText(QCoreApplication.translate("SettingsDialog", u"Lock Aspect Ratio", None))
        self.label_8.setText(QCoreApplication.translate("SettingsDialog", u"Size:", None))
        self.label_9.setText(QCoreApplication.translate("SettingsDialog", u"x", None))
        self.pb_hover_color.setText(QCoreApplication.translate("SettingsDialog", u"Hover Color", None))
        self.pb_background_color.setText(QCoreApplication.translate("SettingsDialog", u"Background Color", None))
        self.pb_border_color.setText(QCoreApplication.translate("SettingsDialog", u"Border Color", None))
        self.settings_ui_enable_banner_border.setText(QCoreApplication.translate("SettingsDialog", u" Capsule Border", None))
        self.label_12.setText(QCoreApplication.translate("SettingsDialog", u"Spacing (Not Shown)", None))
        self.settings_ui_centeritems.setText(QCoreApplication.translate("SettingsDialog", u"Center Items", None))
        self.ImageSettings.setTitle(QCoreApplication.translate("SettingsDialog", u"Image Settings", None))
        self.label_18.setText(QCoreApplication.translate("SettingsDialog", u"Blur Sides ", None))
        self.settings_ui_image_layout.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Stretch", None))
        self.settings_ui_image_layout.setItemText(1, QCoreApplication.translate("SettingsDialog", u"Fit", None))
        self.settings_ui_image_layout.setItemText(2, QCoreApplication.translate("SettingsDialog", u"Fill", None))
        self.settings_ui_image_layout.setItemText(3, QCoreApplication.translate("SettingsDialog", u"Fit + Blur", None))

        self.settings_ui_blur_type.setItemText(0, QCoreApplication.translate("SettingsDialog", u"Off", None))
        self.settings_ui_blur_type.setItemText(1, QCoreApplication.translate("SettingsDialog", u"On", None))

        self.label_11.setText(QCoreApplication.translate("SettingsDialog", u"Layout", None))
        self.label_26.setText(QCoreApplication.translate("SettingsDialog", u"Feather Radius", None))
        self.label_21.setText(QCoreApplication.translate("SettingsDialog", u"Blur Stregnth", None))
        self.label_2.setText(QCoreApplication.translate("SettingsDialog", u"Games Path:", None))
        self.gamesSizeLabel.setText(QCoreApplication.translate("SettingsDialog", u"0B", None))
        self.label_3.setText(QCoreApplication.translate("SettingsDialog", u"Database Path:", None))
        self.label_5.setText(QCoreApplication.translate("SettingsDialog", u"Where all games are installed to (Except games marked 'do not manage')", None))
        self.imagesSizeLabel.setText(QCoreApplication.translate("SettingsDialog", u"0B", None))
        self.label_6.setText(QCoreApplication.translate("SettingsDialog", u"Location of database file", None))
        self.label_4.setText(QCoreApplication.translate("SettingsDialog", u"Where previews and banners are stored", None))
        self.label.setText(QCoreApplication.translate("SettingsDialog", u"Images Path:", None))
        self.canonicalPathLabel.setText(QCoreApplication.translate("SettingsDialog", u"Canonical path:", None))
        self.databaseSizeLabel.setText(QCoreApplication.translate("SettingsDialog", u"0B", None))
        self.setPreviewPath.setText(QCoreApplication.translate("SettingsDialog", u"...", None))
        self.setGamePath.setText(QCoreApplication.translate("SettingsDialog", u"...", None))
        self.label_45.setText(QCoreApplication.translate("SettingsDialog", u"Importer: ", None))
        self.label_46.setText(QCoreApplication.translate("SettingsDialog", u"Image importer:", None))
        self.label_44.setText(QCoreApplication.translate("SettingsDialog", u"Import pre-processor:", None))
        self.label_43.setText(QCoreApplication.translate("SettingsDialog", u"Image loading: ", None))
        self.label_47.setText(QCoreApplication.translate("SettingsDialog", u"All the settings below are specific to loading images and importing. This will modify how fast games are imported and how fast images diplay while using banner view. You can reset to system defaults if you run into any issues while importing. ", None))
        self.cbExpFindAtlData.setText(QCoreApplication.translate("SettingsDialog", u"Try to find game metadata based on title and developer (This will only be shown in the Game Details screen)", None))
        self.label_42.setText(QCoreApplication.translate("SettingsDialog", u"Images", None))
        self.cbExpBannerDownload.setText(QCoreApplication.translate("SettingsDialog", u"Download banner images for games (During Import)", None))
        self.label_39.setText(QCoreApplication.translate("SettingsDialog", u"UI Features", None))
        self.cbExpThumb.setText(QCoreApplication.translate("SettingsDialog", u"Load Image thumbnails while waiting on fullsize image to load", None))
        self.label_28.setText(QCoreApplication.translate("SettingsDialog", u"Blur Radius", None))
    # retranslateUi

