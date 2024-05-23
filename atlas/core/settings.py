from configparser import ConfigParser
from os import path
from enum import Enum

#This will be treated like a static class
class settings(object):

    config = ConfigParser()
    #Iinit all vars here
    #PATHS
    root_path = 'data'
    database_path = 'data'
    database_abs_path = ''
    images_path = 'data/images'
    games_path = 'data/games'
    themes_path = 'data/themes'

    #IMPORTER
    pathparse = '{creator}/{title}/{version}'
    skip_filesize = False
    search_gameinfo =  True
    download_banner = False
    download_vndb = False
    move_imported = False 

    #ATLAS
    use_system_theme = False
    current_theme = 'dark.qss'

    #THESE VARS NOT IN CONFIG
    os = ""


    def create_config():
        settings.update_config()
        with open('config.ini', 'w') as configfile:
            settings.config.write(configfile)
    
    def update_config():        
        settings.config['PATHS'] = {
            'root':settings.root_path,
            'database':settings.database_path, 
            'images':settings.images_path,
            'games':settings.games_path,
            'themes':settings.themes_path
            }

        settings.config['IMPORTER'] = {
            'pathparse' : settings.pathparse,
            'skip_filesize': settings.skip_filesize,
            'search_gameinfo': settings.search_gameinfo,
            'download_banner': settings.download_banner,
            'download_vndb': settings.download_vndb,
            'move_imported': settings.move_imported 
            }

        settings.config['ATLAS'] = {
            'use_system_theme' : settings.use_system_theme,
            'current_theme' : settings.current_theme
        }

    def load():
        file_path = 'config.ini'
        if(path.exists(file_path)):            
            settings.config.sections()   
            settings.config.read(file_path)
        else:
            settings.create_config()

    def save():
        settings.update_config()
        file_path = 'config.ini'
        if(path.exists(file_path)):            
            settings.config.sections()   
            settings.config.write(file_path,)


class banner_type(Enum):
	error = -1,
	normal = 0,
	wide = 1,
	cover = 2,
	logo = 3,
	#SENTINEL // Used to determine the number of banner types
