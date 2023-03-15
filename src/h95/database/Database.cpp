//
// Created by kj16609 on 1/12/23.
//

#include <filesystem>


#include "Database.hpp"
#include "h95/config.hpp"

#include <h95/logging.hpp>

namespace internal
{
	static sqlite::database* db { nullptr };
	static std::mutex db_mtx;

#ifndef NDEBUG
	static std::atomic<std::thread::id> last_locked;
#endif

	//static std::mutex db_mtx {};
}  // namespace internal

sqlite::database& Database::ref()
{
	if ( internal::db != nullptr )
		return *internal::db;
	else
		throw std::runtime_error( "Database was not initalized!" );
}

std::mutex& Database::lock()
{
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path ) try
{
	spdlog::info( "Initalizing database with path {}", init_path );
	std::filesystem::create_directories( init_path.parent_path() );
	spdlog::info( "Initalizing database with path {}", init_path );

	internal::db = new sqlite::database( init_path );

	if ( getSettings< bool >( "db/first_start", true ) )
	{
		NonTransaction transaction;

		const std::vector< std::string > table_queries {
			"CREATE TABLE records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, last_played_r DATE, total_playtime INTEGER, UNIQUE(title, creator, engine));",
			"CREATE TABLE game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT UNIQUE, game_path TEXT, exec_path TEXT, in_place, last_played DATE, version_playtime INTEGER);",
			"CREATE TABLE images (record_id INTEGER REFERENCES records(record_id), type INTEGER, path TEXT, UNIQUE(record_id, type, path));",
			"CREATE TABLE tags (tag_id INTEGER PRIMARY KEY, tag TEXT UNIQUE)",
			"CREATE TABLE tag_mappings (record_id INTEGER REFERENCES records(record_id), tag_id REFERENCES tags(tag_id), UNIQUE(record_id, tag_id))",

			//Dummy tables. Should be filled out later (Exists to allow X_mapping to use `REFERENCE`
			"CREATE TABLE f95zone_data (f95_id INTEGER PRIMARY KEY);",
			"CREATE TABLE dlsite_data (dlsite_id TEXT PRIMARY KEY);",

			"CREATE TABLE f95zone_mapping (record_id REFERENCES records(record_id), f95_id REFERENCES f95zone_data(f95_id), UNIQUE(record_id, f95_id));",
			"CREATE TABLE dlsite_mapping (record_id REFERENCES records(record_id), dlsite_id REFERENCES dlsite_data(dlsite_id), UNIQUE(record_id, dlsite_id));"

			//Old tables
			/*
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT)",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, UNIQUE(record_id, version, game_path, exec_path))",
		"CREATE TABLE IF NOT EXISTS images (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
		"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, installed INTEGER, played INTEGER, wanted INTEGER)"*/
		};

		for ( const auto& query_str : table_queries )
		{
			transaction << query_str;
			spdlog::debug( "Executing {}", query_str );
		}

		transaction.commit();

		setSettings( "db/first_start", false );
	}
	else
		spdlog::info( "Database has been initalized before! Skipping table creation" );
}
catch(sqlite::sqlite_exception& e)
{
	spdlog::error("{}", e.get_sql());
	std::rethrow_exception(std::current_exception());
}

void Database::deinit()
{
	std::lock_guard guard { internal::db_mtx };
	delete internal::db;
	internal::db = nullptr;
}


std::lock_guard<std::mutex> TransactionData::getLock()
{
	//Check if we are already locked
	if(internal::last_locked == std::this_thread::get_id())
		throw std::runtime_error("Deadlock");
	else
		return std::lock_guard<std::mutex>(Database::lock());
}

TransactionData::TransactionData() :  guard(getLock())
{
	internal::last_locked = std::this_thread::get_id();
}
TransactionData::~TransactionData()
{
	internal::last_locked = std::thread::id(-1);
}


Transaction::Transaction(const bool autocommit) : m_autocommit(autocommit), data(new TransactionData())
{
	if ( internal::db == nullptr )
	{
		data.reset();
		throw TransactionInvalid();
	}

	*this << "BEGIN TRANSACTION";
}

Transaction::~Transaction()
{
	if (data != nullptr)
	{
		if ( m_autocommit )
			commit();
		else
			abort();
	}
}

void Transaction::commit()
{
	if ( data->finished ) throw TransactionInvalid();
	*this << "COMMIT TRANSACTION";

	data->finished = true;
	data.reset();
}

void Transaction::abort()
{
	if ( data->finished ) throw TransactionInvalid();
	*this << "ROLLBACK TRANSACTION";

	data->finished = true;
	data.reset();
}

sqlite::database_binder Transaction::operator<<( const std::string& sql )
{
	spdlog::info( "Executing {}", sql );
	if ( data->finished ) throw TransactionInvalid();
	return Database::ref() << sql;
}


NonTransaction::NonTransaction() : guard( new std::lock_guard( Database::lock() ) )
{
	if ( internal::db == nullptr )
	{
		delete guard;
		throw TransactionInvalid();
	}
}

NonTransaction::~NonTransaction()
{
	if ( !finished ) abort();
}

void NonTransaction::commit()
{
	if ( finished ) throw TransactionInvalid();
	finished = true;
	delete guard;
}

void NonTransaction::abort()
{
	if ( finished ) throw TransactionInvalid();
	finished = true;
	delete guard;
}

sqlite::database_binder NonTransaction::operator<<( const std::string& sql )
{
	spdlog::debug( "Executing {} without transaction", sql );
	if ( finished ) throw TransactionInvalid();
	return Database::ref() << sql;
}
