//
// Created by kj16609 on 5/17/23.
//

#include <QApplication>
#include <QFile>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#pragma GCC diagnostic pop
#else
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#endif

#include "core/database/Database.hpp"
#include "core/database/Version.hpp"
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"

TEST_CASE( "Database Init Memory", "[database]" )
{
	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );
}

TEST_CASE( "Database Init", "[database]" )
{
	std::filesystem::remove( "test.db" );
	REQUIRE_NOTHROW( Database::initalize( "test.db" ) );
	REQUIRE( std::filesystem::exists( "test.db" ) );
	REQUIRE_NOTHROW( Database::deinit() );
	REQUIRE( std::filesystem::remove( "test.db" ) );
}

TEST_CASE( "Record", "[database][record][import]" )
{
	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );
	SECTION( "Create record" )
	{
		auto record { importRecord( "Test title", "Test creator", "Test engine" ) };

		SECTION( "Test getters" )
		{
			SECTION( "title" )
			{
				REQUIRE( record->m_title == "Test title" );
			}
			SECTION( "creator" )
			{
				REQUIRE( record->m_creator == "Test creator" );
			}
			SECTION( "engine" )
			{
				REQUIRE( record->m_engine == "Test engine" );
			}
			SECTION( "last played" )
			{
				REQUIRE( record->m_last_played == 0 );
			}
			SECTION( "total playtime" )
			{
				REQUIRE( record->m_total_playtime == 0 );
			}
		}

		SECTION( "Test setters" )
		{
			SECTION( "title" )
			{
				REQUIRE_NOTHROW( record.setTitle( "new Test Title" ) );
				REQUIRE( record->m_title == "new Test title" );
			}
			SECTION( "creator" )
			{
				REQUIRE_NOTHROW( record.setCreator( "new Test creator" ) );
				REQUIRE( record->m_creator == "new Test creator" );
			}
			SECTION( "engine" )
			{
				REQUIRE_NOTHROW( record.setEngine( "new Test engine" ) );
				REQUIRE( record->m_engine == "new Test engine" );
			}
			SECTION( "last played" )
			{
				REQUIRE_NOTHROW( record.addPlaytime( 5 ) );
				REQUIRE( record->m_total_playtime == 5 );
			}
			SECTION( "total playtime" )
			{
				REQUIRE_NOTHROW( record.setLastPlayed( 5 ) );
				REQUIRE( record->m_total_playtime == 5 );
			}
		}
	}
}

TEST_CASE( "Config record", "[database]" )
{
	//Ensure that the test record exists
	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );

	Game record { 1 };

	REQUIRE( record->m_title == "Galaxy Crossing: First Conquest" );
	REQUIRE( record->m_engine == "Unity" );
	REQUIRE( record->m_creator == "Atlas Games" );

	REQUIRE( record->m_versions.size() == 1 );
	const auto& version { record->m_versions.at( 0 ) };

	REQUIRE(
		version.getExecPath() == "C:/Atlas Games/Galaxy Crossing First Conquest/Galaxy Crossing First Conquest.exe" );
}

TEST_CASE( "Database benches", "[!benchmark]" )
{
	std::filesystem::remove( "test.db" );
	Database::initalize( "test.db" );

	Game record { importRecord( "test", "test", "test" ) };

	int counter { 0 };

	BENCHMARK_ADVANCED( "Add versions" )( Catch::Benchmark::Chronometer meter )
	{
		meter.measure(
			[ & ]
			{
				VersionInfo info {
					QString::fromStdString( fmt::format( "v{}.0", counter ) ),
					"C:\\games\\my_game",
					"game.exe",
				};

				record.addVersion( info );

				++counter;
			} );
	};

	Database::deinit();
	std::filesystem::remove( "test.db" );
}