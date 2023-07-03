//
// Created by kj16609 on 7/3/23.
//

//The following tests all constexpr query generation for
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <catch2/catch_test_macros.hpp>
#pragma GCC diagnostic pop

#include "core/database/Column.hpp"

TEST_CASE( "Test constexpr query generation", "[database][query][constexpr]" )
{
	SECTION( "update" )
	{
		constexpr std::string_view expected { "UPDATE table_name SET column = ? WHERE table_key_name = ?" };
		constexpr std::string_view actual {
			atlas::database::utility::update_query< "column", "table_name", "table_key_name" >()
		};

		REQUIRE( expected.size() == actual.size() );
		REQUIRE( expected == actual );
	}

	SECTION( "select" )
	{
		constexpr std::string_view expected { "SELECT column FROM table_name WHERE table_key_name = ?" };
		constexpr std::string_view actual {
			atlas::database::utility::select_query< "column", "table_name", "table_key_name" >()
		};

		REQUIRE( expected.size() == actual.size() );
		REQUIRE( expected == actual );
	}

	SECTION( "select_t_2" )
	{
		constexpr std::string_view expected { "SELECT col1, col2 FROM table_name WHERE table_key_name = ?" };
		constexpr std::string_view actual {
			atlas::database::utility::select_query_t< "table_name", "table_key_name", "col1", "col2" >()
		};

		REQUIRE( expected == actual );
	}

	SECTION( "select_t_5" )
	{
		constexpr std::string_view expected {
			"SELECT col1, col2, col3, col4, col5 FROM table_name WHERE table_key_name = ?"
		};
		constexpr std::string_view actual {
			atlas::database::utility::
				select_query_t< "table_name", "table_key_name", "col1", "col2", "col3", "col4", "col5" >()
		};

		REQUIRE( expected == actual );
	}
}
