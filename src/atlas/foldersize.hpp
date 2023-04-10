//
// Created by kj16609 on 3/27/23.
//

#ifndef ATLAS_FOLDERSIZE_HPP
#define ATLAS_FOLDERSIZE_HPP

#include <filesystem>

#include <tracy/Tracy.hpp>

#include "atlas/logging.hpp"
#include "atlas/utils/FileScanner.hpp"

//! Returns the byte size of a folder
inline std::size_t folderSize( FileScanner& folder )
{
	ZoneScoped;
	std::size_t counter { 0 };

	spdlog::debug( "Calculating filesize of folder {}", folder.path() );

	for ( const auto& file : folder ) counter += file.size;

	spdlog::debug( "Completed scanning files with a total size of {} bytes", counter );

	return counter;
}

inline std::size_t folderSize( const std::filesystem::path& path )
{
	ZoneScopedN( "Folder size bad" );
	FileScanner scanner { path };
	return folderSize( scanner );
}

#endif //ATLAS_FOLDERSIZE_HPP
