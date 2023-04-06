//
// Created by kj16609 on 4/5/23.
//

#ifndef HYDRUS95_QUERYBUILDER_HPP
#define HYDRUS95_QUERYBUILDER_HPP

#include <array>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include <h95/logging.hpp>

#include "h95/system.hpp"

std::string_view trimSpaces( std::string_view str );

std::string parseBytesize( std::string_view str );

std::string generateQuery( const std::string str );

std::pair< std::string_view, std::string_view > seperateNamespace( const std::string_view str );

std::string processString( std::string_view str_view );

std::string escape( std::string_view str );

std::string_view extractUntilNext( std::string_view& str );

#endif //HYDRUS95_QUERYBUILDER_HPP
