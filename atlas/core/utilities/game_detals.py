import sys, os, csv
import re
from difflib import SequenceMatcher
from atlas.core.settings import *
from atlas.core.logger import *

class executable(object):
    skip_x86 = True
    blacklist = ['UnityCrashHandler64.exe',
                 'UnityCrashHandler32.exe',
                 'payload.exe',
                 'nwjc.exe',
                 'notification_helper.exe',
                 'nacl64.exe',
                 'chromedriver.exe',
                 'Squirrel.exe',
                 'zsync.exe',
                 'zsyncmake.exe',
                 'cmake.exe',
                 'pythonw.exe',
                 'python.exe',
                 'dxwebsetup.exe',
                 'README.html',
                 'manual.htm',
                 'unins000.exe',
                 'UE4PrereqSetup_X64.exe',
                 'UEPrereqSetup_x64.exe',
                 'credits.html',
                 'LICENSES.chromium.html',
                 'Uninstall.exe',
                 'CONFIG_dl.exe']
    lin_exe = ['.sh']
    win_exe = ['.exe']
    osx_exe = ['.dmg']
    oth_exe = ['.swf', '.flv', '.f4v', '.rag', '.cmd','.bat','.jar', '.html']
    
    def detect_executable(files: str):
        extensions = executable.get_extensions()
        potential_executables = [] # This has to reset every time we change folders   
        for file in files:
            for extension in extensions:
                if extension.lower().strip() in file.lower().strip():
                    # Verify file is not in skip list
                    if file not in executable.blacklist:

                        if executable.skip_x86 and "-32" in file:
                            break           
                        potential_executables.append(file) 
                        break   
        #!!!NOTE Now that we have all executables, check if there is an exe. If there is, remove everything else !!!
        # NEEDS TO BE TESTED
        if ".exe" in potential_executables:
            potential_executables = [ x for x in potential_executables if ".html" not in x ]
        return potential_executables
    
    def get_extensions() -> str:
        if settings.os == "Windows":
            return executable.win_exe + executable.oth_exe
        if(settings.os == "Linux"):
            return executable.lin_exe + executable.oth_exe

class engine(object):
    def find_engine(file_list):
        engine = "Others"
        #Get a list of files in the engine dir and iterate through all of the possible engine types
        for engine_file in os.listdir(settings.engines_path):
            engine_path = os.path.join(settings.engines_path, engine_file)
            #open each file and read contents
            with open(engine_path) as f:
                attrs = f.readlines()
                for attr in attrs:
                    for file in file_list:  
                        if file not in executable.blacklist:                     
                            if attr.strip().lower() in str(file).lower():                               
                                #Because html files can be in each folder we need to skip it before returning
                                engine = engine_file.split('.')[1]
                                if ".html" not in attr.strip().lower() :
                                    return engine
        #This will return HTML if found, if not, it will return OTHERS
        return engine

class details(object):
    def parse_details( s: str) -> dict:
        #Check len of the arry of itsm in the folder string
        text = s.split('\\')
        data = {}
        if len(text) == 1:
            data = details.parse_single_filename(text[0])
        # There are two possilbe solutions for len = 2. Either the folder was extracted and there is a copy of the top folder or 
        # its title/version
        if len(text) == 2:
            #Check to see if both strings are close
            if details.check_for_similar_strings(text[0], text[1]) > 0.80:
                data = details.parse_single_filename(text[1])
            else:
                #assume Title, Version for now
                data = [text[0],text[1]]
        if len(text) > 2:
            data = [text[1],text[2], text[0]]
        #Try to use regex to parse data. We are assuming the following layouts
        # Creator/Title/Version
        # 
        #logger.info(text)
        return data
   
    def parse_single_filename(s: str):
        #Set Defaults
        file_data = {} #dict
        if '-' in s:
            file_data = details.parse_string_by_delimeter( s, "-" )
        else:
            if '_' in s:
                file_data = details.parse_string_by_delimeter( s, "_" ) 
            else: 
                if " " in s:                
                    file_data = details.parse_string_by_delimeter( s, " " )
                else:
                    #If we have made it here then we are asuming its just the game title with nothing else.
                    file_data = [details.add_spaces( s ), ""]

        return file_data

    def parse_string_by_delimeter(s:str, delimiter:str):
        title = ""
        version = ""
        slist = s.split(delimiter)
        is_version = False
        for index in range(len(slist)):        
            item = slist[ index ]
            if " " in item:
                temp = details.parse_single_filename(item)
                title = temp[0]
                version = temp[1]
            #Assume first item will always be a part of the title.
            #Check if it has a number or has an OS name
            else:
                if not details.check_os_names( item ) and not details.check_languages( item ):                  
                    if not ( index > 0 and details.is_digit( item ) ) and is_version == False:
                    
                        #Check if string contains a version type
                        version_result = details.find_version_type( item )

                        title += version_result[ 0 ]
                        version += version_result[ 1 ]
                        #Check for version chapter or season                    
                    else:                    
                        is_version = True
                        version += item
                        #Add Spaces between Capital Letters before returning the title
        return [ details.add_spaces( title ), version ]

    def check_os_names( s:str) -> bool:
        os_arr = {"pc", "win", "linux", "windows", "unc", "win64"}
        if s.lower() in os_arr:
            return True
        else:
            return False
    
    def check_languages( s:str) -> bool:
        os_arr = {"japanese", "english"}
        if s in os_arr:
            return True
        else:
            return False
    
    def is_digit( s:str) -> bool:
        return any(chr.isdigit() for chr in s)
    
    def add_spaces( s: str ):
        return re.sub(r"(\w)([A-Z])", r"\1 \2", s)

    def find_version_type( s: str):
        title = ""
        version = ""
        delimiters = {"final", "episode", "chapter", "version", "season", "v."}
        for item in delimiters:
            if item in s.lower():
                version = s
            else:
                title = s        
        return [title, version]
    
    def check_for_similar_strings(s1:str, s2:str):
        return SequenceMatcher(None, s1, s2).ratio()
    
  