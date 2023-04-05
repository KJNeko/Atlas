//
// Created by kj16609 on 1/12/23.
//

#include "Database.hpp"

#include <tracy/Tracy.hpp>

#include "h95/config.hpp"

namespace internal
{
	static sqlite::database* db { nullptr };
#ifdef TRACY_ENABLE
	static TracyLockable( std::mutex, db_mtx );
#else
	static std::mutex db_mtx;
#endif

	static std::atomic< std::thread::id > last_locked { std::thread::id( 0 ) };

	//static std::mutex db_mtx {};
} // namespace internal

sqlite::database& Database::ref()
{
	if ( internal::db != nullptr )
		return *internal::db;
	else
		throw std::runtime_error( "Database was not initalized!" );
}

internal::MtxType& Database::lock()
{
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path )
try
{
	initLogging();

	ZoneScoped;
	spdlog::info( "Initalizing database with path {}", init_path );
	if ( init_path != ":memory:" && !std::filesystem::exists( init_path ) )
		std::filesystem::create_directories( init_path.parent_path() );

	internal::db = new sqlite::database( init_path.string() );

	NonTransaction transaction;

	const std::vector< std::string > table_queries {
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, last_played_r DATE, total_playtime INTEGER, UNIQUE(title, creator, engine));",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, in_place, last_played DATE, version_playtime INTEGER, UNIQUE(record_id, version));",
		"CREATE TABLE IF NOT EXISTS images (record_id INTEGER REFERENCES records(record_id), type INTEGER, path TEXT, UNIQUE(record_id, type, path));",
		"CREATE TABLE IF NOT EXISTS tags (tag_id INTEGER PRIMARY KEY, tag TEXT UNIQUE)",
		"CREATE TABLE IF NOT EXISTS tag_mappings (record_id INTEGER REFERENCES records(record_id), tag_id REFERENCES tags(tag_id), UNIQUE(record_id, tag_id))",

		//Dummy tables. Should be filled out later (Exists to allow X_mapping to use `REFERENCE`
		"CREATE TABLE IF NOT EXISTS f95zone_data (f95_id INTEGER PRIMARY KEY);",
		"CREATE TABLE IF NOT EXISTS dlsite_data (dlsite_id TEXT PRIMARY KEY);",

		"CREATE TABLE IF NOT EXISTS f95zone_mapping (record_id REFERENCES records(record_id), f95_id REFERENCES f95zone_data(f95_id), UNIQUE(record_id, f95_id));",
		"CREATE TABLE IF NOT EXISTS dlsite_mapping (record_id REFERENCES records(record_id), dlsite_id REFERENCES dlsite_data(dlsite_id), UNIQUE(record_id, dlsite_id));"

		//Old tables
		/*
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT)",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, UNIQUE(record_id, version, game_path, exec_path))",
		"CREATE TABLE IF NOT EXISTS images (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
		"CREATE TABLE IF NOT EXISTS flags (record_id INTEGER REFERENCES records(record_id) PRIMARY KEY, installed INTEGER, played INTEGER, wanted INTEGER)"*/
	};

	for ( const auto& query_str : table_queries ) transaction << query_str;

	transaction.commit();

	config::db::first_start::set( false );
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "{}", e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}

void Database::deinit()
{
	ZoneScoped;
	std::lock_guard guard { internal::db_mtx };
	delete internal::db;
	internal::db = nullptr;
}

internal::LockGuardType TransactionData::getLock()
{
	ZoneScoped;
	//Check if we are already locked
	if ( internal::last_locked == std::this_thread::get_id() )
	{
		spdlog::critical( "Deadlock detected! Ejecting!" );
		throw std::runtime_error( "Deadlock" );
	}
	else
		return internal::LockGuardType( Database::lock() );
}

TransactionData::TransactionData() : guard( getLock() )
{
	internal::last_locked = std::this_thread::get_id();
}

TransactionData::~TransactionData()
{
	internal::last_locked = std::thread::id( 0 );
}

Transaction::Transaction( const bool autocommit ) : data( new TransactionData() ), m_autocommit( autocommit )
{
	ZoneScoped;
	if ( internal::db == nullptr )
	{
		spdlog::error( "Database was not ready!" );
		data.reset();
		throw TransactionInvalid();
	}

	*this << "BEGIN TRANSACTION";
	data->ran_once = false;
}

Transaction::~Transaction()
{
	ZoneScoped;
	if ( data.use_count() == 1 && !data->invalid )
	{
		if ( m_autocommit )
			commit();
		else
		{
			spdlog::warn( "Transaction defaulted to abort on dtor! Check if this is intended!" );
			abort();
		}
	}
}

void Transaction::commit()
{
	ZoneScoped;
	if ( !data->ran_once ) spdlog::warn( "Nothing was done in this Transaction? Check if this is intended" );
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid();
	*this << "COMMIT TRANSACTION";

	data->invalid = true;
	releaseData();
}

void Transaction::abort()
{
	ZoneScoped;
	spdlog::error( "A transaction was aborted!" );
	if ( !data->ran_once ) spdlog::warn( "Nothing was done in this Transaction? Check if this is intended" );
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid();
	*this << "ROLLBACK TRANSACTION";

	data->invalid = true;
	releaseData();
}

sqlite::database_binder Transaction::operator<<( const std::string& sql )
{
	ZoneScoped;
	data->ran_once = true;
	spdlog::debug( "Executing {}", sql );
	if ( data.use_count() == 0 ) throw TransactionInvalid();
	return Database::ref() << sql;
}

Transaction::Transaction( Transaction& other ) :
  m_parent( &other ),
  data( other.data ),
  m_autocommit( other.m_autocommit )
{}

NonTransaction::NonTransaction() : guard( new internal::LockGuardType( Database::lock() ) )
{
	ZoneScoped;
	if ( internal::db == nullptr )
	{
		guard.reset();
		throw TransactionInvalid();
	}
}

NonTransaction::~NonTransaction()
{
	ZoneScoped;
	if ( !finished ) abort();
}

void NonTransaction::commit()
{
	ZoneScoped;
	if ( finished ) throw TransactionInvalid();
	finished = true;
	guard.reset();
}

void NonTransaction::abort()
{
	ZoneScoped;
	if ( finished ) throw TransactionInvalid();
	finished = true;
	guard.reset();
	spdlog::error( "Transaction aborted!" );
}

sqlite::database_binder NonTransaction::operator<<( const std::string& sql )
{
	ZoneScoped;
	spdlog::debug( "Executing {} without transaction", sql );
	if ( finished ) throw TransactionInvalid();
	return Database::ref() << sql;
}