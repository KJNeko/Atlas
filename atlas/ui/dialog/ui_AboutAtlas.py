# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'AboutAtlas.ui'
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
from PySide6.QtWidgets import (QApplication, QDialog, QFrame, QGridLayout,
    QHBoxLayout, QLabel, QPushButton, QSizePolicy,
    QSpacerItem, QTextEdit, QToolButton, QWidget)

class Ui_AboutAtlas(object):
    def setupUi(self, AboutAtlas):
        if not AboutAtlas.objectName():
            AboutAtlas.setObjectName(u"AboutAtlas")
        AboutAtlas.resize(640, 371)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(AboutAtlas.sizePolicy().hasHeightForWidth())
        AboutAtlas.setSizePolicy(sizePolicy)
        self.gridLayout = QGridLayout(AboutAtlas)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.widget = QWidget(AboutAtlas)
        self.widget.setObjectName(u"widget")
        self.widget.setMinimumSize(QSize(0, 50))
        self.horizontalLayout = QHBoxLayout(self.widget)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.btnGithub = QPushButton(self.widget)
        self.btnGithub.setObjectName(u"btnGithub")

        self.horizontalLayout.addWidget(self.btnGithub)

        self.pushButton_2 = QPushButton(self.widget)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setEnabled(False)

        self.horizontalLayout.addWidget(self.pushButton_2)

        self.btnDiscord = QPushButton(self.widget)
        self.btnDiscord.setObjectName(u"btnDiscord")

        self.horizontalLayout.addWidget(self.btnDiscord)

        self.pushButton_4 = QPushButton(self.widget)
        self.pushButton_4.setObjectName(u"pushButton_4")
        self.pushButton_4.setEnabled(False)

        self.horizontalLayout.addWidget(self.pushButton_4)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer)

        self.btnClose = QPushButton(self.widget)
        self.btnClose.setObjectName(u"btnClose")

        self.horizontalLayout.addWidget(self.btnClose)


        self.gridLayout.addWidget(self.widget, 1, 0, 1, 2)

        self.frame_2 = QFrame(AboutAtlas)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.gridLayout_2 = QGridLayout(self.frame_2)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setVerticalSpacing(0)
        self.frame_3 = QFrame(self.frame_2)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setMinimumSize(QSize(37, 0))
        self.frame_3.setMaximumSize(QSize(100, 16777215))
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.gridLayout_3 = QGridLayout(self.frame_3)
        self.gridLayout_3.setSpacing(0)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalSpacer = QSpacerItem(20, 186, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout_3.addItem(self.verticalSpacer, 2, 1, 1, 1)

        self.toolButton = QToolButton(self.frame_3)
        self.toolButton.setObjectName(u"toolButton")
        self.toolButton.setStyleSheet(u"background-color:none; border:none;")
        icon = QIcon()
        icon.addFile(u":/images/assets/Atlas_logo_v2.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.toolButton.setIcon(icon)
        self.toolButton.setIconSize(QSize(100, 100))

        self.gridLayout_3.addWidget(self.toolButton, 0, 1, 2, 1)


        self.gridLayout_2.addWidget(self.frame_3, 0, 1, 5, 1)

        self.frame = QFrame(self.frame_2)
        self.frame.setObjectName(u"frame")
        self.frame.setMinimumSize(QSize(0, 75))
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.gridLayout_4 = QGridLayout(self.frame)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.gridLayout_4.setVerticalSpacing(0)
        self.frame_6 = QFrame(self.frame)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setMinimumSize(QSize(0, 23))
        self.frame_6.setFrameShape(QFrame.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Raised)
        self.gridLayout_6 = QGridLayout(self.frame_6)
        self.gridLayout_6.setSpacing(0)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.gridLayout_6.setContentsMargins(0, 0, 0, 0)
        self.label_2 = QLabel(self.frame_6)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setMaximumSize(QSize(55, 16777215))
        font = QFont()
        font.setPointSize(8)
        self.label_2.setFont(font)

        self.gridLayout_6.addWidget(self.label_2, 0, 0, 1, 1)

        self.lblGitBranch = QLabel(self.frame_6)
        self.lblGitBranch.setObjectName(u"lblGitBranch")
        self.lblGitBranch.setFont(font)

        self.gridLayout_6.addWidget(self.lblGitBranch, 0, 1, 1, 1)


        self.gridLayout_4.addWidget(self.frame_6, 3, 0, 1, 5)

        self.textEdit = QTextEdit(self.frame)
        self.textEdit.setObjectName(u"textEdit")
        self.textEdit.setStyleSheet(u"border:none;\n"
"background:transparent;")
        self.textEdit.setFrameShape(QFrame.NoFrame)
        self.textEdit.setFrameShadow(QFrame.Plain)
        self.textEdit.setReadOnly(True)

        self.gridLayout_4.addWidget(self.textEdit, 1, 0, 1, 5)

        self.frame_4 = QFrame(self.frame)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setMinimumSize(QSize(0, 23))
        self.frame_4.setFrameShape(QFrame.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.frame_4)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.lbUsingVers = QLabel(self.frame_4)
        self.lbUsingVers.setObjectName(u"lbUsingVers")
        self.lbUsingVers.setFont(font)

        self.horizontalLayout_2.addWidget(self.lbUsingVers)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_2)


        self.gridLayout_4.addWidget(self.frame_4, 5, 0, 1, 5)

        self.frame_5 = QFrame(self.frame)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setMinimumSize(QSize(0, 23))
        self.frame_5.setFrameShape(QFrame.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Raised)
        self.gridLayout_5 = QGridLayout(self.frame_5)
        self.gridLayout_5.setSpacing(0)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setContentsMargins(0, 0, 0, 0)
        self.label_3 = QLabel(self.frame_5)
        self.label_3.setObjectName(u"label_3")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.MinimumExpanding, QSizePolicy.Policy.MinimumExpanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy1)
        self.label_3.setMaximumSize(QSize(55, 16777215))
        self.label_3.setFont(font)

        self.gridLayout_5.addWidget(self.label_3, 0, 0, 1, 1)

        self.lblGitRev = QLabel(self.frame_5)
        self.lblGitRev.setObjectName(u"lblGitRev")
        self.lblGitRev.setFont(font)

        self.gridLayout_5.addWidget(self.lblGitRev, 0, 1, 1, 1)


        self.gridLayout_4.addWidget(self.frame_5, 4, 0, 1, 5)

        self.lblGitVersion = QLabel(self.frame)
        self.lblGitVersion.setObjectName(u"lblGitVersion")
        font1 = QFont()
        font1.setPointSize(14)
        self.lblGitVersion.setFont(font1)

        self.gridLayout_4.addWidget(self.lblGitVersion, 0, 1, 1, 1)

        self.label = QLabel(self.frame)
        self.label.setObjectName(u"label")
        font2 = QFont()
        font2.setPointSize(14)
        font2.setBold(False)
        self.label.setFont(font2)

        self.gridLayout_4.addWidget(self.label, 0, 0, 1, 1)

        self.frame_7 = QFrame(self.frame)
        self.frame_7.setObjectName(u"frame_7")
        self.frame_7.setMinimumSize(QSize(0, 23))
        self.frame_7.setFrameShape(QFrame.StyledPanel)
        self.frame_7.setFrameShadow(QFrame.Raised)
        self.gridLayout_7 = QGridLayout(self.frame_7)
        self.gridLayout_7.setSpacing(0)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.gridLayout_7.setContentsMargins(0, 0, 0, 0)
        self.label_8 = QLabel(self.frame_7)
        self.label_8.setObjectName(u"label_8")
        self.label_8.setMaximumSize(QSize(55, 16777215))
        self.label_8.setFont(font)

        self.gridLayout_7.addWidget(self.label_8, 0, 0, 1, 1)

        self.lblCompiler = QLabel(self.frame_7)
        self.lblCompiler.setObjectName(u"lblCompiler")
        self.lblCompiler.setFont(font)

        self.gridLayout_7.addWidget(self.lblCompiler, 0, 1, 1, 1)


        self.gridLayout_4.addWidget(self.frame_7, 2, 0, 1, 5)


        self.gridLayout_2.addWidget(self.frame, 1, 2, 1, 1)


        self.gridLayout.addWidget(self.frame_2, 0, 0, 1, 2)


        self.retranslateUi(AboutAtlas)

        QMetaObject.connectSlotsByName(AboutAtlas)
    # setupUi

    def retranslateUi(self, AboutAtlas):
        AboutAtlas.setWindowTitle(QCoreApplication.translate("AboutAtlas", u"About Atlas", None))
        self.btnGithub.setText(QCoreApplication.translate("AboutAtlas", u"GitHub", None))
        self.pushButton_2.setText(QCoreApplication.translate("AboutAtlas", u"Website", None))
        self.btnDiscord.setText(QCoreApplication.translate("AboutAtlas", u"Discord", None))
        self.pushButton_4.setText(QCoreApplication.translate("AboutAtlas", u"Wiki", None))
        self.btnClose.setText(QCoreApplication.translate("AboutAtlas", u"Close", None))
        self.toolButton.setText(QCoreApplication.translate("AboutAtlas", u"...", None))
        self.label_2.setText(QCoreApplication.translate("AboutAtlas", u"Branch:", None))
        self.lblGitBranch.setText(QCoreApplication.translate("AboutAtlas", u"TextLabel", None))
        self.textEdit.setHtml(QCoreApplication.translate("AboutAtlas", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Atlas is an open-source game manager with a primary goal of organizing your NSFW game libary. It is written in C++ for Windows and Linux. We currently support adding games from F95Zone and DLSite.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bot"
                        "tom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Atlas is for the user that wants ease of use, quality of life and have the software do the heavy lifting for you.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Our developers and contributors are always working hard to ensure this project is the best that it can be. There are still plenty of implementations to make and optimizations to do. </p></body></html>", None))
        self.lbUsingVers.setText(QCoreApplication.translate("AboutAtlas", u"Using:  ", None))
        self.label_3.setText(QCoreApplication.translate("AboutAtlas", u"Revision:", None))
        self.lblGitRev.setText(QCoreApplication.translate("AboutAtlas", u"TextLabel", None))
        self.lblGitVersion.setText(QCoreApplication.translate("AboutAtlas", u"TextLabel", None))
        self.label.setText(QCoreApplication.translate("AboutAtlas", u"Atlas Game Manager", None))
        self.label_8.setText(QCoreApplication.translate("AboutAtlas", u"Compiler:", None))
        self.lblCompiler.setText(QCoreApplication.translate("AboutAtlas", u"TextLabel", None))
    # retranslateUi

