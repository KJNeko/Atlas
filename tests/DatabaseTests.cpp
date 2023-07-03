//
// Created by kj16609 on 5/17/23.
//

#include <QApplication>
#include <QFile>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#pragma GCC diagnostic pop

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

TEST_CASE( "Record", "[database][record]" )
{

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