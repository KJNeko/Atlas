import os
from atlas.core.settings import *
from atlas.core.logger import *
from atlas.core.logger import *
from atlas.core.utilities.threading import *
from atlas.ui.importer.ui_BatchImportDialog import Ui_BatchImportDialog
from pandas import DataFrame

from PySide6.QtCore import QTimer, QRunnable, Slot, Signal, QObject, QThreadPool
from PySide6.QtWidgets import QTableWidgetItem, QComboBox

class game_scanner():
    def __init__(self, ui: Ui_BatchImportDialog, path: str, format: str, size_folders: bool):
        self.ui = ui
        self.path = path
        self.format = format
        self.size_folders = size_folders
        self.skip_x86 = True
        self.skip_list = ['UnityCrashHandler64.exe','UnityCrashHandler32.exe', 'README.html','manual.htm','unins000.exe','credits.html', 'LICENSES.chromium.html']
        self.lin_exe = ['.sh']
        self.win_exe = ['.exe']
        self.osx_Exe = ['.dmg']
        self.oth_exe = ['.swf', '.flv', '.f4v', '.rag', 'html']

    #skip list shoudld all be uppercase
    def run(self):
        self.threadpool = QThreadPool()
        print("Multithreading with maximum %d threads" % self.threadpool.maxThreadCount())
        self.ui.progressBar.show()
        worker = Worker(self.start)
        #worker.signals.result.connect(self.print_output)
        #worker.signals.finished.connect(self.thread_complete)
        worker.signals.data.connect(self.update_table)
        worker.signals.progress.connect(self.update_progress)
        worker.signals.initprogress.connect(self.set_progress_max)
        self.threadpool.start(worker)
            
    def start(self, initprogress_callback: WorkerSignals,  data_callback: WorkerSignals, progress_callback: WorkerSignals):        
        #We need to run this in a QThread Make Qrunnable
        #set executable type
        row = 0 
        executable_list = self.get_executable_type()

        #Get number of folders in root and set progress bar max to this number.
        initprogress_callback.emit(len(next(os.walk(self.path))[1]))

        if self.format == "":
            root_path = ""
            game_path = ""
            engine = ""
            index = 0
            logger.debug(f'os:{settings.os} exe:{executable_list}')
            logger.debug(f'Path: {self.path}')
            for subdir, dirs, files in os.walk(self.path):
                #logger.warn(subdir)
                executables = [] # This has to reset every time we change folders     
                #logger.warn(f'{t_root} not in {subdir}') 
                if root_path.replace(self.path, "") not in subdir or root_path == "":
                     
                    root_path = subdir.replace(self.path, "")
                    progress_callback.emit(index)
                    index+=1
                    folder_size = 0
                    if root_path != "":
                        folder_size = self.get_folder_size(subdir)
                        #logger.debug(subdir) 
                      
      

                if game_path not in subdir or game_path == "":
                    #Set root path
                    if len(files) > 0:
                        for file in files:
                            #Verify file array has files 
                            for executable in executable_list:
                                if executable in file:
                                    # Verify file is not in skip list
                                    if file not in self.skip_list:
                                        #Add both lists together so we can verify engine type
                                        list = files + dirs
                                        #logger.warn(subdir)
                                        engine = self.find_engine(list)
                                        #logger.info(engine)
                                        #logger.debug(files)
                                        #logger.debug(dirs)
                                        #logger.debug(file)
                                        game_path = subdir
                                        #Check if we are skipping 32bit file types
                                        #if self.skip_x86 and '-32' not in file:
                                        game_data = self.parse_data(subdir.replace(self.path, ""))
                                        if self.skip_x86 and "-32" in file:
                                            break
                                        line = f'data row: {row}, Title: test, Creator: test, Engine: {engine}, Version: 0.0, Executable: {file}, Folder: {game_path}, FolderSize": {folder_size}'
                                        logger.debug(line)                
                                        executables.append(file)                                        
                                        break
                #Try to add item to ui
                        if len(executables) > 0:
                            data = {'title': 'test', 'creator' : 'test', 'engine' : engine, 'version' : '0.0', 'executables' : executables, 'folder' : game_path, 'row' : row, 'folder_size': folder_size}
                            data_callback.emit(data)                       
                            row+=1 #increase table row  

            self.thread_complete()

    def get_executable_type(self) -> str:
        if settings.os == "Windows":
            return self.win_exe + self.oth_exe
        if(settings.os == "Linux"):
            return self.lin_exe + self.oth_exe
        
    def find_engine(self, file_list):
        engine = "Others"
        #get a list of files in the engine dir and iterate through all of the possible engine types
        for engine_file in os.listdir(settings.engines_path):
            engine_path = os.path.join(settings.engines_path, engine_file)
            #open each file and read contents
            with open(engine_path) as f:
                attrs = f.readlines()
                for attr in attrs:
                    for file in file_list:  
                        if file not in self.skip_list:                     
                            if attr.strip().lower() in str(file).lower():                               
                                #Because html files can be in each folder we need to skip it before returning
                                engine = engine_file.split('.')[1]
                                if ".html" not in attr.strip().lower() :
                                    return engine
        #This will return HTML if found, if not, it will return OTHERS
        return engine
                        
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
        

    def thread_complete(self):
        self.ui.twGames.resizeColumnsToContents()
        print("THREAD COMPLETE!")
    
    def update_progress(self, s: int):
        self.ui.progressBar.setValue(s)
    
    def set_progress_max(self, s: int):
        #print(f'progress bar max set {s}')
        self.ui.progressBar.setMaximum(s)
    
    def parse_data(self, folder: str) -> dict:
        #Try to use regex to parse data. We are assuming the following layouts
        # Title/Version
        # Creator/Title/Version
        # 
        #print(folder)
        return
    
    def get_folder_size(self, folder: str) -> str:
        total_size = 0
        start_path = folder # To get size of current directory
        for path, dirs, files in os.walk(start_path):
            for f in files:
                fp = os.path.join(path, f)
                total_size += os.path.getsize(fp)                
        return str(round(total_size/1048576,1)) + "MB"
        