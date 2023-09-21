//
// Created by kj16609 on 3/2/23.
//

#include "imageManager.hpp"

#include <QBuffer>
#include <QCryptographicHash>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QPixmap>
#include <QtConcurrentRun>

#include <tracy/TracyC.h>

#include <fstream>

#include "config.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/utils/ImageCache/ImageCache.hpp"
#include "core/utils/threading/pools.hpp"

namespace atlas::images
{
	void cleanOrphans()
	{
		ZoneScoped;
		atlas::logging::debug( "Clearing orphan previews/banners" );
		//Grab all images from the database
		RapidTransaction transaction {};

		for ( const auto& path : std::filesystem::directory_iterator( config::paths::images::getPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, config::paths::images::getPath() ).u8string()
				>> [ & ]( [[maybe_unused]] int count ) noexcept { found = true; };

			if ( !found ) std::filesystem::remove( path );
		}
	}

	[[nodiscard]] QByteArray hashData( const char* data_ptr, const int size ) // Hash data with Sha256
	{
		ZoneScopedN( "Hash" );
		QCryptographicHash hash { QCryptographicHash::Sha256 };

		hash.addData( { data_ptr, size } );

		return hash.result();
	}

	[[nodiscard]] std::filesystem::path getDestFilePath(
		const QByteArray& byteArray,
		const std::filesystem::path& dest_root,
		const std::string& ext ) // Use the image hash + ext as its filename
	{
		const QByteArray hash { hashData( byteArray, static_cast< int >( byteArray.size() ) ) };
		const std::filesystem::path dest { dest_root / ( hash.toHex().toStdString() + ext ) };
		return dest;
	}

	namespace internal
	{
		/**
		 *
		 * @param source Source path of the image. Used for logging purposes.
		 * @param byteArray
		 * @param dest
		 */
		void saveImage( const QByteArray& byteArray, const std::filesystem::path& dest )
		{
			const QImage img { QImage::fromData( byteArray ) };

			if ( img.isNull() || img.size() == QSize( 0, 0 ) )
			{
				throw ImageSaveError( format_ns::format( "QImage came back as null" ).c_str() );
			}

			const QImage thumb { img.scaled( 200, 94, Qt::KeepAspectRatio ) };
			const std::string thumb_file { dest.parent_path().string() + "//" + dest.stem().string() + "_thumb"
				                           + dest.extension().string() };
			//img.save( QString::fromStdString( dest.string() ) );
			thumb.save( QString::fromStdString( thumb_file ) );
			if ( !img.save( QString::fromStdString( dest.string() ) ) )
			{
				throw ImageSaveError( format_ns::format( "Failed to save image to location: {}", std::move( dest ) )
				                          .c_str() );
			}
			//Try to save thumbnail
		}

		[[nodiscard]] std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id )
		{
			//spdlog::debug( path );
			ZoneScoped;
			if ( !std::filesystem::exists( path ) )
			{
				atlas::logging::
					warn( "Attempted to open file {} which doesn't exist anymore. Wrong permissions?", path );
				throw ImportException( format_ns::format( "Filepath {} does not exist. Unable to add as image", path )
				                           .c_str() );
			}

			//Load file so we have direct access to the bytearray
			const QString qstr_file_path { QString::fromStdWString( path.wstring() ) };
			QFile file( qstr_file_path );
			if ( !file.open( QFile::ReadOnly ) )
			{
				atlas::logging::error( "Failed to open image file located at: {}", path );
				throw ImportException( format_ns::format( "Failed to load image from file: {}", path ).c_str() );
			}
			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			const QByteArray byteArray { file.readAll() };
			const QImage temp_image { QImage::fromData( byteArray ) };
			TracyCZoneEnd( tracy_ImageLoad );
			file.close();

			const std::string ext { path.extension().string().substr( 1 ) };
			const auto dest_root { config::paths::images::getPath() / std::to_string( game_id ) };
			std::filesystem::create_directories( dest_root );

			TracyCZoneN( tracy_SaveImage, "Image save to buffer as WEBP", true );
			QByteArray webp_byteArray;
			QBuffer webp_buffer( &webp_byteArray );
			temp_image.save( &webp_buffer, "webp", 95 );
			TracyCZoneEnd( tracy_SaveImage );

			constexpr std::uint16_t webp_max { 16383 };
			if ( ( temp_image.width() > webp_max )
			     || ( temp_image.height() > webp_max ) ) // Dimensions too big for WebP?
			{
				throw ImageSaveError( format_ns::format(
										  "File too big to be stored as webp: ({}x{}) vs ({}x{}) {}",
										  temp_image.width(),
										  temp_image.height(),
										  webp_max,
										  webp_max,
										  path )
				                          .c_str() );
			}

			//If GIF then store, do not convert
			if ( ext == "gif" )
			{
				const auto dest { getDestFilePath( byteArray, dest_root, path.extension().string() ) };
				//Qt is stupid and will not save gifs...  so we have to copy it
				//const bool file_copied { std::filesystem::copy_file( path, dest ) };
				if ( std::ofstream ofs( dest, std::ios::binary ); ofs )
				{
					ofs.write( byteArray.data(), byteArray.size() );
					return dest;
				}
				else
					throw ImportException( format_ns::
					                           format( "Unable to save gif to images folder: {}", path.filename() )
					                               .c_str() );
			}

			try
			{
				//if webp conversion is bigger then save original image
				if ( ( webp_buffer.size() >= byteArray.size() ) ) // Is WebP bigger? Write the other format.
				{
					auto dest { getDestFilePath( byteArray, dest_root, path.extension().string() ) };
					saveImage( byteArray, dest );

					return dest;
				}
				else
				{
					auto dest { getDestFilePath( webp_byteArray, dest_root, ".webp" ) };
					saveImage( webp_byteArray, dest );

					return dest;
				}
			}
			catch ( ImageSaveError& e )
			{
				throw ImportException( format_ns::format( "Failed to save image {}: {}", path, e.whatRaw() ).c_str() );
			}
		}

