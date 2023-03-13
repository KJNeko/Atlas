#Towerwatchman
#3/4/2022

#Python Libraries
import sys
import os
from glob import glob
import re
from ui.dialog.UIImportGames import ImportDialog
from db.ClientFunctions import *

from PyQt6 import QtCore, QtGui, QtWidgets, uic
from PyQt6.QtWidgets import QPushButton, QToolButton, QFileDialog, QTableWidget, QMainWindow, QDialog, QListView, QFormLayout, QSizePolicy
from PyQt6.QtGui import *
from PyQt6.QtCore import *
class MainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        uic.loadUi("ui/window/mainwindow.ui", self)

        #DEBUG
        #deleteLocalDatabase()
        createLocalDatabase()

        self.updateGamesList() #Get games from DB and put in listview
        self.showCoverView()
        #Assign Button Actions
        self.actionImport.triggered.connect(self.openImportDialog)


        #self.btnOpen = self.findChild(QToolButton, "btnOpen")
        #self.twGame = self.findChild(QTableWidget, "twGame")
        #self.btnOpen.clicked.connect(self.find_games)

    def openImportDialog(self):
        import_dialog = ImportDialog()
        import_dialog.games_added.connect(self.updateGamesList)
        import_dialog.exec()

    def updateGamesList(self):
        self.lwGames.clear()
        for data in getRecordTitles():
            title = data[0].strip()
            img =  Path("data/images/icons/"+ str(data[1]) + ".png")
            if img.is_file():
                icon = QtGui.QIcon("data/images/icons/"+ str(data[1]) + ".png")
            else:
                icon = QtGui.QIcon("data/images/icons/BLANK.png")
            #print(data[0].strip())
            #check if icon exist
            item = QtWidgets.QListWidgetItem(icon, title)
            self.lwGames.addItem(item)

    def showCoverView(self):
        #self.layout = QFormLayout(self)

        #self.BannerView.setLayout(self.layout)

        games = getRecordTitles()
        for game in games:
            self.toolbutton = QToolButton()
            self.toolbutton.setMinimumWidth(200)
            self.toolbutton.setMinimumHeight(200)
            #self.toolbutton.setSize(QSize(20,300))
            self.toolbutton.setToolButtonStyle(Qt.ToolButtonStyle.ToolButtonTextUnderIcon)
            self.toolbutton.setText(game[0])

            sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
            #sizePolicy.setHeightForWidth(self.lineEdit.sizePolicy().hasHeightForWidth())


            self.toolbutton.setSizePolicy(sizePolicy)
            img =  Path("data/images/covers/"+ str(game[1]) + ".webp")

            if img.is_file():
                icon = QtGui.QIcon("data/images/covers/"+ str(game[1]) + ".webp")
                print(img)
            else:
                icon = QtGui.QIcon("data/images/covers/blank.webp")
            self.toolbutton.setIconSize(QSize(200, 300));
            self.toolbutton.setIcon(icon)

            self.cvLayout.setAlignment(Qt.AlignmentFlag.AlignLeft)
            self.cvLayout.addWidget(self.toolbutton)
            print(game[0])




