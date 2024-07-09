from os import path
from atlas.core.logger import *
from atlas.core.database.record.gamedata import*
from atlas.core.database.record.version import *
from atlas.core.database.migrations.migrations import *
from atlas.core.settings import *
from PySide6.QtGui import (QImage)

class database(object):

    def initialize(database_path: str) -> str:
        if not path.exists(database_path):
            logger.debug("Creating Database")
            #Create database file
            f = open(database_path, "a")
            f.close()
    
        #run all migrations
        migrations.run_up(database_path)

        #add dummy record
        test_name = "Galaxy Crossing: First Conquest"
        test_creator = "Atlas Games"
        test_engine = "Unity"

        if not GameData.record_exists(test_name, test_creator, test_engine):
            record_id = version.add_version("Chapter: 1","C:/Atlas Games/Galaxy Crossing First Conquest","Galaxy Crossing First Conquest.exe",0,True )
            banner_image = QImage(":/images/assets/Grid_Capsule_Default.webp")
            #if not banner_image.isNull():      
            #    game.set_banner(record_id,  "./data/images/config_image.webp", banner_type.Normal ) 