import sqlite3
from contextlib import closing

# Constants for invalid IDs
INVALID_RECORD_ID = 0
INVALID_ATLAS_ID = 0
INVALID_F95_ID = 0

# Define the GameData class
class GameData:
    def __init__(self, id=None, title=None, creator=None, engine=None):
        self.m_game_id = id
        self.m_title = title
        self.m_creator = creator
        self.m_engine = engine
        self.m_last_played = None
        self.m_total_playtime = None
        self.m_description = None
        self.m_preview_count = None
        self.m_versions = []
        self.m_banner_paths = {}
        self.m_preview_paths = []
        self.m_favorite = None

        if id is not None:
            self.load_from_db(id)
        elif title is not None and creator is not None and engine is not None:
            self.create_new_record(title, creator, engine)

    def load_from_db(self, id):
        with closing(sqlite3.connect('game_database.db')) as conn:
            cursor = conn.cursor()
            query = f'SELECT record_id FROM games WHERE title = {self.m_title} AND creator = {self.m_creator} AND engine = {self.m_engine}'
        
    def create_new_record(title, creator, engine):
        query = "INSERT INTO games (title, creator, engine, last_played_r, total_playtime) VALUES ({title}, {creator}, {engine}, 0, 0) RETURNING record_id"




    # Define the recordID function
    def recordID(title, creator, engine):
        record_id = INVALID_RECORD_ID
        query = f'SELECT record_id FROM games WHERE title = {title} AND creator = {creator} AND engine = {engine}'
        return record_id
        # ... (function implementation)

    # Define the recordExists function
    def record_exists(title, creator, engine):
        return GameData.recordID(title, creator, engine)
        # ... (function implementation)
