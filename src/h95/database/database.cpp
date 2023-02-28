//
// Created by kj16609 on 1/12/23.
//

#include <filesystem>

#include <tracy/Tracy.hpp>

#include "database.hpp"

#include <h95/logging.hpp>
#include <h95/config.hpp>

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
		spdlog::info( "Checking for database update" );
		const auto ver_number { getSettings< int >( "version_number", 100 ) };
		constexpr auto ver_latest { 100 };
		spdlog::info( "DB: Current: {} -> Supported: {}", ver_number, ver_latest );

		if ( ver_number == ver_latest )
		{
			spdlog::info( "No update require!" );
			return;
		}

		delete internal::db;

		spdlog::info( "Backing up database" );
		std::filesystem::copy( "./data/hydrus95.db", "./data/hydrus95.db.backup" );
		spdlog::info( "Backup complete" );

		internal::db = new sqlite::database( "./data/hydrus95.db" );

		//Abusing fallthrough.
		switch ( ver_number )
		{
			default:
				throw std::runtime_error(
					"I fucked up and forgot to add a case for this update. Report your version number to me in an issue" );
				break;
			case 100:
				spdlog::info( "Update chain reached end." );
				break;
		}

		setSettings( "version_number", ver_latest );

		spdlog::info( "Update finished. Erasing backup and restoring connection" );
		delete internal::db;
		std::filesystem::remove( "./data/hydrus95.db.backup" );
		internal::db = new sqlite::database( "./data/hydrus95.db" );

		return;
	}
	catch ( std::exception& e )
	{
		spdlog::error(
			"Something went wrong! Send the database to me along with this error if possible (Assuming your DB isn't stupidly large).\nUpdate failure message: {}",
			e.what() );
		std::abort();
	}

	void initalize()
	try
	{
		ZoneScoped;
		spdlog::debug( "Initalizing database" );
		std::filesystem::create_directory( "./data" );

		internal::db = new sqlite::database( "./data/hydrus95.db" );

		const std::vector< std::string > table_strs {
			"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, unique(title, creator, engine))",
			"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, UNIQUE(record_id, version, game_path, exec_path))",
			"CREATE TABLE IF NOT EXISTS previews (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
			"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, installed INTEGER, played INTEGER, wanted INTEGER)" };

		for ( const auto& query_str : table_strs )
		{
			db_ref() << query_str;
			spdlog::debug( "Executing {}", query_str );
		}

		update();
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::error(
			"Something went wrong while initalizing the database! {}: {} during {}",
			e.get_code(),
			e.what(),
			e.get_sql() );
		std::abort();
	}

}  // namespace database