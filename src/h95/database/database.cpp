//
// Created by kj16609 on 1/12/23.
//

#include "database.hpp"
#include <filesystem>
#include <sqlite_modern_cpp.h>

namespace database
{
	namespace internal
	{
		sqlite::database* db {nullptr};
	}

	sqlite::database& db_ref() {return *internal::db;}

	void initalize()
	{
		std::filesystem::create_directory( "./data" );

		internal::db = new sqlite::database("./data/hydrus95.db");

		const std::vector< std::string > table_strs {
			"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, version TEXT, unique(title, creator, version))",
			"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), game_path TEXT, exec_path TEXT)",
			"CREATE TABLE IF NOT EXISTS previews (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
			"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id), installed INTEGER, played INTEGER, wanted INTEGER)" };

		for ( const auto& query_str : table_strs )
		{
			db_ref() << query_str;
		}
	}

}  // namespace database