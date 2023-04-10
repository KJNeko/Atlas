//
// Created by kj16609 on 4/6/23.
//

#include <assert.h>

#include <gtest/gtest.h>

#include "atlas/search/QueryBuilder.hpp"

// system:filesize > 2G & creator:my name & !( engine:unity | engine:ren'py )

/*

 system:filesize > 2G & creator:my name & !

 engine:unity | engine:ren'py



SELECT record_id FROM records WHERE

 record_id IN (SELECT record_id FROM game_metadata GROUP BY record_id HAVING sum(folder_size) > 256)

 AND creator = 'my name'
 AND NOT(engine NOT LIKE 'unity' AND engine NOT LIKE 'ren''py');



 */

TEST( Search, QueryGen )
{
	const std::string str { "system:size > 2G & creator:my name & !( engine:unity | engine:ren'py )" };
	const std::string generated_str { generateQuery( str ) };

	GTEST_ASSERT_EQ(
		"SELECT record_id FROM records WHERE record_id IN (SELECT record_id FROM game_metadata GROUP BY record_id HAVING sum(folder_size) > 2000000000) AND creator LIKE 'my name' AND NOT ( engine LIKE 'unity' OR engine LIKE 'ren'py' )",
		generated_str );
}

TEST( Search, QueryGen2 )
{
	const std::string str { "tag:3d & tag:animated & engine:renpy & system:size < 5G" };
	const std::string generated_str { generateQuery( str ) };

	GTEST_ASSERT_EQ(
		"SELECT record_id FROM records WHERE record_id IN (SELECT record_id FROM tag_mappings NATURAL JOIN tags WHERE tag LIKE \'3d\') AND record_id IN (SELECT record_id FROM tag_mappings NATURAL JOIN tags WHERE tag LIKE \'animated\') AND engine LIKE 'renpy' AND record_id IN (SELECT record_id FROM game_metadata GROUP BY record_id HAVING sum(folder_size) > 5000000000)",
		generated_str );
}

TEST( Search, parseBytestring )
{
	constexpr std::string_view str { "245G" };

	GTEST_ASSERT_EQ( "245000000000", parseBytesize( str ) );
}

TEST( Search, parseBytesize2 )
{
	constexpr std::string_view str { " 2G" };

	GTEST_ASSERT_EQ( "2000000000", parseBytesize( str ) );
}

TEST( Search, extractUntilNext )
{
	std::string_view str { "system:rating > 2 & creator:my name" };

	GTEST_ASSERT_EQ( "system:rating > 2", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( str, "& creator:my name" );
}

// Testing extraction behaviour for if there are no namespaces or operators
// We should just extract the rest of the string at that point
TEST( Search, extractUntilNext2 )
{
	std::string_view str { "rating > 2" };

	GTEST_ASSERT_EQ( "rating > 2", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( str, "" );
}

// Testing behaviour that we should extract anything appending the string
TEST( Search, extractUntilNext3 )
{
	std::string_view str { " system:rating > 2" };

	GTEST_ASSERT_EQ( "", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( str, "system:rating > 2" );
}

// Testing normal extraction behaviour
TEST( Search, extractUntilNext4 )
{
	std::string_view str { "system:rating > 2" };

	GTEST_ASSERT_EQ( "system:rating > 2", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( str, "" );
}

TEST( Search, extractUntilNextFull )
{
	std::string_view str { "system:filesize > 2G & creator:my name & !( engine:unity | engine:ren'py )" };

	GTEST_ASSERT_EQ( "system:filesize > 2G", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "&", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "creator:my name", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "&", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "!", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "( engine:unity | engine:ren'py )", extractUntilNext( str ) );
}

TEST( Search, extractUntilNextFullEscaped )
{
	std::string_view str { "system:filesize > 2G & creator:my name & !\\( engine:unity | engine:ren'py \\)" };

	GTEST_ASSERT_EQ( "system:filesize > 2G", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "&", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "creator:my name", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "&", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "!\\(", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "engine:unity", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "|", extractUntilNext( str ) );
	GTEST_ASSERT_EQ( "engine:ren'py \\)", extractUntilNext( str ) );
}

TEST( Search, splitNamespace )
{
	const std::string_view str { "mynamespace:mytext" };

	const auto [ namespace_text, sub ] = seperateNamespace( str );
	GTEST_ASSERT_EQ( namespace_text, "mynamespace" );
	GTEST_ASSERT_EQ( sub, "mytext" );
}
