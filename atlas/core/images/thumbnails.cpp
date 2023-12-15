//
// Created by kj16609 on 9/22/23.
//

#include "thumbnails.hpp"

#include <QImageReader>
#include <QtConcurrentRun>

#include "core/exceptions.hpp"
#include "core/utils/ImageCache/ImageCache.hpp"
#include "core/utils/threading/pools.hpp"
#include "loader.hpp"

inline static atlas::cache::ImageCache thumb_cache;

namespace atlas::images
{
	QSize thumbnailSize()
	{
		return { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
	}

	std::filesystem::path createThumbnail( const std::filesystem::path& image_path )
	{
		atlas::logging::debug( "Creating thumbnail for {}", image_path );
		if ( !std::filesystem::exists( image_path ) )
			throw AtlasException( format_ns::format( "Invalid path: {} does not exist", image_path ) );

		QPixmap thumb { loadPixmap( image_path ) };

		if ( thumb.isNull() )
			throw AtlasException( format_ns::
			                          format( "Failed to load pixmap for {} while creating thumbnail", image_path ) );

		thumb = thumb.scaled( thumbnailSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

		const auto dest { thumbnailPath( image_path ) };
		std::filesystem::create_directories( dest.parent_path() );
		if ( !thumb.save( QString::fromStdString( dest.string() ), "webp", 95 ) )
			throw AtlasException( format_ns::format( "Failed to save thumbnail for {} to {}", image_path, dest ) );

		return dest;
	}

	std::filesystem::path thumbnailPath( const std::filesystem::path& image )
	{
		if ( !std::filesystem::exists( image ) )
			throw AtlasException(
				format_ns::format( "attempted to get a thumbanil path for a non existant path: {}", image ) );
		return config::paths::images::getPath() / "thumbnails" / ( image.stem().string() + ".thumb" );
	}

	/**
	 *
	 * @param path Path of the SOURCE image. NOT thumbnail
	 * @return
	 */
	QPixmap thumbnail( const std::filesystem::path& path )
	{
		logging::debug( "Gathering thumbnail for {}", path );
		if ( path.extension() == ".thumb" )
			throw AtlasException(
				"Invalid path provided to thumbnail. Must not provide the thumbnail path. Must provide the real image path" );

		if ( !std::filesystem::exists( path ) )
			throw AtlasException( format_ns::
			                          format( "Tried to provide a thumbnail for a non existent image: {}", path ) );

		const auto thumb_path { thumbnailPath( path ) };

		if ( auto opt = thumb_cache.find( thumb_path.string() ); opt.has_value() )
			return opt.value();
		else
		{
			logging::debug( "Thumbnail path should be: {}", thumb_path );

			if ( !std::filesystem::exists( thumb_path ) ) (void)createThumbnail( path );

			auto image { loadPixmap( thumb_path ) };

			thumb_cache.insert( thumb_path.string(), image );

			return image;
		}
	}

	//! Verifies that the thumbnail is the proper resolution and fixes it if not
	void verifyThumbnail( const std::filesystem::path& path )
	{
		const auto thumb_path { thumbnailPath( path ) };

		QImageReader original_reader { QString::fromStdString( path.string() ) };
		QImageReader thumb_reader { QString::fromStdString( thumb_path.string() ) };

		const auto target_size { thumbnailSize() };
		const auto expected_size { original_reader.size().scaled( target_size, Qt::KeepAspectRatio ) };

		if ( expected_size != thumb_reader.size() )
		{
			atlas::logging::debug(
				"Had to remake thumbnail for {} at {}. Expected size was {} but came back as {}",
				path,
				thumb_path,
				expected_size,
				target_size );
			createThumbnail( path );
		}
	}

	namespace async
	{
		namespace internal
		{
			void loadThumbnail( QPromise< QPixmap >& promise, const std::filesystem::path origin_path )
			{
				if ( promise.isCanceled() ) return;
				const auto thumb_path { atlas::images::thumbnailPath( origin_path ) };

				if ( !std::filesystem::exists( thumb_path ) )
					(void)createThumbnail( origin_path );
				else
					verifyThumbnail( origin_path );

				QPixmap pixmap { atlas::images::loadPixmap( thumb_path ) };

				thumb_cache.insert( thumb_path.string(), pixmap );
				promise.addResult( std::move( pixmap ) );
			}

			void loadScaledThumb(
				QPromise< QPixmap >& promise,
				const QSize size,
				const SCALE_TYPE scale_type,
				const std::filesystem::path origin_path )
			{
				if ( promise.isCanceled() ) return;
				try
				{
					const auto thumb_path { atlas::images::thumbnailPath( origin_path ) };

					if ( !std::filesystem::exists( thumb_path ) )
						(void)createThumbnail( origin_path );
					else
						verifyThumbnail( origin_path );

					QPixmap pixmap { atlas::images::loadScaledPixmap( size, scale_type, thumb_path ) };
					atlas::logging::debug(
						"Loaded thumb {} with size of {}x{}",
						thumb_path,
						pixmap.size().width(),
						pixmap.size().height() );

					const auto key { format_ns::format(
						"{}x{}-{}-{}", size.width(), size.height(), static_cast< int >( scale_type ), thumb_path ) };

					thumb_cache.insert( key, pixmap );
					promise.addResult( std::move( pixmap ) );
				}
				catch ( ... )
				{
					promise.setException( std::current_exception() );
					return;
				}
			}

		} // namespace internal

		QFuture< QPixmap > thumbnail( const std::filesystem::path& path )
		{
			const auto thumb_path { atlas::images::thumbnailPath( path ) };
			if ( auto opt = thumb_cache.find( thumb_path.string() ); opt.has_value() )
				return QtFuture::makeReadyFuture( std::move( opt.value() ) );
			else
				return QtConcurrent::run( &globalPools().image_loaders, &internal::loadThumbnail, path );
		}

		QFuture< QPixmap >
			scaledThumbnail( const QSize size, const SCALE_TYPE scale_type, const std::filesystem::path& path )
		{
			const auto thumb_path { atlas::images::thumbnailPath( path ) };
			const auto key {
				format_ns::
					format( "{}x{}-{}-{}", size.width(), size.height(), static_cast< int >( scale_type ), thumb_path )
			};

			if ( auto opt = thumb_cache.find( key ); opt.has_value() )
				return QtFuture::makeReadyFuture( std::move( opt.value() ) );
			else
				return QtConcurrent::
					run( &globalPools().image_loaders, &internal::loadScaledThumb, size, scale_type, path );
		}

	} // namespace async

} // namespace atlas::images
