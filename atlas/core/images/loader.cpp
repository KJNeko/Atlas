//
// Created by kj16609 on 9/22/23.
//

#include "loader.hpp"

#include <QImageReader>
#include <QPainter>
#include <QtConcurrentRun>

#include "core/utils/ImageCache/ImageCache.hpp"
#include "core/utils/literals/size_literals.hpp"
#include "core/utils/threading/pools.hpp"
#include "images.hpp"

using namespace atlas::literals::size_literals;

inline static atlas::cache::ImageCache scale_cache { 256_MiB };

namespace atlas::images
{

	auto scaledPixmapKey(
		const QPixmap& pixmap, const QSize& size, const SCALE_TYPE& scale_type, const Alignment& align_type )
	{
		const auto pixmap_key { pixmap.cacheKey() };

		std::size_t seed { 0 };
		seed ^= std::hash< std::size_t > {}( static_cast< unsigned long >( pixmap_key ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< unsigned long >( size.width() ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< unsigned long >( size.height() ) ) + 0x9e3779b9
		      + ( seed << 6 ) + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< std::size_t >( scale_type ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< std::size_t >( align_type ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );

		return seed;
	}

	QPixmap scalePixmap( QPixmap img, const QSize target_size, const SCALE_TYPE scale_type, const Alignment align_type )
	{
		const auto pixmap_key { scaledPixmapKey( img, target_size, scale_type, align_type ) };

		if ( img.isNull() ) return {};

		if ( auto cached = scale_cache.find( pixmap_key ); cached.has_value() )
		{
			return cached.value();
		}

		atlas::logging::debug(
			"Scaling image to {}x{} from {}x{}",
			target_size.width(),
			target_size.height(),
			img.size().width(),
			img.size().height() );

		img = img.scaled( target_size, Qt::AspectRatioMode( scale_type ), Qt::SmoothTransformation );
		QRect crop_rect { 0, 0, target_size.width(), target_size.height() };
		//Align image inside of the target_size
		if ( scale_type != SCALE_TYPE::FIT_BLUR_EXPANDING )
		{
			if ( align_type == CENTER )
			{
				crop_rect.moveCenter( QPoint( img.size().width() / 2, img.size().height() / 2 ) );
				img = img.copy( crop_rect ); //Crop banner when using the fill scaling option
			}
		}

		atlas::logging::debug(
			"Scaled image to {}x{} from {}x{}",
			target_size.width(),
			target_size.height(),
			img.size().width(),
			img.size().height() );

		scale_cache.insert( pixmap_key, img );

		return img;
	}

	QPixmap loadPixmap( const std::filesystem::path& path )
	{
		if ( path.empty() ) return {};

		if ( !std::filesystem::exists( path ) ) return {};

		if ( std::filesystem::file_size( path ) == 0 )
			throw ImageLoadError( format_ns::format( "Image at path {} was empty", path ).c_str() );

		QPixmap pixmap;
		pixmap.load( QString::fromStdString( path.string() ) );

		if ( pixmap.isNull() || pixmap.size() == QSize( 0, 0 ) )
			throw ImageLoadError( format_ns::format( "Failed to load pixmap for {}", path ).c_str() );

		atlas::logging::debug( "Loaded image: {}", path );
		return pixmap;
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

} // namespace atlas::images