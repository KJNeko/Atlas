from pandas import DataFrame
class tables(object):
    
    class test():
        print("Test")

    def table_list() -> DataFrame:
        data ={"games":
			[str("CREATE TABLE IF NOT EXISTS games"
		   	"(record_id      INTEGER PRIMARY KEY,"
			"title          TEXT NOT NULL,"
			"creator        TEXT NOT NULL,"
			"engine         TEXT,"
			"last_played_r  DATE    DEFAULT 0,"
			"total_playtime INTEGER DEFAULT 0,"
			"description    TEXT,"
			"UNIQUE (title, creator, engine));")],
            "versions":
           [ str("CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS "
			"SELECT DISTINCT record_id, versions.date_added "
			"FROM games "
			"NATURAL JOIN versions "
			"ORDER BY versions.date_added DESC;")]         
        }
        df = DataFrame(data)
        return df