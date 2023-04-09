//
// Created by kj16609 on 3/27/23.
//

#ifndef ATLAS_FOLDERSIZE_HPP
#define ATLAS_FOLDERSIZE_HPP

#include <filesystem>

#include <tracy/Tracy.hpp>

#include "h95/logging.hpp"

//! Returns the byte size of a folder
inline std::size_t folderSize( const std::filesystem::path& folder )
{
	ZoneScoped;
	std::size_t counter { 0 };

	spdlog::debug( "Calculating filesize of folder {}", folder );

	if ( !std::filesystem::exists( folder ) ) return 0;

	for ( const auto& file : std::filesystem::recursive_directory_iterator( folder ) )
		if ( file.is_regular_file() ) counter += std::filesystem::file_size( file );

	return counter;
}

#endif //ATLAS_FOLDERSIZE_HPP
