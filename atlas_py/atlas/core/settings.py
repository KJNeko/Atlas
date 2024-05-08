import configparser
import os

#This will be treated like a static class
class settings(object):

    config = configparser.ConfigParser()

    def create_config():
        #define all settings here that will be writte to config file
        cfg = settings.config
        cfg['PATHS'] = {
            'database':'./data', 
            'images':'./data/images',
            'games':'./data/games',
            'themes':'./data/themes'
            }

        cfg['IMPORTER'] = {
            'pathparse' : '{creator}/{title}/{version}',
            'skip_filesize': False,
            'search_gameinfo': True,
            'download_banner': False,
            'download_vndb': False,
            'move_imported': False 
            }

        cfg['ATLAS'] = {
            'default_theme' : 'dark.qss'
        }


        with open('config.ini', 'w') as configfile:
            cfg.write(configfile)


    def load():
        working_dir = os.getcwd()
        file_path = 'config.ini'
        if(os.path.exists(file_path)):            
            settings.config.sections()   
            settings.config.read(file_path)
        else:
            settings.create_config()
