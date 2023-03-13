#Python Libraries
import sys
import os
from glob import glob
from media.Hydrus95ImageDownloader import *
import re

#Custom Libraries
#from gameImporter import *
from db.ClientFunctions import *
#from imageDownloader import *

from PyQt6 import QtCore, QtGui, QtWidgets
from PyQt6.QtWidgets import QPushButton, QToolButton, QFileDialog, QTableWidget, QMainWindow, QDialog
from PyQt6 import uic

class ImportDialog(QDialog):
    games_added = QtCore.pyqtSignal()
    page_counter = 0 #used to keep track of what page we are on in the import diag
    ext = "" #game extension
    path_format = ""
    folders = ""

    def __init__(self):
        super().__init__()
        uic.loadUi("ui/dialog/UIImportGames.ui", self)
        self.btnNext.clicked.connect(self.clk_btnNext)
        self.pbtest.clicked.connect(self.clk_test)
        self.twGames.setColumnWidth(0, 125);
        self.twGames.setColumnWidth(1, 370);
        self.twGames.setColumnWidth(2, 65);
        self.twGames.setColumnWidth(3, 75);

    def clk_test(self):
        self.games_added.emit()
    def clk_btnAdd(self):
        folderName = QFileDialog.getExistingDirectory(self)
        if(folderName != None):
            self.lwImports.addItem(folderName)

    def clk_btnNext(self):
        self.page_counter +=1
        if(self.page_counter ==1):
            self.folders = self.tbPath.toPlainText()
            self.path_format = self.tbFormat.toPlainText()
            self.ext = self.tbExt.toPlainText()
            self.getGameInfo()

        self.swImportGames.setCurrentIndex(self.page_counter)
        print("Page:" + str(self.page_counter))

    def updateDatabaseWithGameInfo(self,name, shortname, game, version, game_path):

        #VARIABLES
        f95_id = ""
        exec_path = game
        in_place = 0 #default
        last_played = "0"
        version_playtime ="0"
        title = ""
        developer =""
        engine = ""
        last_played_record="0"
        total_playtime="0"

        #STEP 1: CHECK TO SEE IF GAME LOCATION IS ALREADY IN THE DB
        row = checkIfgameinstalled(game_path)
        if(row == None):
            print("Adding Game to db")

            #STEP 2: CHECK TO SEE IF GAME IS IN F95db
            result = FindF95zoneRecord(shortname)
            rowCount = len(FindF95zoneRecord(shortname).fetchall()) #Get row count. if more than 1 the user needs to select it
            if rowCount >0:
                #need to add a check for if multiple rows are returned
                for data in result:
                    f95_id=data[0]
                    title =data[1]
                    developer=data[2]
                    engine=data[3]
                    banner_link = data[4]
                    print(data)
                    #download image
                    #getF95BannerImage(f95_id,banner_link)
            else:
                title =name

            print(title)

            #STEP3: CHECK IF GAME RECORD ALREADY EXIST
            print(getRecordTitle(title))
            if getRecordTitle(title) != None:
                record_id = getRecordTitle(title)[0]
            else:
                record_id = getLastIdUsed()
                if record_id == None:
                    record_id=0
                else:
                    record_id+=1
            print("record_id", record_id)

            #STEP 4: Download Images


            if f95_id != "":
               getF95BannerImage(record_id,banner_link)
               addf95mapping(f95_id, record_id)

            addGameRecord(record_id, title, developer, engine, last_played_record, total_playtime)
            #addGameMetadata()
        else:
            print("game already in db")

        #addGame(record_id, str(f95_id), version, game_path, exec_path, in_place, last_played, version_playtime,
        #title, developer, engine, last_played_record,total_playtime )

    def getGameInfo(self):
        for folder in os.listdir(self.folders):
            if("-" in self.path_format):
                #try:
                tmp = folder.split('-')
                if(len(tmp) >1):
                    shortname = re.sub('[\W_]+', '',tmp[0].strip().replace(" ","")).upper()
                    name = tmp[0].replace("_"," ")
                    version = tmp[1].replace("_"," ")
                    game_path = os.path.join(self.folders,folder)
                    games = glob(os.path.join(game_path, ("*"+self.ext)))
                    for game in games:
                        if("-32" not in game):
                            #Add to table
                            rowPosition = self.twGames.rowCount()
                            self.twGames.insertRow(rowPosition)
                            self.twGames.setItem(rowPosition , 0, QtWidgets.QTableWidgetItem(name))
                            self.twGames.setItem(rowPosition , 1, QtWidgets.QTableWidgetItem(game))
                            self.twGames.setItem(rowPosition , 2, QtWidgets.QTableWidgetItem(version))
                            self.twGames.setItem(rowPosition , 3, QtWidgets.QTableWidgetItem(self.ext))
                            self.updateDatabaseWithGameInfo(name, shortname, game, version, game_path) #use shortname to match with f95. Will use diff method later. this is easiest for now.
        self.games_added.emit()
        #self.close()
                #except Exception as e:
                    #print("Folder name is not in correct:" +folder)
