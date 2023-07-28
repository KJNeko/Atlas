//
// Created by kj16609 on 3/2/23.
//

#ifndef ATLAS_IMAGEMANAGER_HPP
#define ATLAS_IMAGEMANAGER_HPP

#include <filesystem>

#include <QFuture>
#include <QPixmap>
#include <QSize>

#include "Types.hpp"

namespace imageManager
{
	//! Clears all images not found attached to any record
	void cleanOrphans();

	//! Stores the image located at `path` in the data folder
	//std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id );

	//! Stores the image located at `path` in the data folder
	QFuture< std::filesystem::path > importImage( const std::filesystem::path& path, const RecordID game_id );

} // namespace imageManager

#endif //ATLAS_IMAGEMANAGER_HPP
