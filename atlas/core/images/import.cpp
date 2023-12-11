//
// Created by kj16609 on 9/22/23.
//

#include "import.hpp"

#include <QImage>
#include <QNetworkAccessManager>
#include <QtConcurrent>

#include <fstream>

#include "blurhash.hpp"
#include "core/logging/logging.hpp"
#include "core/network/NetworkManager.hpp"
#include "core/utils/crypto.hpp"
#include "core/utils/threading/pools.hpp"
#include "images.hpp"

namespace atlas::images
{

	void saveImage( const QByteArray& byteArray, const std::filesystem::path& dest )
	{
		if ( std::ofstream ofs( dest, std::ios::binary ); ofs )
		{
			ofs.write( byteArray.data(), byteArray.size() );
		}
		else
			throw AtlasException( format_ns::format( "Failed to open file to write at: {}", dest ) );
	}

	std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id )
	{
		if ( !std::filesystem::exists( path ) )
			throw AtlasException( format_ns::format( "Invalid path: {} does not exist", path ) );

		const QPixmap img( QString::fromStdString( path.string() ) );

		const QImage image { img.toImage() };
		const std::uint64_t original_file_size { std::filesystem::file_size( path ) };
		const std::filesystem::path game_image_folder { config::paths::images::getPath() / std::to_string( game_id ) };
		std::filesystem::create_directories( game_image_folder );

		//Determine if the image is able to be fit as a webp
		constexpr int WEBP_MAX { 1 << 14 }; //16384x16384
		if ( image.size().width() > WEBP_MAX || image.size().height() > WEBP_MAX )
		{
			//Save image as what it was originally
			const auto dest { game_image_folder / ( crypto::sha256::hashFile( path ) + path.extension().string() ) };
			std::filesystem::copy_file( path, dest, std::filesystem::copy_options::skip_existing );
			return dest;
		}

		QByteArray webp_file_data;
		QBuffer webp_file_buffer { &webp_file_data };
		image.save( &webp_file_buffer, "webp", 95 );

		//Create blurhash.
		if ( config::experimental::use_blurhash::get() ) (void)createBlurhash( image );

		if ( webp_file_data.size() > static_cast< qsizetype >( original_file_size ) )
		{
			//Save image as what it was originally
			const auto dest { game_image_folder / ( crypto::sha256::hashFile( path ) + path.extension().string() ) };
			std::filesystem::copy_file( path, dest, std::filesystem::copy_options::skip_existing );
			return dest;
		}
		else
		{
			const auto dest { game_image_folder / ( atlas::crypto::sha256::hash( webp_file_data ) + ".webp" ) };
			saveImage( webp_file_data, dest );
			return dest;
		}
	}

	std::filesystem::path importPixmap( const QPixmap pixmap, const RecordID game_id )
	{
		const QImage image = pixmap.toImage();
		const std::filesystem::path game_image_folder { config::paths::images::getPath() / std::to_string( game_id ) };
		std::filesystem::create_directories( game_image_folder );

		QByteArray webp_file_data;
		QBuffer webp_file_buffer { &webp_file_data };
		image.save( &webp_file_buffer, "webp", 95 );

		//Create blurhash.
		if ( config::experimental::use_blurhash::get() ) (void)createBlurhash( image );

		const auto dest { game_image_folder / ( atlas::crypto::sha256::hash( webp_file_data ) + ".webp" ) };
		saveImage( webp_file_data, dest );
		return dest;
	}

	std::filesystem::path importImageFromURL( const QString url, const RecordID record_id )
	{
		atlas::logging::debug( "Importing image from url: \"{}\"", url );

		auto future { atlas::network::NetworkManager::instance().getImage( url ) };

		if ( future.isCanceled() )
		{
			throw AtlasException( format_ns::format( "Failed to download image from url: \"{}\".", url ).c_str() );
		}

		future.waitForFinished();
		if ( !future.isValid() )
			throw AtlasException( format_ns::
			                          format( "Failed to download image from url: \"{}\".\nFuture was invalid!", url )
			                              .c_str() );
		QImage pixmap { future.result() };

		if ( pixmap.isNull() )
		{
			throw AtlasException( format_ns::format( "Failed to download image from url: \"{}\".", url ).c_str() );
		}

		return importPixmap( QPixmap::fromImage( pixmap ), record_id );
	}

	namespace async
	{
		[[nodiscard]] QFuture< std::filesystem::path > importImageFromURL( const QString url, const RecordID record_id )
		{
			return QtConcurrent::
				run( &( globalPools().image_importers ), &atlas::images::importImageFromURL, url, record_id );
		}

		[[nodiscard]] QFuture< std::filesystem::path >
			importImage( const std::filesystem::path& path, const RecordID game_id )
		{
			return QtConcurrent::run( &( globalPools().image_importers ), &atlas::images::importImage, path, game_id );
		}

	} // namespace async
} // namespace atlas::images
