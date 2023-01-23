//
// Created by kj16609 on 1/12/23.
//

#include "database.hpp"
#include <filesystem>
#include <sqlite_modern_cpp.h>
#include <tracy/Tracy.hpp>

namespace database
{
	namespace internal
	{
		sqlite::database* db { nullptr };
	}

	sqlite::database& db_ref()
	{
		return *internal::db;
	}

	void initalize()
	try
	{
		ZoneScoped;
		std::filesystem::create_directory( "./data" );

		internal::db = new sqlite::database( "./data/hydrus95.db" );

		const std::vector< std::string > table_strs {
			"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, version TEXT, engine TEXT, unique(title, creator, version, engine))",
			"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, game_path TEXT, exec_path TEXT)",
			"CREATE TABLE IF NOT EXISTS previews (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, type TEXT, path TEXT)",
			"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, installed INTEGER, played INTEGER, wanted INTEGER)" };

		for ( const auto& query_str : table_strs ) { db_ref() << query_str; }
	}
	catch ( sqlite::sqlite_exception& e )
	{
		std::cout << "Shit: " << e.get_code() << ": " << e.what() << " during " << e.get_sql() << std::endl;
	}

}  // namespace database