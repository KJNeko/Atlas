import sys, os, csv
from atlas.core.logger import *
from atlas.core.utilities.threading.worker import *
from atlas.core.utilities.threading.pools import *
from atlas.ui.importer.ui_BatchImportDialog import Ui_BatchImportDialog
from atlas.core.utilities.game_detals import *
from pandas import DataFrame


from PySide6.QtCore import QTimer, QRunnable, Slot, Signal, QObject, QThreadPool
from PySide6.QtWidgets import QTableWidgetItem, QComboBox

class game_scanner():

    def __init__(self, ui: Ui_BatchImportDialog, path: str, format: str, size_folders: bool):
        self.ui = ui
        self.path = path
        self.format = format
        self.size_folders = size_folders
       

    #skip list shoudld all be uppercase
    def run(self):        
        logger.debug("Multithreading with maximum %d threads" % ThreadPools.pre_importers.maxThreadCount())
        self.ui.progressBar.show()
        self.threadpool = ThreadPools.pre_importers
        self.worker = Worker(self.start)
        #Set up signals/slots
        self.worker.signals.data.connect(self.update_table)
        self.worker.signals.progress.connect(self.update_progress)
        self.worker.signals.initprogress.connect(self.set_progress_max)
        self.worker.signals.complete.connect(self.scanner_complete)
        ThreadPools.pre_importers.start(self.worker)
            
    def start(self, initprogress_callback: WorkerSignals,  data_callback: WorkerSignals, progress_callback: WorkerSignals, complete_callback: WorkerSignals):   
        try:   
            row = 0 #Table Row
            #Get number of folders in root and set progress bar max to this number.
            folder_num = len(next(os.walk(self.path))[1])
            initprogress_callback.emit(folder_num)

            #This is for auto mode
            if self.format == "":
                root_path = ""
                game_path = ""
                title = ""
                version = ""
                creator = ""
                folder_size = 0

                index = 0
                for subdir, dirs, files in os.walk(self.path):
                    #Store the root dir for each folder that contains an executable
                    if root_path.replace(self.path, "") not in subdir or root_path == "":                     
                        root_path = subdir.replace(self.path, "")
                        progress_callback.emit(index)   
                        index +=1    
        
                    #game_path contains the path to the executable.
                    if game_path not in subdir or game_path == "" and game_path.replace(self.path, "") != root_path:
                        #Verify we have files in the folder
                        if len(files) > 0:
                            #Get Executables
                            potential_executables = executable.detect_executable(files)
                            if len(potential_executables) >0:   
                                game_path = subdir          
                                #logger.info(subdir)               
                                #logger.warn(f'found executable: {potential_executables}')
                                #Now that we found all executables, find the engine
                                file_list = dirs + files
                                game_engine = engine.find_engine(file_list)
                                game_data = details.parse_details(subdir.replace(self.path + "\\", ""))#Make sure to remove \ following path
                                if len(game_data) > 0:
                                    title = game_data[0]
                                    version = game_data[1]
                                    #check for creator
                                    if len(game_data) >2:
                                        creator = game_data[2]
                                data = {'title': title, 'creator' : creator, 'engine' : game_engine, 'version' : version, 'executables' : potential_executables, 'folder' : game_path, 'row' : row, 'folder_size': folder_size}
                                logger.debug(data)
                                data_callback.emit(data)                       
                                row+=1 #increase table row  
                logger.info("Finished Parsing Games")
                complete_callback.emit(0)      
                #When all files are done, Exit
        except Exception as e:
            logger.warn(e)
                
    def update_table(self, s: dict):
        #Verify we have valid data
        if s.get('row') != None:
            table = self.ui.twGames           
            row = s.get('row')
            table.insertRow(row)
            cbExecutables = QComboBox()
            cbExecutables.addItems(s.get('executables'))
            #["Title", "Creator", "Engine", "Version", "Executable", "Folder"]
            table.setItem(row, 0, QTableWidgetItem(s.get('title')))
            table.setItem(row, 1, QTableWidgetItem(s.get('creator')))
            table.setItem(row, 2, QTableWidgetItem(s.get('engine')))
            table.setItem(row, 3, QTableWidgetItem(s.get('version')))
            if len(s.get('executables')) == 1:
                table.setItem(row, 4, QTableWidgetItem(s.get('executables')[0]))
            else:
                table.setCellWidget(row, 4, cbExecutables)
            table.setItem(row, 5, QTableWidgetItem(s.get('folder_size')))
            table.setItem(row, 6, QTableWidgetItem(s.get('folder')))
        
    def scanner_complete(self):
        try:
            self.ui.twGames.resizeColumnsToContents()
            self.ui.progressBar.hide()
            self.ui.statusLabel.setText(f'Finished Processing all games (Found {self.ui.twGames.model().rowCount()} games)')
            #self.table_to_csv()
            #enable import button
            self.ui.btnNext.setEnabled(True)
            logger.debug("Pre-Import Tasks Complete")
        except Exception as e:
            logger.warn(e)
    
    def update_progress(self, s: int):
        self.ui.progressBar.setValue(s)
    
    def set_progress_max(self, s: int):
        #print(f'progress bar max set {s}')
        self.ui.progressBar.setMaximum(s)
    
    def get_folder_size(self, folder: str) -> str:
        total_size = 0
        start_path = folder # To get size of current directory
        for path, dirs, files in os.walk(start_path):
            for f in files:
                fp = os.path.join(path, f)
                total_size += os.path.getsize(fp)                
        return str(round(total_size/1048576,1)) + "MB"
    
    def table_to_csv(self):
        columns = range(self.ui.twGames.columnCount())
        header = [self.ui.twGames.horizontalHeaderItem(column).text()
                    for column in columns]
        with open("games_list.csv", 'w') as csvfile:
            writer = csv.writer(
                csvfile, dialect='excel', lineterminator='\n')
            writer.writerow(header)
            for row in range(self.ui.twGames.rowCount()):
                try:
                    writer.writerow(
                        self.ui.twGames.item(row, column).text()
                        for column in columns)
                except Exception as e:
                    logger.error(e)