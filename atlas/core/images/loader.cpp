//
// Created by kj16609 on 9/22/23.
//

#include "loader.hpp"

#include <QImageReader>
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
			pixmap_cache.insert( path.string(), pixmap );
			return pixmap;
		}
	}

	QPixmap loadScaledPixmap( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
	{
		atlas::logging::debug( "Loading image: {}", path );

		QImageReader reader;
		const QSize image_size { reader.size() };

		//Default to the other loader if the image size is invalid
		if ( image_size == QSize() ) return scalePixmap( atlas::images::loadPixmap( path ), target_size, scale_type );

		//Calculate the size we need to load for each scaling tyle
		switch ( scale_type )
		{
			case IGNORE_ASPECT_RATIO:
				[[fallthrough]];
			case KEEP_ASPECT_RATIO:
				[[fallthrough]];
			case KEEP_ASPECT_RATIO_BY_EXPANDING:
				{
					reader.setScaledSize( image_size.scaled( target_size, Qt::AspectRatioMode( scale_type ) ) );
					break;
				}
			case FIT_BLUR_EXPANDING:
				[[fallthrough]];
			case FIT_BLUR_STRETCH:
				{
					reader.setScaledSize( image_size.scaled( target_size, Qt::KeepAspectRatio ) );
					break;
				}
		}

		return QPixmap::fromImage( reader.read() );
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

	namespace internal
	{
		void loadPixmap( QPromise< QPixmap >& promise, const std::filesystem::path& path )
		{
			if ( promise.isCanceled() ) return;
			if ( path.empty() ) throw ImageLoadError( "Path was empty!" );

			if ( !std::filesystem::exists( path ) )
				throw ImageLoadError( format_ns::format( "Invalid path {} does not exist", path ).c_str() );

			if ( promise.isCanceled() ) return;
			if ( auto opt = pixmap_cache.find( path.string() ); opt.has_value() )
				promise.addResult( opt.value() );
			else
			{
				if ( promise.isCanceled() ) return;
				QPixmap pixmap;
				pixmap.load( QString::fromStdString( path.string() ) );
				pixmap_cache.insert( path.string(), pixmap );
				promise.addResult( std::move( pixmap ) );
			}
		}

		void loadScaledPixmap(
			QPromise< QPixmap >& promise,
			const QSize target_size,
			const SCALE_TYPE scale_type,
			const std::filesystem::path& path )
		{
			if ( promise.isCanceled() ) return;
			atlas::logging::debug( "Loading image: {}", path );

			if ( !std::filesystem::exists( path ) )
				throw AtlasException( format_ns::format( "Failed to find image at path {}", path ) );

			QImageReader reader { QString::fromStdString( path.string() ) };
			const QSize image_size { reader.size() };
			const auto key { pixmapKey( target_size, scale_type, path ) };

			//Default to the other loader if the image size is invalid
			if ( image_size == QSize() )
			{
				atlas::logging::
					warn( "Image was not readable by QImageReader. Falling back to alternative loading method" );
				const auto pixmap { scalePixmap( atlas::images::loadPixmap( path ), target_size, scale_type ) };
				scale_cache.insert( key, pixmap );
				promise.addResult( std::move( pixmap ) );
			}

			//Calculate the size we need to load for each scaling tyle
			switch ( scale_type )
			{
				case IGNORE_ASPECT_RATIO:
					[[fallthrough]];
				case KEEP_ASPECT_RATIO:
					[[fallthrough]];
				case KEEP_ASPECT_RATIO_BY_EXPANDING:
					{
						reader.setScaledSize( image_size.scaled( target_size, Qt::AspectRatioMode( scale_type ) ) );
						break;
					}
				default:
					[[fallthrough]];
				case FIT_BLUR_EXPANDING:
					[[fallthrough]];
				case FIT_BLUR_STRETCH:
					{
						reader.setScaledSize( image_size.scaled( target_size, Qt::KeepAspectRatio ) );
						break;
					}
			}
			if ( promise.isCanceled() ) return;

			const auto pixmap { QPixmap::fromImage( reader.read() ) };
			scale_cache.insert( key, pixmap );
			atlas::logging::debug(
				"Finished loading image: {} with size {}x{}", path, pixmap.size().width(), pixmap.size().width() );
			promise.addResult( std::move( pixmap ) );
		}

	} // namespace internal

	namespace async
	{
		QFuture< QPixmap >
			loadScaledPixmap( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
		{
			if ( path.empty() ) throw ImageLoadError( "Failed to load image. Path empty" );

			const auto key { pixmapKey( target_size, scale_type, path ) };

			if ( auto pixmap_opt = scale_cache.find( key ); pixmap_opt.has_value() )
				return QtFuture::makeReadyFuture( pixmap_opt.value() );
			else
				return QtConcurrent::
					run( &globalPools().image_loaders,
				         &atlas::images::internal::loadScaledPixmap,
				         target_size,
				         scale_type,
				         path );
		}

		QFuture< QPixmap > loadPixmap( const std::filesystem::path& path )
		{
			if ( path.empty() ) throw ImageLoadError( "Failed to load image. Empty path" );

			return QtConcurrent::run( &globalPools().image_loaders, &atlas::images::internal::loadPixmap, path );
		}
	} // namespace async

} // namespace atlas::images