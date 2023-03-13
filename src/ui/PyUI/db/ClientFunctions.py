import os
import sqlite3 as sl
import requests
from pathlib import Path

localdbName = "hydrus95.db"
remotedbName = "data.db"
#check if database exist, if it does not then create it
def deleteLocalDatabase():
     if(Path((localdbName)).is_file() == True):
        print("Deleting Database")
        os.remove(localdbName)

def createLocalDatabase():
    if(Path((localdbName)).is_file() == False):
        print("Creating Database")
        con = sl.connect(localdbName)
        with con:
            con.execute("""
                CREATE TABLE settings (
                    id INTEGER NOT NULL PRIMARY KEY,
                    game_dir TEXT NOT NULL
                );
            """)
            con.execute("""
                CREATE TABLE records (
                    record_id INT NOT NULL PRIMARY KEY,
                    title TEXT NOT NULL,
                    developer TEXT NOT NULL,
                    engine TEXT NOT NULL,
                    last_played_record DATETIME,
                    total_playtime INT
                );
            """)            
            con.execute("""
                CREATE TABLE game_metadata (
                    record_id INT NOT NULL PRIMARY KEY,
                    version TEXT NOT NULL,
                    game_path TEXT NOT NULL,
                    exec_path TEXT NOT NULL,
                    in_place bool NOT NULL,
                    last_played DATETIME,
                    version_playtime INT
                );
            """)
            con.execute("""
                CREATE TABLE images (
                    record_id INT NOT NULL PRIMARY KEY,
                    type INT,
                    sha256 TEXT
                );
            """)
            con.execute("""
                CREATE TABLE f95zone_mapping (
                    record_id INT NOT NULL PRIMARY KEY,
                    f95_id INT
                );
            """)
            con.execute("""
                CREATE TABLE f95zone_data (
                    f95_id TEXT NOT NULL UNIQUE PRIMARY KEY,
                    short_name TEXT NOT NULL,
                    other TEXT,
                    engine TEXT,
                    banner_url TEXT,
                    title TEXT NOT NULL,
                    status TEXT,
                    version TEXT NOT NULL,
                    developer TEXT,
                    site_url TEXT,
                    overview TEXT,
                    thread_update DATE,
                    release_date DATE,
                    censored TEXT,
                    language TEXT,
                    translations TEXT,
                    length TEXT,
                    vndb TEXT,
                    genre TEXT,
                    voice TEXT,
                    os TEXT,
                    other_games TEXT,
                    views TEXT,
                    likes TEXT,
                    tags TEXT,
                    rating TEXT,
                    screens TEXT,
                    last_update TEXT
                );
            """)
    else:
        print("Database Exist")

def createRemoteDatabase():
    if(Path((remotedbName)).is_file() == False):
        print("Creating Database")
        con = sl.connect(dbName)
        with con:
            con.execute("""
                CREATE TABLE f95zone_data (
                    f95_id TEXT NOT NULL UNIQUE PRIMARY KEY,
                    short_name TEXT NOT NULL,
                    other TEXT,
                    engine TEXT,
                    banner_url TEXT,
                    title TEXT NOT NULL,
                    status TEXT,
                    version TEXT NOT NULL,
                    developer TEXT,
                    site_url TEXT,
                    overview TEXT,
                    thread_update DATE,
                    release_date DATE,
                    censored TEXT,
                    language TEXT,
                    translations TEXT,
                    length TEXT,
                    vndb TEXT,
                    genre TEXT,
                    voice TEXT,
                    os TEXT,
                    other_games TEXT,
                    views TEXT,
                    likes TEXT,
                    tags TEXT,
                    rating TEXT,
                    screens TEXT,
                    last_update TEXT
                );
            """)
    else:
        print("Database Exist")


def addGame(record_id, f95_id, version, game_path, exec_path, in_place, last_played, version_playtime,title, developer, engine, last_played_record,total_playtime):
    con = sl.connect(localdbName)
    cursor = con.cursor()

    #Check to see if the file already exist in the database, if it does then do not continue
    cursor.execute("""SELECT game_path FROM game_metadata WHERE game_path like '"""+game_path +"""'""")
    row = cursor.fetchone()
    if(row == None):
        sql = """INSERT OR REPLACE INTO game_metadata
        (record_id, version, game_path, exec_path, in_place, last_played, version_playtime)
        VALUES (?,?,?,?,?,?,?);"""
        parameters = (record_id, version, game_path, exec_path, in_place, last_played, version_playtime,)
        cursor.execute(sql,parameters)
        con.commit()
        #f95zone_mapping
        if f95_id != "":
            sql = """INSERT OR REPLACE INTO f95zone_mapping
            (f95_id, record_id)
            VALUES (?,?);"""
            parameters = (f95_id, record_id,)
            cursor.execute(sql,parameters)
            con.commit()
        #records
        sql = """INSERT OR REPLACE INTO records
        (record_id, title, developer, engine, last_played_record, total_playtime)
        VALUES (?,?,?,?,?,?);"""
        parameters = (record_id, title, developer, engine, last_played_record, total_playtime,)
        cursor.execute(sql,parameters)
        con.commit()
    else:
        print("Game already in the database")
    cursor.close()

