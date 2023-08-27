import os
import sqlite3 as sl
import requests
import re
import shutil
from datetime import datetime
from urllib.parse import urlparse
import time
from csv import writer
import random
from pathlib import Path


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


#dbPath = "C:/Users/tower/source/repos/Atlas/build/bin/data/atlas.db"
dbPath = "C:/Users/bgibson/Source/Atlas/build/bin/data/atlas.db"

# Format is /developer/name/version
# Put this py file in the same folder as your "Atlas" directory
# Make sure Atlas.db is also in the same folder
# Change the path below to where you games directory is
base_dir = "C:/games"


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
            print(
                bcolors.HEADER
                + str(datetime.now())
                + " : DEBUG   [FOUND MUTIPLE DATABASE ENTIRES]"
                + bcolors.ENDC
            )
            print(
                "\nFound "
                + str(len(data))
                + " possible matches. Select from the list below:"
            )
            for index, item in enumerate(data):
                if int(item[3]) <= 10:
                    print(str(index) + " : " + str(item))
            id = input()
            atlas_id = data[int(id)][0]
            return atlas_id
        else:
            print(
                bcolors.HEADER
                + str(datetime.now())
                + " : DEBUG   [ FOUND SINGLE DATABASE ENTRY  ]"
                + bcolors.ENDC
            )
            atlas_id = data[0][0]
            return atlas_id

    else:
        return None


def createCSV(title, developer, version):
    data = [developer, title, version]
    with open("data.csv", "a", newline="") as f_object:
        writer_object = writer(f_object)
        writer_object.writerow(data)
        f_object.close()


def getPreviews(atlas_id, dir):
    query = "SELECT screens FROM f95_zone_data WHERE atlas_id='" + str(atlas_id) + "'"
    cursor.execute(query)
    previews = cursor.fetchone()[0]
    for preview_url in previews.split(","):
        # Skip cover image in preview
        if "cover" not in preview_url and "header" not in preview_url:
            downloadPreviewImage(dir, preview_url)


def getBannerImage(atlas_id, dir):
    query = (
        "SELECT banner_url FROM f95_zone_data WHERE atlas_id='" + str(atlas_id) + "'"
    )
    cursor.execute(query)
    banner_url = cursor.fetchone()[0]
    try:
        downloadImage(dir, banner_url)
    except:
        print(
            bcolors.FAIL
            + str(datetime.now())
            + " : ERROR [UNABLE TO DOWNLOAD IMAGE]"
            + bcolors.ENDC
        )


def downloadImage(dir, url):
    # Generate random time for sleep. Should help to keep server from stressing
    seconds = random.uniform(0.2, 1)
    ext = os.path.splitext(os.path.basename(urlparse(url).path))[1]
    image_path = os.path.join(dir, "banner" + ext)

    if not os.path.exists(image_path):
        print(
            bcolors.HEADER
            + str(datetime.now())
            + " : DEBUG   [DOWNLOADING BANNER IMAGE] -> "
            + bcolors.ENDC
            + url
        )
        image = requests.get(url, stream=True)
        time.sleep(seconds)
        if image.status_code == 200:
            with open(image_path, "wb") as f:
                shutil.copyfileobj(image.raw, f)


def downloadPreviewImage(dir, url):
    seconds = random.uniform(0.2, 1)
    filename = os.path.basename(urlparse(url).path)
    image_path = os.path.join(dir, filename)
    # print(image_path)
    if not os.path.exists(image_path):
        print(
            bcolors.HEADER
            + str(datetime.now())
            + " : DEBUG   [DOWNLOADING PREVIEW IMAGE] -> "
            + bcolors.ENDC
            + url
        )
        image = requests.get(url, stream=True)
        time.sleep(seconds)
        if image.status_code == 200:
            with open(image_path, "wb") as f:
                shutil.copyfileobj(image.raw, f)
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
        + " : ["+type +"] "+
        + bcolors.ENDC
        + message
    )
        
def downloadData(title):
    atlas_id = getAtlasId(title)
    if atlas_id == None:
        print(
            bcolors.WARNING
            + str(datetime.now())
            + " : WARNING [  ENTRY NOT IN DATABASE ] -> "
            + bcolors.ENDC
            + "Developer: "
            + developer
            + " | Title: "
            + title
            + " | Version: "
            + version
        )
    else:
        try:
            getBannerImage(atlas_id, version_folder)
        except:
            print(
                bcolors.FAIL
                + str(datetime.now())
                + " : ERROR   [ ERROR GETTING BANNER IMAGE  ] -> "
                + bcolors.ENDC
                + "Developer: "
                + developer
                + " | Title: "
                + title
                + " | Version: "
                + version
            )
        try:
            getPreviews(atlas_id, previews_folder)
        except:
            print(
                bcolors.FAIL
                + str(datetime.now())
                + " : ERROR   [ ERROR GETTING PREVIEW IMAGES  ] -> "
                + bcolors.ENDC
                + "Developer: "
                + developer
                + " | Title: "
                + title
                + " | Version: "
                + version
            )


# MAIN ENTRY POINT FOR PROGRAM
if checkDataBase(dbPath):
    # Use nomal path type Createor/Title/Version
    # Get a list of folders from games path
    for developer in os.listdir(base_dir):
        dev_folder = os.path.join(base_dir, developer)
        # checking if it is a folder
        if os.path.isdir(dev_folder):
            for title in os.listdir(dev_folder):
                title_folder = os.path.join(dev_folder, title)
                if os.path.isdir(title_folder):
                    for version in os.listdir(title_folder):
                        version_folder = os.path.join(title_folder, version)
                        # createCSV(title, developer, version)
                        if os.path.isdir(version_folder):
                            previews_folder = os.path.join(version_folder, "previews")
                            # Create previews folder if it does not exist
                            if not os.path.exists(previews_folder):
                                os.mkdir(previews_folder)
                            print_c(bcolors.HEADER, "INFO", "CHECKING DATABASE FOR MATCH", title, developer, version)
                            
else:
    print_c(bcolors.HEADER, "INFO", "CHECKING DATABASE FOR MATCH", title, developer, version)
    print(
        bcolors.FAIL
        + str(datetime.now())
        + " : ERROR   [ DATABASE PATH INCORRECT ] -> "
        + bcolors.ENDC
    )
