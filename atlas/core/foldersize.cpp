//
// Created by kj16609 on 6/4/23.
//

#include "foldersize.hpp"

#include "core/logging.hpp"
#include "core/utils/FileScanner.hpp"

//! Returns the byte size of a folder
std::size_t folderSize( FileScanner& folder )
{
	std::size_t counter { 0 };

	spdlog::debug( "Calculating filesize of folder {}", folder.path() );

	for ( const auto& file : folder ) counter += file.size;

	spdlog::debug( "Completed scanning files with a total size of {} bytes", counter );

	return counter;
}

std::size_t folderSize( const std::filesystem::path& path )
{
	if ( !std::filesystem::exists( path ) || std::filesystem::is_empty( path ) ) return 0;

	FileScanner scanner { path };
	return folderSize( scanner );
}
