//
// Created by kj16609 on 3/2/23.
//

#ifndef HYDRUS95_IMAGEMANAGER_HPP
#define HYDRUS95_IMAGEMANAGER_HPP

#include <filesystem>
#include <QSize>

namespace imageManager
{
	//! Returns the canonical preview path (Creates if it doesn't exist)
	std::filesystem::path getImagePath();

	void cleanOrphans();

	//! Stores the image located at `path` in the data folder
	std::filesystem::path importImage( const std::filesystem::path& path, bool delete_after = false );
}  // namespace imageManager


#endif	//HYDRUS95_IMAGEMANAGER_HPP
