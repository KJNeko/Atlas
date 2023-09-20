//
// Created by kj16609 on 3/2/23.
//

#ifndef ATLAS_IMAGEMANAGER_HPP
#define ATLAS_IMAGEMANAGER_HPP

#include <QPixmap>

#include <filesystem>

#include "Types.hpp"
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

namespace imageManager
{
	//! Clears all images not found attached to any record
	void cleanOrphans();

	//! Stores the image located at `path` in the data folder
	//std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id );

	//! Stores the image located at `path` in the data folder
	[[nodiscard]] QFuture< std::filesystem::path >
		importImage( const std::filesystem::path& path, const RecordID game_id );

	/*
	[[nodiscard]] QByteArray hashData( const char* data_ptr, const int size );
	[[nodiscard]] std::filesystem::path
		getDestFilePath( const QByteArray& byteArray, const std::filesystem::path& dest_root, const std::string& ext );
	 */

} // namespace imageManager

#endif //ATLAS_IMAGEMANAGER_HPP
