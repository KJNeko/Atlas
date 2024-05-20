from pandas import DataFrame
class tables(object):

    def table_list() -> DataFrame:
        data ={
            "0-games":
			[str(
                """CREATE TABLE IF NOT EXISTS games
				(
					record_id      INTEGER PRIMARY KEY,
					title          TEXT NOT NULL,
					creator        TEXT NOT NULL,
					engine         TEXT,
					last_played_r  DATE    DEFAULT 0,
					total_playtime INTEGER DEFAULT 0,
					description    TEXT,
					UNIQUE (title, creator, engine)
				);"""
            )],
			"1-versions":
            [str(
                """CREATE TABLE IF NOT EXISTS versions
			   (
				record_id        INTEGER REFERENCES games (record_id),
				version          TEXT,
				game_path        TEXT,
				exec_path        TEXT,
				in_place		 BOOLEAN,
				last_played      DATE,
				version_playtime INTEGER,
				folder_size      INTEGER,
				date_added       INTEGER,
				UNIQUE (record_id, version)
			   );"""
			)],           
            "2-import_times":
            [str(
                """CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS
				SELECT DISTINCT record_id, versions.date_added
				FROM games
				NATURAL JOIN versions
				ORDER BY versions.date_added DESC;"""
            )],
			"3-atlas_data":
            [str(
                """CREATE TABLE IF NOT EXISTS atlas_data
				(
					atlas_id       INTEGER PRIMARY KEY,
					id_name        STRING UNIQUE,
					short_name     STRING,
					title          STRING,
					original_name  STRING,
					category       STRING,
					engine         STRING,
					status         STRING,
					version        STRING,
					developer      STRING,
					creator        STRING,
					overview       STRING,
					censored       STRING,
					language       STRING,
					translations   STRING,
					genre          STRING,
					tags           STRING,
					voice          STRING,
					os             STRING,
					release_date   DATE,
					length         STRING,
					banner         STRING,
					banner_wide    STRING,
					cover          STRING,
					logo           STRING,
					wallpaper      STRING,
					previews       STRING,
					last_db_update STRING
				);"""
			)] ,        
            "4-atlas_previews":
            [str(
                """CREATE TABLE IF NOT EXISTS atlas_previews
				(
					atlas_id    INTEGER REFERENCES atlas_data (atlas_id),
					preview_url STRING NOT NULL,
					UNIQUE (atlas_id, preview_url)
				);"""
			)],
            "5-atlas_mappings":
            [str(
                """CREATE TABLE IF NOT EXISTS atlas_mappings
				(
					record_id INTEGER REFERENCES games (record_id),
					atlas_id  INTEGER REFERENCES atlas_data (atlas_id),
					UNIQUE (record_id, atlas_id)
				);"""
			)],
            "6-f95_zone_data":
            [str(
                """CREATE TABLE IF NOT EXISTS f95_zone_data
				(
					f95_id              INTEGER UNIQUE PRIMARY KEY,
					atlas_id            INTEGER REFERENCES atlas_data (atlas_id) UNIQUE,
					banner_url          STRING,
					site_url            STRING,
					last_thread_comment STRING,
					thread_publish_date STRING,
					last_record_update  STRING,
					views               STRING,
					likes               STRING,
					tags                STRING,
					rating              STRING,
					screens             STRING,
					replies             STRING
				);"""
			)],
            "7-f95_zone_screens":
            [str(
                """CREATE TABLE IF NOT EXISTS f95_zone_screens
				(
					f95_id     INTEGER REFERENCES f95_zone_data (f95_id),
					screen_url TEXT NOT NULL
				);"""
			)],
            "8-updates":
            [str(
                """CREATE TABLE IF NOT EXISTS updates
				(
					update_time    INTEGER PRIMARY KEY,
					processed_time INTEGER,
					md5            BLOB
				);"""
			)],
            "9-tags":
            [str(
                """CREATE TABLE IF NOT EXISTS tags
				(
					tag_id INTEGER PRIMARY KEY,
					tag    TEXT UNIQUE
				);"""
			)],
            "10-tag_mappings":
            [str(
                """CREATE TABLE IF NOT EXISTS tag_mappings
				(
					record_id INTEGER REFERENCES games (record_id),
					tag_id REFERENCES tags (tag_id),
					UNIQUE (record_id, tag_id)
				);"""
			)],
            "11-atlas_tags":
            [str(
                """CREATE TABLE IF NOT EXISTS atlas_tags
				(
					tag_id   INTEGER REFERENCES tags (tag_id),
					atlas_id INTEGER REFERENCES atlas_data (atlas_id),
					UNIQUE (atlas_id, tag_id)
				);"""
			)],
            "12-f95_zone_tags":
            [str(
                """CREATE TABLE IF NOT EXISTS f95_zone_tags
				(
					f95_id INTEGER REFERENCES f95_zone_data (f95_id),
					tag_id INTEGER REFERENCES tags (tag_id)
				);"""
			)],
            "13-previews":
            [str(
                """CREATE TABLE IF NOT EXISTS previews
				(
					record_id REFERENCES games (record_id),
					path     TEXT UNIQUE,
					position INTEGER DEFAULT 256,
					UNIQUE (record_id, path)
				);"""
			)],
            "14-banners":
            [str(
                """CREATE TABLE IF NOT EXISTS banners
				(
					record_id REFERENCES games (record_id),
					path TEXT UNIQUE,
					type INTEGER,
					UNIQUE (record_id, path, type)
				);"""
			)],
            "15-data_change":
            [str(
                """CREATE TABLE IF NOT EXISTS data_change
				(
					timestamp INTEGER,
					delta     INTEGER
				)"""
			)],
            "16-f95_zone_mappings":
            [str(
                """CREATE TABLE IF NOT EXISTS f95_zone_mappings
				(
					record_id INTEGER REFERENCES games(record_id),
					f95_id INTEGER REFERENCES f95_zone_data(f95_id)
				);"""
			)],
            "17-blurhash":
            [str(
                """CREATE TABLE image_blurhash
				(
					image_sha256 BLOB UNIQUE PRIMARY KEY NOT NULL,
					blurhash TEXT NOT NULL,
					image_width INTEGER NOT NULL,
					image_height INTEGER NOT NULL
				);"""
			)],
            "18-favorites":
            [str(
                """CREATE TABLE favorites (record_id INTEGER REFERENCES games(record_id))"""
            )]
        }
        return DataFrame(data)