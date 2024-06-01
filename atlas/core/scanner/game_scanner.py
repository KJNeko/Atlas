import os
from atlas.core.settings import *
from atlas.core.logger import *
class game_scanner(object):

    #skip list shoudld all be uppercase
    skip_list = ['UnityCrashHandler64.exe','UnityCrashHandler32.exe', 'README.html','manual.htm','unins000.exe','credits.html']
    def start(path: str, format: str, size_folders: bool ):        
        #We need to run this in a QThread Make Qrunnable
        #set executable type
        executable_list = game_scanner.get_executable_type()

        if format == "":
            exe_found = False
            root_path = ""
            logger.debug(f'os:{settings.os} exe:{executable_list}')
            logger.debug(f'Path: {path}')
            for subdir, dirs, files in os.walk(path):
                if root_path not in subdir or root_path == "":
                    #logger.debug(root_path)
                    if len(files) > 0:
                        for file in files:
                            #Verify file array has files 
                            for executable in executable_list:
                                if executable in file:
                                    # Verify file is not in skip list
                                    if file not in game_scanner.skip_list:
                                        #Add both lists together so we can verify engine type
                                        list = files + dirs
                                        logger.warn(subdir)
                                        engine = game_scanner.find_engine(list)
                                        logger.info(engine)
                                        logger.debug(files)
                                        logger.debug(dirs)
                                        logger.debug(file)
                                        root_path = subdir
                                        break
                            else:
                                continue

                        
                        

                #    logger.info(os.path.join(subdir, file))
            #We are in automode, try to guess the game based on the engine. 


        
    def get_executable_type() -> str:
        lin = ['.sh']
        win = ['.exe']
        osx = ['.dmg']
        ex = [ '.swf', '.flv', '.f4v', '.rag', 'html']
        if settings.os == "Windows":
            return win + ex
        if(settings.os == "Linux"):
            return lin + ex
        
    def find_engine(file_list):
        engine = "Others"
        #get a list of files in the engine dir and iterate through all of the possible engine types
        for file in os.listdir(settings.engines_path):
            engine_path = os.path.join(settings.engines_path, file)
            #open each file and read contents
            with open(engine_path) as f:
                attrs = f.readlines()
                for attr in attrs:
                    for en in file_list:
                        #logger.error(f'{attr} in {en}')
                        if attr.strip() in en:     
                            #Because html files can be in each folder we need to skip it before returning
                            #return first engine found
                            engine = file.split('.')[1]
                            if attr.strip() != ".html":
                                return engine
        #This will return HTML if found, if not, it will return OTHERS
        return engine
                        
                        
                        
                
        