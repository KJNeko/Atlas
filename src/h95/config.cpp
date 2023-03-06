//
// Created by kj16609 on 3/6/23.
//

#include "config.hpp"

//! Returns the canonical path for `path/data` (Default `./data/`)
std::filesystem::path dataPath()
{
	const std::filesystem::path path { getSettings< QString >( "paths/data", "./data/" ).toStdString() };
	if ( !std::filesystem::exists( path ) ) [[unlikely]]
		std::filesystem::create_directories( path );
	return std::filesystem::canonical( path );
}

//! Returns the canonical path for `path/images` (Default `./data/images`)
std::filesystem::path imagePath()
{
	const std::filesystem::path path { getSettings< QString >( "paths/images", "./data/images" ).toStdString() };
	if ( !std::filesystem::exists( path ) ) [[unlikely]]
		std::filesystem::create_directories( path );
	return std::filesystem::canonical( path );
}

//! Returns the canonical path for `path/games` (Default `./data/igames`)
std::filesystem::path gamePath()
{
	const std::filesystem::path path { getSettings< QString >( "paths/games", "./data/games" ).toStdString() };
	if ( !std::filesystem::exists( path ) ) [[unlikely]]
		std::filesystem::create_directories( path );
	return std::filesystem::canonical( path );
}
