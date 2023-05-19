//
// Created by kj16609 on 4/5/23.
//

#ifndef ATLAS_QUERYBUILDER_HPP
#define ATLAS_QUERYBUILDER_HPP

#include <array>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/logging.hpp"
#include "core/system.hpp"

/**
 * @page SearchParsing Searching
 * In order to understand the basics of searching one concept has to be developed.
 * The concept of `namespace:subsection`. Namespaces allow for the search parser
 * to very easily determine what specific sections of text are. All strings must have a valid namespace. Lacking one is considered undefined behaviour. A full list of all the namespaces
 * and what they do can be found in the @ref NamespaceParsingList "Namespace List" section and a list of valid operators in the @ref OperatorParsingList "Operator List" section
 *
 *
 * @anchor NamespaceParsingList
 * The following is a list of all valid namespaces currently implemented.
 * | Name | Description | Example | Required Atlas version |
 * |------|------|---------------|--|
 * | system | System specific tags that are used for more complicate searches. See @ref SystemParsingList for all the valid subsections | system:filesize | v1.0.0 |
 * | title | Searches for a specific title | title:Haremon | v1.0.0 |
 * | creator | Searches for a specific creator/developer | creator:TsunAmie | v1.0.0 |
 * | version | Searches for a specific version text | version:v1.0 | v1.0.0 |
 *
 *
 * @anchor SystemParsingList
 * The following is a list of all valid `system:` subtags
 * | Name | Description | Example | What it does |
 * |------|------|---------|-|
 * | filesize | Searches for a specific filesize (End in `B, K, M, G or T`) | system:filesize > 5G | searches for all games that have a total size of above 5GB |
 *
 * @anchor OperatorParsingList Operator List
 * | Symbol | Description | Example | What it does |
 * |-|-|-|-|
 * | & | And | title:Haremon & creator:TsunAmie | Searches for all records with the title Haremon AND creator TsunAmie |
 * | \| | Or | title:Haremon \| creator:TsunAmie | Searches for all records with the title Haremon OR creator TsunAmie |
 * | ! | Not | !title:Haremon | Searches for all records with the title NOT being Haremon |
 * | () | Group | system:filesize > 5G & (title:FirstGame \| title:SecondGame) | Searches for all titles with the name `FirstGame` or `SecondGame` that also have a filesize above 5GB
 *
 */

enum SortOrder
{
	Name,
	Creator,
	Engine,
	Time
};

std::string_view trimSpaces( std::string_view str );

std::string parseBytesize( std::string_view str );

std::string generateQuery( const std::string str, const SortOrder order, const bool asc );

std::pair< std::string_view, std::string_view > seperateNamespace( const std::string_view str );

std::string processString( std::string_view str_view );

std::string escape( std::string_view str );

std::string orderToStr( const SortOrder order );

std::string_view extractUntilNext( std::string_view& str );

#endif //ATLAS_QUERYBUILDER_HPP
