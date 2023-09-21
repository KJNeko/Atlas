//
// Created by kj16609 on 3/2/23.
//

#ifndef ATLAS_IMAGEMANAGER_HPP
#define ATLAS_IMAGEMANAGER_HPP

#include <QPixmap>

#include <filesystem>

#include "Types.hpp"
#include "config.hpp"
#include "core/exceptions.hpp"

class QByteArray;

template < typename T >
class QFuture;

class ImageManagerException : public AtlasException
{};

class ImageImportError : public ImageManagerException
{};

class ImageSaveError : public ImageManagerException
{};

namespace atlas::images
{
	//! Clears all images not found attached to any record
	void cleanOrphans();

	//! Stores the image located at `path` in the data folder
	[[nodiscard]] QFuture< std::filesystem::path >
		importImage( const std::filesystem::path& path, const RecordID game_id );

	[[nodiscard]] QFuture< QPixmap >
		loadScaledImage( const QSize target_size, const SCALE_TYPE type, const std::filesystem::path );
	[[nodiscard]] QFuture< QPixmap > loadImage( const std::filesystem::path path );

} // namespace atlas::images

#endif //ATLAS_IMAGEMANAGER_HPP
