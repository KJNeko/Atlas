import os
from atlas.core.settings import *
from atlas.core.logger import *
class game_scanner(object):

    skip_list = ['UnityCrashHandler64.exe', '-32.exe']
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
                            #logger.warn(subdir)
                            #logger.error(file)
                            #Verify file array has files 
                            for executable in executable_list:
                                if executable in file:
                                    # Verify file is not in skip list
                                    if file not in game_scanner.skip_list:
                                        #Add both lists together so we can verify engine type
                                        list = files + dirs
                                        logger.warn(subdir)
                                        game_scanner.find_engine(list)
                                        logger.debug(list)
                                        root_path = subdir
                                        break
                            else:
                                continue
                        else:
                            continue
                        break
                        
                        

                #    logger.info(os.path.join(subdir, file))
            #We are in automode, try to guess the game based on the engine. 


        
    def get_executable_type() -> str:
        if settings.os == "Windows":
            return ['.exe']
        if(settings.os == "Linux"):
            return ['.sh']
        
    def find_engine(file_list):
        #get a list of files in the engine dir
        for file in os.listdir(settings.engines_path):
            engine = file.split('.')[1]
            #logger.warn(engine)
            engine_path = os.path.join(settings.engines_path, file)
            #print(engine_path)
            with open(engine_path) as f:
                attrs = f.readlines()
                for attr in attrs:
                    for en in file_list:
                        #logger.error(f'{attr} in {en}')
                        if attr.strip() in en:
                            logger.info(engine)
                        
                        
                        
                
        