def getLastIdUsed():
    con = sl.connect(localdbName)
    cursor = con.cursor()
    with con:
        cursor.execute("SELECT record_id FROM records ORDER BY record_id DESC LIMIT 1")
        row = cursor.fetchone()
        cursor.close()
        if(row == None):
            return None
        else:
            return row[0]


def updateF95zoneTableFull(f95_id, short_name, other, engine, banner_url, title, status, version, developer,
    site_url, overview, thread_update, release_date, censored, language, translations,
    length, vndb, genre, voice, os, other_games, last_update):
    con = sl.connect(remotedbName)
    cursor = con.cursor()
    sql = """INSERT OR REPLACE INTO f95zone_data
        (f95_id, short_name, other, engine, banner_url, title, status, version, developer,
        site_url, overview, thread_update, release_date, censored, language, translations,
        length, vndb, genre, voice, os, other_games,last_update)
        VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"""
    parameters = (f95_id, short_name, other, engine, banner_url, title, status, version, developer,
    site_url, overview, thread_update, release_date, censored, language, translations,
    length, vndb, genre, voice, os, other_games, last_update)
    cursor.execute(sql,parameters)
    con.commit()
    cursor.close()

def updateF95zoneTableBasic(f95_id, title, version, developer,
    site_url, views, likes, tags, rating, screens):
    con = sl.connect(remotedbName)
    cursor = con.cursor()
    sql = """INSERT OR REPLACE INTO f95zone_data
        (f95_id, title, version, developer,
    site_url, views, likes, tags, rating, screens)
        VALUES (?,?,?,?,?,?,?,?,?,?);"""
    parameters = (f95_id, title, version, developer,
    site_url, views, likes, tags, rating, screens)
    cursor.execute(sql,parameters)
    con.commit()
    cursor.close()


def updateBanners():
    con = sl.connect(localdbName)
    cursor = con.cursor()
    sql = """SELECT F95DB.id, Games.shortname, F95DB.bannerlink FROM F95DB INNER JOIN Games on Games.shortname = F95DB.id"""
    result = cursor.execute(sql)

    for row in result:
        print("getting image ", row[2])
        img_data = requests.get(row[2]).content
        con = sl.connect('gmData.db')
        cursor = con.cursor()
        sql = """UPDATE F95DB SET banner =? WHERE id='""" + row[0] +"""'"""
        #print(sql)
        cursor.execute(sql,(img_data,))

def FindF95zoneRecord(short_name):
    con = sl.connect(remotedbName)
    cursor = con.cursor()
    sql = """SELECT f95_id, title, developer, engine, banner_url FROM f95zone_data WHERE short_name like '""" + short_name +"""'"""
    return cursor.execute(sql)
def getRecordTitles():
    con = sl.connect(localdbName)
    cursor = con.cursor()
    sql = "SELECT title, record_id FROM records ORDER BY title ASC"
    return cursor.execute(sql)

def getRecordTitle(title):
    con = sl.connect(localdbName)
    cursor = con.cursor()
    title = title.replace("'","''")
    sql = """SELECT record_id FROM records WHERE title like '""" + title + """'"""
    cursor.execute(sql)
    return cursor.fetchone()

def checkIfgameinstalled(game_path):
    con = sl.connect(localdbName)
    cursor = con.cursor()
    cursor.execute("""SELECT game_path FROM game_metadata WHERE game_path like '"""+game_path +"""'""")
    return cursor.fetchone()

#INSERT FUNCTIONS
def addf95mapping(f95_id, record_id):
    con = sl.connect(localdbName)
    cursor = con.cursor()
    sql = """INSERT OR REPLACE INTO f95zone_mapping
    (f95_id, record_id)
    VALUES (?,?);"""
    parameters = (f95_id, record_id,)
    cursor.execute(sql,parameters)
    con.commit()

def addGameRecord(record_id, title, developer, engine, last_played_record, total_playtime):
    con = sl.connect(localdbName)
    cursor = con.cursor()
    sql = """INSERT OR REPLACE INTO records
    (record_id, title, developer, engine, last_played_record, total_playtime)
    VALUES (?,?,?,?,?,?);"""
    parameters = (record_id, title, developer, engine, last_played_record, total_playtime,)
    cursor.execute(sql,parameters)
    con.commit()


