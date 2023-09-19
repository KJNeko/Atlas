//
// Created by kj16609 on 9/4/23.
//

#include "core/config.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/logging/logging.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template < std::uint64_t id >
	void runUp( Transaction& trans )
	{
		const auto sql { migration< id >() };
		trans << sql;
		trans << "INSERT INTO migrations (migration_id, sql) VALUES (?,?)" << id << sql;
	}

#define MIGRATE( idx )                                                                                                 \
	case idx:                                                                                                          \
		runUp< idx >( trans );                                                                                         \
		break;

	//! Int to represent what the highest migration is at (Starting at migration 0 we run until we are N <= MIGRATIONS_VERSION)
	inline static constexpr int MIGRATIONS_VERSION { 17 };

	template < std::uint64_t idx >
	void runUpRecurse()
	{
		if constexpr ( idx >= MIGRATIONS_VERSION )
			return;
		else
			runUpRecurse< idx + 1 >;
	}

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

				Transaction trans;

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
					default:
						logging::critical(
							"MIGRATION VERSION HIGHER THEN EXPECTED! Migration was {}. Highest is {}",
							config::database::migration_version::get(),
							MIGRATIONS_VERSION );
						return;
				}

				trans.commit();
			}
		}
		catch ( std::exception& e )
		{
			logging::error(
				"Failed to apply migration. Currently at {}: {}",
				config::database::migration_version::get(),
				e.what() );
			std::abort();
		}
		catch ( ... )
		{
			logging::
				error( "Failed to apply migration. Currently at {}: ...", config::database::migration_version::get() );
			std::abort();
		}
	}
} // namespace atlas::database::migrations
