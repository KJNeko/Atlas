//
// Created by kj16609 on 4/8/23.
//

#include <filesystem>

#include <gtest/gtest.h>

#include "atlas/core/utils/FileScanner.hpp"

TEST( FileScannerTest, test )
{
	std::filesystem::create_directories( "./tmp/dummydir/first/second" );
	std::filesystem::create_directories( "./tmp/dummydir/third/sixth" );
	std::filesystem::create_directories( "./tmp/dummydir/third/fourth" );

	FileScanner scanner { "./tmp" };

	const std::vector< std::string > str { "dummydir", "first", "third", "second", "fourth", "sixth" };

	std::vector< std::string > found {};

	for ( const auto& file : scanner ) found.emplace_back( file.filename );

	GTEST_ASSERT_EQ( str, found );
}

TEST( FileScannerTest, testBreak )
{

	std::filesystem::create_directories( "./tmp/dummydir/first/second" );
	std::filesystem::create_directories( "./tmp/dummydir/third/sixth" );
	std::filesystem::create_directories( "./tmp/dummydir/third/fourth" );

	FileScanner scanner { "./tmp" };

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
	std::filesystem::create_directories( "./tmp/dummydir/first/second" );
	std::filesystem::create_directories( "./tmp/dummydir/third/sixth" );
	std::filesystem::create_directories( "./tmp/dummydir/third/fourth" );

	FileScanner scanner { "./tmp/dummydir/first/second" };

	std::vector< std::string > found {};

	for ( const auto& file : scanner ) found.emplace_back( file.filename );

	GTEST_ASSERT_EQ( found.size(), 1 );
}