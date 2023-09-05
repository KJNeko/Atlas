import os
import sqlite3 as sl
import re
from datetime import datetime
import time

#OUTPUT COLORS
class bcolors:
    HEADER = "\033[95m"
    OKBLUE = "\033[94m"
    OKCYAN = "\033[96m"
    OKGREEN = "\033[92m"
    WARNING = "\033[93m"
    FAIL = "\033[91m"
    ENDC = "\033[0m"
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"

#USE / for folder delimiter
rootdir = "F:/_tmp_Games"
extractdir = "E:/atlas_extraction_test"
dbPath = "C:/Users/tower/source/repos/Atlas/build/bin/data/atlas.db"

#GLOBAL VARS
enableCreatorDirs = False

def checkDataBase(dbPath):
    #path = os.path.join(Path(os.getcwd()).parent.absolute(), dbName)
    print(dbPath)
    print("DATABASE FOUND: " + str(os.path.isfile(dbPath)))
    return os.path.isfile(dbPath)

def getAtlasId(title):
    con = sl.connect(dbPath)
    cursor = con.cursor()
    short_name = re.sub("[\W_]+", "", title.strip().replace(" ", "")).upper()
    query = (
        "Select atlas_id, title, creator, LENGTH(short_name) - LENGTH('"
        + short_name
        + "') as Difference from atlas_data WHERE short_name like '%"
        + short_name
        + "%' Order By LENGTH(short_name) - LENGTH('"
        + short_name
        + "')"
    )
    # print(query)
    # query = "SELECT atlas_id FROM atlas_data WHERE short_name='" + short_name + "'"
    cursor.execute(query)
    data = cursor.fetchall()
    # print(len(data))
    if len(data) > 0:
        if len(data) > 1:
            print_c(bcolors.HEADER, "DEBUG", "FOUND MUTIPLE DATABASE ENTIRES")
            
            print(
                "\nFound "
                + str(len(data))
                + " possible matches. Enter number from list below or Custom Name and press enter:"
            )
            for index, item in enumerate(data):
                #if int(item[3]) <= 10:
                print(str(index) + " : " + str(item))
            id = input()
            if id.isnumeric():
                atlas_id = data[int(id)][0]
                return atlas_id
            else:
                return id;
        else:
            print_c(bcolors.HEADER, "DEBUG", "FOUND SINGLE DATABASE ENTRY")
            print_c(bcolors.HEADER, "INFO", data[0][1])
            if(data[0][3] < 2):
                atlas_id = data[0][0]
                return atlas_id
            else:
                print("If the above title is correct, PRESS ENTER. If it is not correct, enter new title")
                name = input()           

    else:
        return None

#OVERIDE
def print_c(color, type, message, title, developer, version):
        print(
        color
        + str(datetime.now())
        + " : ["+type +"] "+
        + bcolors.ENDC
        + message + " "
        + "Developer: "
        + developer
        + " | Title: "
        + title
        + " | Version: "
        + version
    )
        
def print_c(color, type, message):
        print(
        color
        + str(datetime.now())
        + " : ["+type +"] "
        + bcolors.ENDC
        + message
    )

def checkForCommonNames(str):
    #convert to lower and remove "_"
    str = str.lower().replace('_', "")
    arr = ["pc","linux","win","windows"]
    return any(str == c for c in arr)


def parseFileName():
    #CREATE FOLDER TO STORE ALL GAMES AFTER THEY ARE EXTRACTED. WE ARE NOT DELETING THEM AT THE MOMENT
    if not os.path.exists(os.path.join(rootdir, "_complete_")):
        os.makedirs(os.path.join(rootdir, "_complete_"))
    myfile = open('files.txt', 'w')
    for subdir, dirs, files in os.walk(rootdir):
        for file in files:
            #os.system('cls') #clear console
            #print("-----------------------------")
            myfile.write(file + "\n");
            ext = os.path.splitext(file)[1]
            if(ext.lower() == ".zip" or ext.lower() == ".7z" or ext.lower() == ".rar"):    
                #REMOVE Extension
                file_name = os.path.splitext(file)[0]       
                title = ""
                version = ""
                hasNum = False
                hasWord = False
                verArr = ["season", "episode", "chapter"]
                #Check for "-" This is the first test case
                if('-' in file_name):
                    tmp = file_name.split('-')
                    #Construct title
                    #Skip the first item
                    for i in range(0, len(tmp)):
                        #We have to assume the first entry will always be the game name.
                        #Check for numbers
                        if i > 0 and (any(chr.isdigit() for chr in tmp[i])):  
                            version += tmp[i]
                            #if we found numbers then we should try and skip everything else.
                            hasNum = True
                        else:
                            #specific cases towards the end of the the filename that need to be skipped
                            #Check for episode or season after and store in version
                            if tmp[i].lower() == "season" or tmp[i].lower() == "episode" or tmp[i].lower() == "chapter":
                                version += tmp[i] + " "
                                hasWord = True
                            if( not checkForCommonNames(tmp[i]) and hasNum == False and hasWord == False):
                                #At this point we need to check for a few things, 
                                #1) Check if season or episode is in the name
                                #2) Check if Chapter or CHX is in the title
                                if("episode" in tmp[i].lower()):
                                    ep = tmp[i].lower().split('episode')
                                    if(len(ep) > 1):
                                        version += "Episode" + ep[1]
                                        title += ep[0]
                                    else:
                                        version += "Episode"
                                        title += ep[0]
                                if("season" in tmp[i].lower()):
                                    se = tmp[i].lower().split('season')
                                    if(len(se) > 1):
                                        version += "season" + se[1]
                                        title += se[0]
                                    else:
                                        version += "season"
                                        title += se[0]
                                if("final" in tmp[i].lower()):
                                    fi = tmp[i].lower().split('final')
                                    if(len(fi) > 1):
                                        version += "final" + fi[1]
                                        title += fi[0]
                                    else:
                                        version += "final"
                                        title += fi[0]
                                else:    
                                    title += tmp[i]
                               
                    print_c(bcolors.HEADER,"DEBUG", ("FILE -> " + file))
                    print_c(bcolors.OKGREEN,"INFO", ("TITLE -> " + title))
                    print_c(bcolors.OKGREEN,"INFO", ("VERSION -> " + version))
                #else:
                #    print_c(bcolors.WARNING,"WARNING", ("FILE -> " + file))
                #print_c(bcolors.HEADER,"INFO", ("Looking for title " + title))
                    #getAtlasId(title)
            #print("-----------------------------")
    myfile.close()
                


#PROGRAM ENTRY POINT
if checkDataBase(dbPath):
    parseFileName()
    #extractGames()
