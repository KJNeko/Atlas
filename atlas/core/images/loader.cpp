//
// Created by kj16609 on 9/22/23.
//

#include "loader.hpp"

#include <QtConcurrentRun>

#include "core/utils/ImageCache/ImageCache.hpp"
#include "core/utils/threading/pools.hpp"
#include "images.hpp"

inline static atlas::cache::ImageCache scale_cache;
inline static atlas::cache::ImageCache pixmap_cache;

namespace atlas::images
{

	const std::string
		pixmapKey( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
	{
		return format_ns::
			format( "{}x{}:{}:{}", target_size.width(), target_size.height(), static_cast< int >( scale_type ), path );
	}

	QPixmap scalePixmap( QPixmap img, const QSize target_size, const SCALE_TYPE scale_type )
	{
		atlas::logging::debug( "Scaling image to {}x{}", target_size.width(), target_size.height() );

		img = img.scaled( target_size, Qt::AspectRatioMode( scale_type ) );

		if ( scale_type == SCALE_TYPE::KEEP_ASPECT_RATIO_BY_EXPANDING )
		{
			const QRect crop_rect { 0, 0, target_size.width(), target_size.height() };
			img = img.copy( crop_rect ); //Crop banner when using the fill scaling option
		}

		return img;
	}

	QPixmap loadScaledPixmap( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
	{
		atlas::logging::debug( "Loading image: {}", path );
		return scalePixmap( atlas::images::loadPixmap( path ), target_size, scale_type );
	}

	QImage loadImage( const std::filesystem::path& path )
	{
		QImage image;

		if ( path.empty() )
			throw ImageLoadError( format_ns::format( "Failed to load image: Path was empty", path ).c_str() );

		if ( !std::filesystem::exists( path ) )
			throw ImageLoadError( format_ns::format( "Failed to load image: {} does not exist", path ).c_str() );

		if ( !image.load( QString::fromStdString( path.string() ) ) ) // If we fail to load just throw
			throw ImageLoadError( format_ns::format( "Failed to load {}. Load returned false.", path ).c_str() );

		if ( image.isNull() ) // Check null
			throw ImageLoadError( format_ns::format( "Failed to load {}. Pixmap null.", path ).c_str() );

		if ( image.size() == QSize( 0, 0 ) ) // Check not a stupid size
			throw ImageLoadError( format_ns::format( "Failed to load {}. Pixmap size was 0x0.", path ).c_str() );

		return image;
	}

	QPixmap loadPixmap( const std::filesystem::path& path )
	{
		if ( path.empty() ) throw ImageLoadError( "Path was empty!" );

		if ( !std::filesystem::exists( path ) )
			throw ImageLoadError( format_ns::format( "Invalid path {} does not exist", path ).c_str() );

		if ( auto opt = pixmap_cache.find( path.string() ); opt.has_value() )
			return opt.value();
		else
		{
			QPixmap pixmap;
			pixmap.load( QString::fromStdString( path.string() ) );
			pixmap_cache.insert( path, pixmap );
			return pixmap;
		}
	}

	namespace async
	{
		QFuture< QPixmap >
			loadScaledPixmap( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
		{
			if ( path.empty() ) throw ImageLoadError( "Failed to load image. Path empty" );

			if ( auto pixmap_opt = scale_cache.find( pixmapKey( target_size, scale_type, path ) );
			     pixmap_opt.has_value() )
				return QtFuture::makeReadyFuture( pixmap_opt.value() );
			else
				return QtConcurrent::run( &globalPools().image_loaders, &atlas::images::loadPixmap, path )
				    .then(
						[ target_size, scale_type, path ]( QPixmap pixmap )
						{
							pixmap = atlas::images::scalePixmap( pixmap, target_size, scale_type );

							//Insert into the cache now that we have resized it.
							scale_cache.insert( pixmapKey( target_size, scale_type, path ), pixmap );

							return pixmap;
						} );
		}

		QFuture< QPixmap > loadPixmap( const std::filesystem::path& path )
		{
			if ( path.empty() ) throw ImageLoadError( "Failed to load image. Empty path" );

			return QtConcurrent::run( &globalPools().image_loaders, &atlas::images::loadPixmap, path );
		}
	} // namespace async

} // namespace atlas::images