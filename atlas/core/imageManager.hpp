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

struct ImageManagerException : public AtlasException
{
	ImageManagerException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
	  AtlasException( msg, loc )
	{}
};

struct ImageImportError : public ImageManagerException
{
	ImageImportError( const char* const msg, const std::source_location loc = std::source_location::current() ) :
	  ImageManagerException( msg, loc )
	{}
};

struct ImageSaveError : public ImageManagerException
{
	ImageSaveError( const char* const msg, const std::source_location loc = std::source_location::current() ) :
	  ImageManagerException( msg, loc )
	{}
};

struct ImageLoadError : public ImageManagerException
{
	ImageLoadError( const char* const msg, const std::source_location loc = std::source_location::current() ) :
	  ImageManagerException( msg, loc )
	{}
};

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
