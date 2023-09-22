//
// Created by kj16609 on 9/22/23.
//

#ifndef ATLASGAMEMANAGER_THUMBNAILS_HPP
#define ATLASGAMEMANAGER_THUMBNAILS_HPP

#include <QImage>

#include <filesystem>

namespace atlas::images
{

	//! Creates a thumbnail for a given image.
	/**
	 * Will create a thumbnail next to the original image with a `.thumb` as it's extention
	 * @param image_path path to create a thumbnail for.
	 */
	std::filesystem::path createThumbnail( const std::filesystem::path& image_path );

	//! Returns the thumbnail path for an image. Will return an empty path if no thumbnail is made.
	std::filesystem::path thumbnailPath( const std::filesystem::path& image );

	//! Returns the thumbnail for a path. Path must be a TRUE image. NOT the thumbnail path
	QPixmap thumbnail( const std::filesystem::path& path );

} // namespace atlas::images

#endif //ATLASGAMEMANAGER_THUMBNAILS_HPP
