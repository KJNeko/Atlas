import os
from atlas.core.settings import *
from atlas.core.logger import *
class game_scanner(object):
    def start(path: str, format: str, size_folders: bool ):        
        #We need to run this in a QThread Make Qrunnable
        #set executable type
        exe_type = game_scanner.get_executable_type()

        if format == "":
            logger.debug(f'Path: {path}')
            for subdir, dirs, files in os.walk(path):
                for file in files:
                    logger.info(os.path.join(subdir, file))
            #We are in automode, try to guess the game based on the engine. 
            return

        
    def get_executable_type() -> str:
        if settings.os == "Windows":
            return "exe"
        if(settings.os == "Linux"):
            return "sh"
        