//
// Created by kj16609 on 3/27/23.
//

#include "utils.hpp"

#include <tracy/Tracy.hpp>

std::size_t folderSize( const std::filesystem::path& folder )
{
	ZoneScoped;
	std::size_t counter { 0 };

	for ( const auto& file : std::filesystem::recursive_directory_iterator( folder ) )
		if ( file.is_regular_file() ) counter += std::filesystem::file_size( file );

	return counter;
}
