//
// Created by kj16609 on 9/4/23.
//

#include "core/config.hpp"
#include "core/database/Transaction.hpp"
#include "core/logging.hpp"
#include "templates.hpp"

#define MIGRATE( idx )                                                                                                 \
	case idx:                                                                                                          \
		up< idx >( trans );                                                                                            \
		break;

namespace atlas::database::migrations
{
	//! Int to represent what the highest migration is at (Starting at migration 0 we run until we are N <= MIGRATIONS_VERSION)
	inline static constexpr int MIGRATIONS_VERSION { 16 };

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
		spdlog::info( "Running migrations - UP" );

		try
		{
			while ( config::database::migration_version::get() <= MIGRATIONS_VERSION )
			{
				int migration { config::database::migration_version::get() };

				if ( MIGRATIONS_VERSION == migration )
				{
					spdlog::info( "All migrations processed." );
					return;
				}

				Transaction trans;

				spdlog::info( "Running migration {}", migration );

				switch ( migration )
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
					default:
						spdlog::critical(
							"MIGRATION VERSION HIGHER THEN EXPECTED! Migration was {}. Highest is {}",
							config::database::migration_version::get(),
							MIGRATIONS_VERSION );
						return;
				}

				config::database::migration_version::set( migration + 1 );

				trans.commit();
			}
		}
		catch ( std::exception& e )
		{
			spdlog::error(
				"Failed to apply migration. Currently at {}: {}",
				config::database::migration_version::get(),
				e.what() );
			std::abort();
		}
		catch ( ... )
		{
			spdlog::
				error( "Failed to apply migration. Currently at {}: ...", config::database::migration_version::get() );
			std::abort();
		}
	}
} // namespace atlas::database::migrations
