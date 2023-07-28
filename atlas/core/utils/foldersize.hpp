//
// Created by kj16609 on 3/27/23.
//

#ifndef ATLAS_FOLDERSIZE_HPP
#define ATLAS_FOLDERSIZE_HPP

#include <filesystem>

class FileScanner;

namespace atlas::utils
{

	//! Returns the byte size of a folder
	std::size_t folderSize( FileScanner& folder );

	std::size_t folderSize( const std::filesystem::path& path );
} // namespace atlas::utils

#endif //ATLAS_FOLDERSIZE_HPP
