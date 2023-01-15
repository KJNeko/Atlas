//
// Created by kj16609 on 1/14/23.
//

#include <gtest/gtest.h>



#define F95PARSER_JSON ""
#define DLSITEPARSER_JSON ""

//User inputs f95parser:language:english
//SELECT uid FROM f95parser_info json_tree(text) WHERE path = '$.language' AND value = 'english' COLLATE NOCASE;

TEST(SearchQuery, GenerationSingle)
{
	constexpr Parsers parsers {F95PARSER_JSON, DLSITEPARSER_JSON};

	constexpr std::string_view test_str {"f95parser:language:english"};

	constexpr auto query {parsers.generateSearch(test_str)};
}

// Testing for multiple
TEST(SearchQuery, GenerationMulti)
{

	constexpr Parsers parsers {F95PARSER_JSON, DLSITEPARSER_JSON};

	constexpr std::string_view test_str {"language:english"};
	constexpr std::string_view desired {"SELECT uid FROM parser_view WHERE "};

	constexpr auto query {parsers.generateSearch(test_str)};



}





