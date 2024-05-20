from pandas import DataFrame
class tables(object):

    def table_list() -> DataFrame:
        data ={"0-games":
			[str(
                "CREATE TABLE IF NOT EXISTS games"
				"(record_id      INTEGER PRIMARY KEY,"
				"title          TEXT NOT NULL,"
				"creator        TEXT NOT NULL,"
				"engine         TEXT,"
				"last_played_r  DATE    DEFAULT 0,"
				"total_playtime INTEGER DEFAULT 0,"
				"description    TEXT,"
				"UNIQUE (title, creator, engine));"
            )],
            
			"1-versions":
            [str(
                "CREATE TABLE IF NOT EXISTS versions("
				"record_id        INTEGER REFERENCES games (record_id),"
				"version          TEXT,"
				"game_path        TEXT,"
				"exec_path        TEXT,"
				"in_place		 BOOLEAN,"
				"last_played      DATE,"
				"version_playtime INTEGER,"
				"folder_size      INTEGER,"
				"date_added       INTEGER,"
				"UNIQUE (record_id, version));"
			)],
            
            "2-import_times":
            [str(
                "CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS "
				"SELECT DISTINCT record_id, versions.date_added "
				"FROM games "
				"NATURAL JOIN versions "
				"ORDER BY versions.date_added DESC;"
            )],

			"3-atlas_data":
            [str(
                "CREATE TABLE IF NOT EXISTS atlas_data("			
				"atlas_id       INTEGER PRIMARY KEY,"
				"id_name        STRING UNIQUE,"
				"short_name     STRING,"
				"title          STRING,"
				"original_name  STRING,"
				"category       STRING,"
				"engine         STRING,"
				"status         STRING,"
				"version        STRING,"
				"developer      STRING,"
				"creator        STRING,"
				"overview       STRING,"
				"censored       STRING,"
				"language       STRING,"
				"translations   STRING,"
				"genre          STRING,"
				"tags           STRING,"
				"voice          STRING,"
				"os             STRING,"
				"release_date   DATE,"
				"length         STRING,"
				"banner         STRING,"
				"banner_wide    STRING,"
				"cover          STRING,"
				"logo           STRING,"
				"wallpaper      STRING,"
				"previews       STRING,"
				"last_db_update STRING);"
			)]         
        }
        df = DataFrame(data)
        return df