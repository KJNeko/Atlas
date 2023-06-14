//
// Created by kj16609 on 1/12/23.
//

#include "Database.hpp"

#include "core/config.hpp"
#include "core/database/record/Record.hpp"

namespace internal
{
	static sqlite::database* db { nullptr };
#ifdef TRACY_ENABLE
	static TracyLockableN( std::mutex, db_mtx, "Database lock" );
#else
	static std::mutex db_mtx;
#endif

	static std::atomic< std::thread::id > last_locked {};

	//static std::mutex db_mtx {};
} // namespace internal

sqlite::database& Database::ref()
{
	ZoneScoped;
	if ( internal::db != nullptr )
		return *internal::db;
	else
		throw std::runtime_error( "ref: Database was not initalized!" );
}

internal::MtxType& Database::lock()
{
	ZoneScoped;
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path )
try
{
	ZoneScoped;
	initLogging();

	if ( init_path == ":memory:" )
	{
		internal::db = new sqlite::database( ":memory:" );
	}
	else
	{
		if ( init_path.parent_path() != "" && !std::filesystem::exists( init_path.parent_path() ) )
			std::filesystem::create_directories( init_path.parent_path() );

		internal::db = new sqlite::database( init_path.string() );
	}
	NonTransaction transaction;

	const std::vector< std::string > table_queries {
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, last_played_r DATE, total_playtime INTEGER, UNIQUE(title, creator, engine));",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, in_place, last_played DATE, version_playtime INTEGER, folder_size INTEGER, date_added INTEGER, UNIQUE(record_id, version));",
		"CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS SELECT DISTINCT record_id, game_metadata.date_added FROM records NATURAL JOIN game_metadata ORDER BY game_metadata.date_added DESC;",

		//Extra data for records
		"CREATE TABLE IF NOT EXISTS game_notes (record_id INTEGER REFERENCES records(record_id), notes TEXT, UNIQUE(record_id))",

		//Atlas data tables
		"CREATE TABLE IF NOT EXISTS atlas_data (id INTEGER PRIMARY KEY, id_name STRING UNIQUE, short_name STRING,"
		"title STRING, original_name STRING, category STRING, engine STRING, status STRING, version STRING,"
		"developer STRING, creator STRING, overview STRING, censored STRING, language STRING, translations STRING,"
		"genre STRING, tags STRING, voice STRING, os STRING, release_date DATE, length STRING, banner STRING, banner_wide STRING,"
		"cover STRING, logo STRING, wallpaper STRING, previews STRING, last_db_update STRING);",

		"CREATE TABLE IF NOT EXISTS atlas_mapping (record_id INTEGER REFERENCES records(record_id), atlas_id INTEGER REFERENCES atlas_data(id), UNIQUE(record_id, atlas_id));",

		//F95 data tables
		"CREATE TABLE IF NOT EXISTS f95_zone_data (f95_id INTEGER UNIQUE, id INTEGER PRIMARY KEY, banner_url STRING, site_url STRING,"
		"last_thread_comment STRING, thread_publish_date STRING, last_record_update STRING, views STRING, likes STRING, tags STRING, rating STRING,"
		"screens STRING, replies STRING);",

		"CREATE TABLE IF NOT EXISTS f95_zone_mapping (record_id INTEGER REFERENCES records(record_id), f95_id INTEGER REFERENCES f95_zone_data(f95_id), UNIQUE(record_id, f95_id));",

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

	transaction.commit();

	config::db::first_start::set( false );

	try
	{
		if ( !recordExists( "Galaxy Crossing: First Conquest", "Atlas Games", "Unity" ) )
		{
			const Record record { importRecord( "Galaxy Crossing: First Conquest", "Atlas Games", "Unity" ) };

			record->addVersion(
				"Chapter: 1",
				"C:/Atlas Games/Galaxy Crossing First Conquest",
				"C:/Atlas Games/Galaxy Crossing First Conquest/Galaxy Crossing First Conquest.exe",
				0,
				true );
		}
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::info( "Failed to insert dummy record: {}", e.errstr() );
	}
	catch ( const RecordAlreadyExists& e )
	{
		//do nothing
	}
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "initalize: Database has failed to initalize: {}", e.get_sql() );
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
	ZoneScoped;
	internal::last_locked = std::this_thread::get_id();
}

TransactionData::~TransactionData()
{
	ZoneScoped;
	internal::last_locked = std::thread::id();
}

Transaction::Transaction( const bool autocommit ) : data( new TransactionData() ), m_autocommit( autocommit )
{
	ZoneScoped;
	if ( internal::db == nullptr )
	{
		spdlog::error( "Transaction: Database was not ready!" );
		data.reset();
		throw TransactionInvalid( m_previous_statement );
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
	if ( !data->ran_once )
	{
		spdlog::warn( "commit(): Nothing was done in this Transaction? Check if this is intended" );
	}
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid( m_previous_statement );
	*this << "COMMIT TRANSACTION";

	data->invalid = true;
	releaseData();
}

void Transaction::abort()
{
	ZoneScoped;
	spdlog::warn( "A transaction was aborted! Last executed:\"{}\"", m_previous_statement );
	if ( !data->ran_once )
	{
		spdlog::warn( "abort(): Nothing was done in this Transaction? Check if this is intended" );
	}
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid( m_previous_statement );
	*this << "ROLLBACK TRANSACTION";

	data->invalid = true;
	releaseData();
}

sqlite::database_binder Transaction::operator<<( const std::string& sql )
{
	ZoneScoped;
	TracyMessage( sql.c_str(), sql.size() );
	if ( data == nullptr ) throw TransactionInvalid( sql );

	data->ran_once = true;
	if ( data.use_count() == 0 ) throw TransactionInvalid( sql );
	m_previous_statement = sql;
	return Database::ref() << sql;
}

Transaction::Transaction( Transaction& other ) :
  m_parent( &other ),
  data( other.data ),
  m_autocommit( other.m_autocommit )
{
	ZoneScoped;
	if ( data == nullptr ) throw TransactionInvalid( other.m_previous_statement );
}

NonTransaction::NonTransaction() : guard( new internal::LockGuardType( Database::lock() ) )
{
	ZoneScoped;
	if ( internal::db == nullptr )
	{
		guard.reset();
		throw TransactionInvalid( m_previous_statement );
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
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	finished = true;
	guard.reset();
}

void NonTransaction::abort()
{
	ZoneScoped;
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	finished = true;
	guard.reset();
	spdlog::error( "Transaction aborted!" );
}

sqlite::database_binder NonTransaction::operator<<( const std::string& sql )
{
	ZoneScoped;
	m_previous_statement = sql;
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	return Database::ref() << sql;
}