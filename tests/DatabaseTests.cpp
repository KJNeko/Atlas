//
// Created by kj16609 on 5/17/23.
//

#include <QApplication>
#include <QFile>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
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
#include "core/database/GameMetadata.hpp"
#include "core/database/record/Record.hpp"

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
		const auto record { importRecord( "Test title", "Test creator", "Test engine" ) };

		SECTION( "Test getters" )
		{
			SECTION( "title" )
			{
				REQUIRE( record->get< RecordColumns::Title >() == "Test title" );
			}
			SECTION( "creator" )
			{
				REQUIRE( record->get< RecordColumns::Creator >() == "Test creator" );
			}
			SECTION( "engine" )
			{
				REQUIRE( record->get< RecordColumns::Engine >() == "Test engine" );
			}
			SECTION( "last played" )
			{
				REQUIRE( record->get< RecordColumns::LastPlayed >() == 0 );
			}
			SECTION( "total playtime" )
			{
				REQUIRE( record->get< RecordColumns::TotalPlaytime >() == 0 );
			}
		}

		SECTION( "Test setters" )
		{
			SECTION( "title" )
			{
				REQUIRE_NOTHROW( record->set< RecordColumns::Title >( "new Test title" ) );
				REQUIRE( record->get< RecordColumns::Title >() == "new Test title" );
			}
			SECTION( "creator" )
			{
				REQUIRE_NOTHROW( record->set< RecordColumns::Creator >( "new Test creator" ) );
				REQUIRE( record->get< RecordColumns::Creator >() == "new Test creator" );
			}
			SECTION( "engine" )
			{
				REQUIRE_NOTHROW( record->set< RecordColumns::Engine >( "new Test engine" ) );
				REQUIRE( record->get< RecordColumns::Engine >() == "new Test engine" );
			}
			SECTION( "last played" )
			{
				REQUIRE_NOTHROW( record->set< RecordColumns::LastPlayed >( 5 ) );
				REQUIRE( record->get< RecordColumns::LastPlayed >() == 5 );
			}
			SECTION( "total playtime" )
			{
				REQUIRE_NOTHROW( record->set< RecordColumns::TotalPlaytime >( 5 ) );
				REQUIRE( record->get< RecordColumns::TotalPlaytime >() == 5 );
			}
		}
	}
}

TEST_CASE( "Config record", "[database]" )
{
	//Ensure that the test record exists
	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );

	Record record { 1 };

	REQUIRE( record->get< RecordColumns::Title >() == "Galaxy Crossing: First Conquest" );
	REQUIRE( record->get< RecordColumns::Engine >() == "Unity" );
	REQUIRE( record->get< RecordColumns::Creator >() == "Atlas Games" );

	const auto version { record->getVersion( "Chapter: 1" ) };

	REQUIRE(
		version->getExecPath() == "C:/Atlas Games/Galaxy Crossing First Conquest/Galaxy Crossing First Conquest.exe" );
}

TEST_CASE( "Database benches", "[!benchmark]" )
{
	std::filesystem::remove( "test.db" );
	Database::initalize( "test.db" );

	const Record record { importRecord( "test", "test", "test" ) };

	int counter { 0 };

	BENCHMARK_ADVANCED( "Add versions" )( Catch::Benchmark::Chronometer meter )
	{
		meter.measure(
			[ & ]
			{
				record->addVersion(
					QString::fromStdString( fmt::format( "v{}.0", counter ) ),
					"C:\\games\\my_game",
					"game.exe",
					1234,
					true );

				++counter;
			} );
	};

	Database::deinit();
	std::filesystem::remove( "test.db" );
}