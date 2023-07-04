//
// Created by kj16609 on 1/12/23.
//

#include "Database.hpp"

#include <sqlite3.h>

#include "Transaction.hpp"
#include "core/config.hpp"
#include "core/database/record/Record.hpp"

namespace internal
{
	static sqlite3* db_handle { nullptr };
#ifdef TRACY_ENABLE
	static TracyLockableN( std::mutex, db_mtx, "Database lock" );
#else
	static std::mutex db_mtx;
#endif

	//static std::mutex db_mtx {};
} // namespace internal

sqlite3& Database::ref()
{
	ZoneScoped;
	if ( internal::db_handle != nullptr )
		return *internal::db_handle;
	else
		throw std::runtime_error( "ref: Database was not initalized!" );
}

internal::MtxType& Database::lock()
{
	ZoneScoped;
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path )
{
	ZoneScoped;
	initLogging();

	if ( init_path.parent_path() != "" && !std::filesystem::exists( init_path.parent_path() ) )
		std::filesystem::create_directories( init_path.parent_path() );

	const int ret_code { sqlite3_open_v2(
		init_path.string().c_str(), &internal::db_handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr ) };

	if ( ret_code != SQLITE_OK )
	{
		spdlog::critical( "Failed to load sqlite!" );
		std::abort();
	}

	RapidTransaction transaction {};

	const std::vector< std::string > table_queries {
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, last_played_r DATE, total_playtime INTEGER, UNIQUE(title, creator, engine));",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, in_place, last_played DATE, version_playtime INTEGER, folder_size INTEGER, date_added INTEGER, UNIQUE(record_id, version));",
		"CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS SELECT DISTINCT record_id, game_metadata.date_added FROM records NATURAL JOIN game_metadata ORDER BY game_metadata.date_added DESC;",

		//Extra data for records
		"CREATE TABLE IF NOT EXISTS game_notes (record_id INTEGER REFERENCES records(record_id), notes TEXT, UNIQUE(record_id))",

		//Atlas data tables
		"CREATE TABLE IF NOT EXISTS atlas_data (atlas_id INTEGER PRIMARY KEY, id_name STRING UNIQUE, short_name STRING,"
		"title STRING, original_name STRING, category STRING, engine STRING, status STRING, version STRING,"
		"developer STRING, creator STRING, overview STRING, censored STRING, language STRING, translations STRING,"
		"genre STRING, tags STRING, voice STRING, os STRING, release_date DATE, length STRING, banner STRING, banner_wide STRING,"
		"cover STRING, logo STRING, wallpaper STRING, previews STRING, last_db_update STRING);",

		"CREATE TABLE IF NOT EXISTS atlas_mapping (record_id INTEGER REFERENCES records(record_id), atlas_id INTEGER REFERENCES atlas_data(id), UNIQUE(record_id, atlas_id));",

		//F95 data tables
		"CREATE TABLE IF NOT EXISTS f95_zone_data (f95_id INTEGER UNIQUE PRIMARY KEY, atlas_id INTEGER REFERENCES atlas_data(atlas_id) UNIQUE , banner_url STRING, site_url STRING,"
		"last_thread_comment STRING, thread_publish_date STRING, last_record_update STRING, views STRING, likes STRING, tags STRING, rating STRING,"
		"screens STRING, replies STRING);",

		//Update handling
		"CREATE TABLE IF NOT EXISTS updates (update_time INTEGER PRIMARY KEY, processed_time INTEGER, md5 BLOB);",

		//Tags
		"CREATE TABLE IF NOT EXISTS tags (tag_id INTEGER PRIMARY KEY, tag TEXT UNIQUE)",
		"CREATE TABLE IF NOT EXISTS tag_mappings (record_id INTEGER REFERENCES records(record_id), tag_id REFERENCES tags(tag_id), UNIQUE(record_id, tag_id))",

		//Tag views
		"CREATE VIEW IF NOT EXISTS title_tags (tag, record_id) AS SELECT 'title:' || title, record_id FROM records;",
		"CREATE VIEW IF NOT EXISTS creator_tags (tag, record_id) AS SELECT 'creator:' || creator, record_id FROM records;",
		"CREATE VIEW IF NOT EXISTS engine_tags (tag, record_id) AS SELECT 'engine:' || engine, record_id FROM records;",
		"CREATE VIEW IF NOT EXISTS full_tags (tag, record_id) AS SELECT tag, record_id FROM tags NATURAL JOIN tag_mappings NATURAL JOIN records UNION SELECT tag, record_id FROM title_tags UNION SELECT tag, record_id FROM creator_tags UNION SELECT tag, record_id FROM engine_tags;",

		//Image tables
		"CREATE TABLE IF NOT EXISTS previews (record_id REFERENCES records(record_id), path TEXT UNIQUE, position INTEGER DEFAULT 256, UNIQUE(record_id, path))",
		"CREATE TABLE IF NOT EXISTS banners (record_id REFERENCES records(record_id), path TEXT UNIQUE, type INTEGER, UNIQUE(record_id, path, type))",

		//Stats tables
		"CREATE TABLE IF NOT EXISTS data_change (timestamp INTEGER, delta INTEGER)",
	};

	for ( const auto& query_str : table_queries ) transaction << query_str;

	config::db::first_start::set( false );

	//Prepare our example record for the config
	if ( !recordExists( "Galaxy Crossing: First Conquest", "Atlas Games", "Unity" ) )
	{
		const Record record { importRecord( "Galaxy Crossing: First Conquest", "Atlas Games", "Unity" ) };

		record->addVersion(
			"Chapter: 1",
			"C:/Atlas Games/Galaxy Crossing First Conquest",
			"Galaxy Crossing First Conquest.exe",
			0,
			true );
	}
}

void Database::deinit()
{
	ZoneScoped;
	std::lock_guard guard { internal::db_mtx };
	sqlite3_close_v2( internal::db_handle );
}
