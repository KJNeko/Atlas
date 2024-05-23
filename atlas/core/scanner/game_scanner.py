import os
from atlas.core.settings import *
from atlas.core.logger import *
class game_scanner(object):
    def start(path: str, format: str, size_folders: bool ):        
        #We need to run this in a QThread Make Qrunnable
        #set executable type
        exe_type = game_scanner.get_executable_type()

        if format == "":
            logger.debug(f'os:{settings.os} exe:{exe_type}')
            logger.debug(f'Path: {path}')
            for subdir, dirs, files in os.walk(path):
                #logger.warn(dirs)
                if len(files) > 0:
                    #logger.error(files)
                    for file in files:
                        #Verify file array has files                      
                        logger.error(file)
                        if exe_type in file:
                            #Add both lists together so we can verify engine type
                            list = files + dirs
                            logger.debug(list)
                            return
                #    logger.info(os.path.join(subdir, file))
            #We are in automode, try to guess the game based on the engine. 


        
    def get_executable_type() -> str:
        if settings.os == "Windows":
            return ".exe"
        if(settings.os == "Linux"):
            return ".sh"
        
    #def find_engine(file_list):

        