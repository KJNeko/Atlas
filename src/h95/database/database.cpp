//
// Created by kj16609 on 1/12/23.
//

#include "database.hpp"
#include <filesystem>
#include <sqlite_modern_cpp.h>
#include <tracy/Tracy.hpp>
#include "h95/config.hpp"

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

	void update()
	try
	{
		const auto ver_number { getSettings< int >( "version_number", 100 ) };
		constexpr auto ver_latest { 103 };

		if ( ver_number == ver_latest ) return;

		delete internal::db;

		std::cout << "Backing up database" << std::endl;
		std::filesystem::copy( "./data/hydrus95.db", "./data/hydrus95.db.backup" );
		std::cout << "Backup complete" << std::endl;

		internal::db = new sqlite::database("./data/hydrus95.db");

		//Abusing fallthrough.
		switch ( ver_number )
		{
			default:
				throw std::runtime_error(
					"I fucked up and forgot to add a case for this update. Report your version number to me in an issue" );

			case 100:
				std::cout << "Updating 100 -> 101" << std::endl;
				[[fallthrough]];
			case 101:
				std::cout << "Updating 101 -> 102" << std::endl;
				[[fallthrough]];
			case 102:
				std::cout << "Updating 102 -> 103" << std::endl;
				[[fallthrough]];
			case 103:
				std::cout << "Fully updated. No updates needed" << std::endl;
				break;
		}

		setSettings( "version_number", ver_latest );

		std::cout << "Finished updating. Deleting backup and restoring connection" << std::endl;
		delete internal::db;
		std::filesystem::remove("./data/hydrus95.db.backup");
		internal::db = new sqlite::database("./data/hydrus95.db");

		return;
	}
	catch ( std::exception& e )
	{
		std::cout
			<< "Update failed! Please ***MANUALLY*** restore your database by renaming ./data/hydrus95.db.backup to ./data/hydurs95.db. Send the DB to the dev."
			<< std::endl;
		std::cout << "Update failure message: " << e.what() << std::endl;
		std::abort();
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

		update();
	}
	catch ( sqlite::sqlite_exception& e )
	{
		std::cout << "Shit: " << e.get_code() << ": " << e.what() << " during " << e.get_sql() << std::endl;
	}

}  // namespace database