		inline static atlas::cache::ImageCache scale_cache;

		void loadScaledImage(
			QPromise< QPixmap >& promise,
			const QSize target_size,
			const SCALE_TYPE scale_type,
			const std::filesystem::path path )
		try
		{
			const QRect bannerRect { 0, 0, target_size.width(), target_size.height() };

			if ( path.empty() )
			{
				promise.setException(
					std::make_exception_ptr( ImageLoadError( "Failed to load. Path for image was empty" ) ) );
				return;
			}

			if ( !std::filesystem::exists( path ) )
			{
				promise.setException( std::make_exception_ptr( ImageLoadError(
					format_ns::format( "Failed to load. Path for image was not a valid path: {}", path ).c_str() ) ) );
				return;
			}

			const auto key { format_ns::format(
				"{}x{}:{}:{}", target_size.width(), target_size.height(), static_cast< int >( scale_type ), path ) };

			if ( promise.isCanceled() ) return;
			if ( std::optional< QPixmap > pixmap_opt = scale_cache.find( key ); pixmap_opt.has_value() )
			{
				promise.addResult( pixmap_opt.value() );
				return;
			}
			else
			{
				QImageReader loader { QString::fromStdString( path.string() ) };
				const auto image_size { loader.size() };

				//Calculate the scale we want in order to fit the image properly.
				loader.setScaledSize( image_size.scaled( target_size, Qt::AspectRatioMode( scale_type ) ) );

				if ( promise.isCanceled() ) return;
				QPixmap pixmap { QPixmap::fromImageReader( &loader ) };

				if ( pixmap.isNull() )
				{
					throw ImageLoadError( format_ns::format( "Qt failed to load image {}. Pixmap was null.", path )
					                          .c_str() );
				}

				if ( pixmap.size() == QSize( 0, 0 ) )
				{
					throw ImageLoadError( format_ns::format( "Qt failed to load image {}. Pixmap was 0x0.", path )
					                          .c_str() );
				}

				if ( scale_type == SCALE_TYPE::KEEP_ASPECT_RATIO_BY_EXPANDING )
				{
					if ( promise.isCanceled() ) return;
					pixmap = pixmap.copy( bannerRect ); //Crop banner image. Mainly used for Fill scale option
				}

				scale_cache.insert( key, pixmap );
				promise.addResult( std::move( pixmap ) );
			}
		}
		catch ( ... ) // Catching... is fine here since we are gonna pass it to the promise anyways
		{
			promise.setException( std::current_exception() );
		}

		void loadImage( QPromise< QPixmap >& promise, const std::filesystem::path path )
		try
		{
			ZoneScoped;
			if ( promise.isCanceled() ) return;
			QPixmap pixmap;

			if ( promise.isCanceled() ) return;
			if ( path.empty() || !std::filesystem::exists( path ) )
			{
				promise.addResult( pixmap );
			}

			if ( promise.isCanceled() ) return;

			if ( !pixmap.load( QString::fromStdString( path.string() ) ) ) // If we fail to load just throw
				throw ImageLoadError( format_ns::format( "Failed to load {}. Load returned f.", path ).c_str() );
			if ( pixmap.isNull() ) // Check null
				throw ImageLoadError( format_ns::format( "Failed to load {}. Pixmap null.", path ).c_str() );
			if ( pixmap.size() == QSize( 0, 0 ) ) // Check not a stupid size
				throw ImageLoadError( format_ns::format( "Failed to load {}. Pixmap size was 0x0.", path ).c_str() );

			promise.addResult( pixmap );
		}
		catch ( ... )
		{
			promise.setException( std::current_exception() );
		}

	} // namespace internal

	[[nodiscard]] QFuture< std::filesystem::path >
		importImage( const std::filesystem::path& path, const RecordID game_id )
	{
		return QtConcurrent::run( &( globalPools().image_importers ), &internal::importImage, path, game_id );
	}

	QFuture< QPixmap >
		loadScaledImage( const QSize target_size, const SCALE_TYPE scale_type, const std::filesystem::path path )
	{
		const auto key { format_ns::format(
			"{}x{}:{}:{}", target_size.width(), target_size.height(), static_cast< int >( scale_type ), path ) };

		if ( auto pixmap_opt = internal::scale_cache.find( key ); pixmap_opt.has_value() )
			return QtFuture::makeReadyFuture( pixmap_opt.value() );
		else
			return QtConcurrent::run( &internal::loadScaledImage, target_size, scale_type, path );
	}

	QFuture< QPixmap > loadImage( const std::filesystem::path path )
	{
		return QtConcurrent::run( &globalPools().image_loaders, &internal::loadImage, path );
	}

} // namespace atlas::images
