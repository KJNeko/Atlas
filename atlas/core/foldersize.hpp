//
// Created by kj16609 on 3/27/23.
//

#ifndef ATLAS_FOLDERSIZE_HPP
#define ATLAS_FOLDERSIZE_HPP

#include <filesystem>

class FileScanner;

//! Returns the byte size of a folder
std::size_t folderSize( FileScanner& folder );

std::size_t folderSize( const std::filesystem::path& path );

#endif //ATLAS_FOLDERSIZE_HPP
