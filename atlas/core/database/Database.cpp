//
// Created by kj16609 on 1/12/23.
//

#include "Database.hpp"

#include "core/config.hpp"

namespace internal
{
	static sqlite::database* db { nullptr };
#ifdef TRACY_ENABLE
	static TracyLockable( std::mutex, db_mtx );
#else
	static std::mutex db_mtx;
#endif

	static std::atomic< std::thread::id > last_locked {};

	//static std::mutex db_mtx {};
} // namespace internal

sqlite::database& Database::ref()
{
	if ( internal::db != nullptr )
		return *internal::db;
	else
		throw std::runtime_error( "ref: Database was not initalized!" );
}

internal::MtxType& Database::lock()
{
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path )
try
{
	initLogging();

	spdlog::debug( "Initalizing database with path {}", init_path );
	if ( init_path != ":memory:" && !std::filesystem::exists( init_path ) )
		std::filesystem::create_directories( init_path.parent_path() );

	internal::db = new sqlite::database( init_path.string() );

	NonTransaction transaction;

	const std::vector< std::string > table_queries {
		"CREATE TABLE IF NOT EXISTS records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, engine TEXT, last_played_r DATE, total_playtime INTEGER, UNIQUE(title, creator, engine));",
		"CREATE TABLE IF NOT EXISTS game_metadata (record_id INTEGER REFERENCES records(record_id), version TEXT, game_path TEXT, exec_path TEXT, in_place, last_played DATE, version_playtime INTEGER, folder_size INTEGER, date_added INTEGER, UNIQUE(record_id, version));",

		//Extra data for records
		"CREATE TABLE IF NOT EXISTS game_notes (record_id INTEGER REFERENCES records(record_id), notes TEXT, UNIQUE(record_id))",

		//Dummy tables. Should be filled out later (Exists to allow X_mapping to use `REFERENCE`
		"CREATE TABLE IF NOT EXISTS f95zone_data (f95_id INTEGER PRIMARY KEY);",
		"CREATE TABLE IF NOT EXISTS dlsite_data (dlsite_id TEXT PRIMARY KEY);",
		"CREATE TABLE IF NOT EXISTS gl_data (glid INTGEGER PRIMARY KEY);",
		"CREATE TABLE IF NOT EXISTS atlas_data (atlas_id INTEGER PRIMARY KEY, id_name TEXT, short_name TEXT, title TEXT, original_name TEXT, category TEXT, engine TEXT, status TEXT, version TEXT, developer TEXT, creator TEXT, overview TEXT, censored TEXT, language TEXT, translations TEXT, genre TEXT, voice TEXT, os TEXT, release_date DATE, length TEXT, banner TEXT, banner_wide TEXT, cover TEXT, logo TEXT, last_update DATE)",

		"CREATE TABLE IF NOT EXISTS f95zone_mapping (record_id REFERENCES records(record_id), f95_id REFERENCES f95zone_data(f95_id), UNIQUE(record_id, f95_id));",
		"CREATE TABLE IF NOT EXISTS dlsite_mapping (record_id REFERENCES records(record_id), dlsite_id REFERENCES dlsite_data(dlsite_id), UNIQUE(record_id, dlsite_id));",
		"CREATE TABLE IF NOT EXISTS gl_mapping (record_id REFERENCES records(record_id), glid REFERENCES gl_data(glid), UNIQUE(record_id, glid))",
		"CREATE TABLE IF NOT EXISTS atlas_mapping (record_id REFERENCES records(record_id), atlas_id INTEGER REFERENCES atlas_data(atlas_id), UNIQUE(record_id, atlas_id))",

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
	};

	for ( const auto& query_str : table_queries ) transaction << query_str;

	transaction.commit();

	config::db::first_start::set( false );

	try
	{
		Transaction transaction_record { Transaction::Autocommit };

		transaction_record
			<< "INSERT INTO records (record_id, title, creator, engine, last_played_r, total_playtime) VALUES ($1, $2, $3, $4, 0, 0)"
			<< static_cast< std::uint64_t >( 1 ) << "Galaxy Crossing: First Conquest"
			<< "Atlas Games"
			<< "Unity";
	}
	catch ( sqlite::sqlite_exception& e )
	{
		spdlog::info( "Failed to insert dummy record: {}", e.errstr() );
	}

	try
	{
		Transaction transaction_record { Transaction::Autocommit };
		transaction_record
			<< "INSERT INTO game_metadata (record_id, version, game_path, exec_path, in_place, last_played, version_playtime, folder_size, date_added) VALUES ($1, $2, $3, $4, $5, 0, 0, 0, 0)"
			<< static_cast< std::uint64_t >( 1 ) << "Chapter: 1"
			<< "C:/Users/kj16609/Documents/Atlas Games/Galaxy Crossing First Conquest"
			<< "C:/Users/kj16609/Documents/Atlas Games/Galaxy Crossing First Conquest/Galaxy Crossing First Conquest.exe"
			<< true;

		transaction_record << "INSERT INTO banners (record_id, path, type) VALUES (?, ?, ?)"
						   << static_cast< std::uint64_t >( 1 ) << ":/images/assets/Grid_Capsule_Default.webp" << 1;
	}
	catch ( ... )
	{
		spdlog::info( "Failed to insert dummy record metadata" );
	}
}
catch ( sqlite::sqlite_exception& e )
{
	spdlog::error( "initalize: Database has failed to initalize: {}", e.get_sql() );
	std::rethrow_exception( std::current_exception() );
}

void Database::deinit()
{
	std::lock_guard guard { internal::db_mtx };
	delete internal::db;
	internal::db = nullptr;
}

internal::LockGuardType TransactionData::getLock()
{
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
	internal::last_locked = std::thread::id();
}

Transaction::Transaction( const bool autocommit ) : data( new TransactionData() ), m_autocommit( autocommit )
{
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
	if ( data == nullptr ) throw TransactionInvalid( other.m_previous_statement );
}

NonTransaction::NonTransaction() : guard( new internal::LockGuardType( Database::lock() ) )
{
	if ( internal::db == nullptr )
	{
		guard.reset();
		throw TransactionInvalid( m_previous_statement );
	}
}

NonTransaction::~NonTransaction()
{
	if ( !finished ) abort();
}

void NonTransaction::commit()
{
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	finished = true;
	guard.reset();
}

void NonTransaction::abort()
{
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	finished = true;
	guard.reset();
	spdlog::error( "Transaction aborted!" );
}

sqlite::database_binder NonTransaction::operator<<( const std::string& sql )
{
	m_previous_statement = sql;
	spdlog::debug( "Executing {} without transaction", sql );
	if ( finished ) throw TransactionInvalid( m_previous_statement );
	return Database::ref() << sql;
}