//
// Created by kj16609 on 9/22/23.
//

#include "import.hpp"

#include <QImage>
#include <QtConcurrent>

#include <fstream>

#include "core/logging/logging.hpp"
#include "core/utils/crypto.hpp"
#include "core/utils/threading/pools.hpp"
#include "images.hpp"
#include "loader.hpp"

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

	void saveImage( const QImage& img, const std::filesystem::path& dest )
	{
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

	std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id )
	{
		const QImage image { loadImage( path ) };
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

	namespace async
	{
		[[nodiscard]] QFuture< std::filesystem::path >
			importImage( const std::filesystem::path& path, const RecordID game_id )
		{
			return QtConcurrent::run( &( globalPools().image_importers ), &atlas::images::importImage, path, game_id );
		}

	} // namespace async
} // namespace atlas::images
