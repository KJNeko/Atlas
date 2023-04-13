//
// Created by kj16609 on 4/8/23.
//

#include <filesystem>

#include <gtest/gtest.h>

#include "atlas/utils/FileScanner.hpp"

TEST( FileScannerTest, test )
{
	FileScanner scanner { "./tests" };

	std::filesystem::create_directories( "./tests/dummydir/first/second" );
	std::filesystem::create_directories( "./tests/dummydir/third/sixth" );
	std::filesystem::create_directories( "./tests/dummydir/third/fourth" );

	const std::vector< std::string > str { "dummydir", "first", "third", "second", "fourth", "sixth" };

	std::vector< std::string > found {};

	for ( const auto& file : scanner ) found.emplace_back( file.filename );

	GTEST_ASSERT_EQ( str, found );
}

TEST( FileScannerTest, testBreak )
{
	FileScanner scanner { "./tests" };

	std::filesystem::create_directories( "./tests/dummydir/first/second" );
	std::filesystem::create_directories( "./tests/dummydir/third/sixth" );
	std::filesystem::create_directories( "./tests/dummydir/third/fourth" );

	const std::vector< std::string > str { "dummydir", "first",  "third", "dummydir", "first",
		                                   "third",    "second", "fourth", "sixth" };

	std::vector< std::string > found {};

	for ( const auto& file : scanner )
	{
		if ( found.size() > 2 ) break;
		found.emplace_back( file.filename );
	}

	for ( const auto& file : scanner ) found.emplace_back( file.filename );

	GTEST_ASSERT_EQ( str, found );
}

TEST( FileScannerTest, testEmpty )
{
	FileScanner scanner { "./tests/dummydir/first/second" };

	std::vector< std::string > found {};

	for ( const auto& file : scanner ) found.emplace_back( file.filename );

	GTEST_ASSERT_EQ( found.size(), 1 );
}