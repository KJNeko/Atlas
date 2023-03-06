//
// Created by kj16609 on 1/12/23.
//

#include <filesystem>

#include <tracy/Tracy.hpp>

#include "Database.hpp"

#include <h95/logging.hpp>

namespace internal
{
	static sqlite::database* db { nullptr };
	static std::mutex db_mtx {};
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

void Database::initalize( const std::filesystem::path init_path )
{
	spdlog::debug( "Initalizing database with path {}", init_path.string() );
	std::filesystem::create_directories( init_path.parent_path() );

	internal::db = new sqlite::database( init_path );

	Transaction transaction;

	const std::vector< std::string > table_queries {
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT)",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, UNIQUE(record_id, version, game_path, exec_path))",
		"CREATE TABLE IF NOT EXISTS images (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
		"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, installed INTEGER, played INTEGER, wanted INTEGER)" };

	for ( const auto& query_str : table_queries )
	{
		transaction << query_str;
		spdlog::debug( "Executing {}" );
	}

	transaction.commit();
}

void Database::deinit()
{
	std::lock_guard guard { internal::db_mtx };
	delete internal::db;
	internal::db = nullptr;
}

Transaction::Transaction() : guard( new std::lock_guard( Database::lock() ) )
{
	if ( internal::db == nullptr )
	{
		delete guard;
		throw TransactionInvalid();
	}

	*this << "BEGIN TRANSACTION";
}

Transaction::~Transaction()
{
	if ( !finished ) abort();
}

void Transaction::commit()
{
	if ( finished ) throw TransactionInvalid();
	*this << "COMMIT TRANSACTION";

	finished = true;
	delete guard;
}

void Transaction::abort()
{
	if ( finished ) throw TransactionInvalid();
	*this << "ROLLBACK TRANSACTION";

	finished = true;
	delete guard;
}

sqlite::database_binder Transaction::operator<<( const std::string& sql )
{
	if ( finished ) throw TransactionInvalid();
	return Database::ref() << sql;
}
