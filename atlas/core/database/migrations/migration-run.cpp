//
// Created by kj16609 on 9/4/23.
//

#include "core/database/RapidTransaction.hpp"
#include "core/logging/logging.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	/**
	 * @brief Runs the transaction and marks it as complete in the migrations table
	 * @tparam id ID of the migration to run.
	 * @param trans Transaction to perform the migration on
	 */
	template < std::uint64_t id >
	void runUp( Transaction& trans )
	{
		const auto sql { migration< id >() };
		trans << sql;
		trans << "INSERT INTO migrations (migration_id, sql) VALUES (?,?)" << id << sql;
	}

	//! Helper MACRO to easily add migrations to the switch inside runUp().
#define MIGRATE( idx )                                                                                                 \
	case idx:                                                                                                          \
		runUp< idx >( transaction );                                                                                   \
		break;

	//! Int to represent what the highest migration is at (Starting at migration 0 we run until we are N <= MIGRATIONS_VERSION)
	inline static constexpr int MIGRATIONS_VERSION { 18 };

	/**
	 * @brief Runs all transactions up until hitting MIGRATIONS_VERSION - 1.
	 * @throws DatabaseException
	 */
	void runUp()
	{
		atlas::logging::info( "Running database migrations - UP" );

		//Check to see if the migration table exists
		int count { 0 };
		RapidTransaction() << "SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name ='migrations'"
			>> count;

		if ( count == 0 )
		{
			//Table we are looking for doesn't exist. We should make it.
			Transaction trans;
			trans << "CREATE TABLE migrations (migration_id INTEGER PRIMARY KEY, sql TEXT)";
			trans.commit();
		}

		int current_migration { -1 };
		RapidTransaction() << "SELECT migration_id FROM migrations ORDER BY migration_id DESC limit 1"
			>> current_migration;

		try
		{
			while ( current_migration <= MIGRATIONS_VERSION )
			{
				//Increment the migration to see if the next migration can be done
				current_migration += 1;
				if ( MIGRATIONS_VERSION == current_migration )
				{
					logging::info( "All migrations processed." );
					return;
				}

				Transaction transaction;

				logging::info( "Running migration {}", current_migration );

				switch ( current_migration )
				{
					MIGRATE( 0 )
					MIGRATE( 1 )
					MIGRATE( 2 )
					MIGRATE( 3 )
					MIGRATE( 4 )
					MIGRATE( 5 )
					MIGRATE( 6 )
					MIGRATE( 7 )
					MIGRATE( 8 )
					MIGRATE( 9 )
					MIGRATE( 10 )
					MIGRATE( 11 )
					MIGRATE( 12 )
					MIGRATE( 13 )
					MIGRATE( 14 )
					MIGRATE( 15 )
					MIGRATE( 16 )
					MIGRATE( 17 )
					default:
						logging::critical(
							"MIGRATION VERSION HIGHER THEN EXPECTED! Migration was {}. Highest is {}",
							current_migration,
							MIGRATIONS_VERSION );
						return;
				}

				transaction.commit();
			}
		}
		catch ( DatabaseException& e )
		{
			logging::error( "Failed to apply migration. Currently at {}", current_migration );

			Database::deinit();

			std::abort();
		}
	}
} // namespace atlas::database::migrations
