//
// Created by kj16609 on 3/2/23.
//

#ifndef ATLAS_IMAGEMANAGER_HPP
#define ATLAS_IMAGEMANAGER_HPP

#include <QPixmap>

#include <filesystem>

#include "core/Types.hpp"
#include "core/config/config.hpp"
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

	QImage blurImage( const QImage& image, qreal blur_radius, bool quality_blur, bool alpha_only, int transposed = 0 );
	QPixmap
		blurPixmap( const QPixmap& pixmap, qreal blur_radius, bool quality_blur, bool alpha_only, int transposed = 0 );

} // namespace atlas::images

#endif //ATLAS_IMAGEMANAGER_HPP
