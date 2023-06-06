//
// Created by kj16609 on 6/4/23.
//

#include "foldersize.hpp"

#include <tracy/Tracy.hpp>

#include "core/utils/FileScanner.hpp"

//! Returns the byte size of a folder
std::size_t folderSize( FileScanner& folder )
{
	ZoneScoped;
	std::size_t counter { 0 };

	for ( const auto& file : folder ) counter += file.size;

	return counter;
}

std::size_t folderSize( const std::filesystem::path& path )
{
	ZoneScoped;
	if ( !std::filesystem::exists( path ) || std::filesystem::is_empty( path ) ) return 0;

	FileScanner scanner { path };
	return folderSize( scanner );
}
