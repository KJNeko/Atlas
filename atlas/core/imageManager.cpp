//
// Created by kj16609 on 3/2/23.
//

#include "imageManager.hpp"

#include <fstream>

#include <QBuffer>
#include <QCryptographicHash>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QPixmap>
#include <QtConcurrentRun>

#include <tracy/TracyC.h>

#include "config.hpp"
#include "core/database/Transaction.hpp"
#include "pools.hpp"
#include "system.hpp"

namespace imageManager
{
	void cleanOrphans()
	{
		ZoneScoped;
		spdlog::debug( "Clearing orphan previews/banners" );
		//Grab all images from the database
		RapidTransaction transaction {};

		for ( const auto& path : std::filesystem::directory_iterator( config::paths::images::getPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, config::paths::images::getPath() ).string()
				>> [ & ]( [[maybe_unused]] int count ) noexcept { found = true; };

			if ( !found ) std::filesystem::remove( path );
		}
	}

	std::filesystem::path internalImportImage( const std::filesystem::path& path, const RecordID game_id )
	{
		ZoneScoped;
		if ( std::filesystem::exists( path ) )
		{
			QImage temp_image;
			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			//const bool load_success { temp_image.load( QString::fromStdString( path.string() ) ) };
			TracyCZoneEnd( tracy_ImageLoad );
			//if ( !load_success ) throw std::runtime_error( fmt::format( "Failed to import image {}", path ) );

			QFile file { path };
			file.open( QFile::ReadOnly );
			// Load image straight into byte array
			QByteArray image_byte_array { file.readAll() };
			//QBuffer buffer( &byteArray );

			//const bool tsave { temp_image.save( &buffer, path.extension().string().substr( 1 ).c_str(), 100 ) };

			/*
			if ( !tsave )
				throw std::runtime_error(
					fmt::format( "Failed to save image to buffer to test size! Banner path: {}", path ) );
			*/

			const auto hashData = []( const char* data_ptr, const int size ) -> QByteArray
			{
				ZoneScopedN( "Hash" );
				QCryptographicHash hash { QCryptographicHash::Sha256 };

				hash.addData( { data_ptr, size } );

				return hash.result();
			};

			std::string image_type { config::images::image_type::get().toStdString() };

			TracyCZoneN( tracy_SaveImage, "Image save to buffer as WEBP", true );

			QByteArray webp_byteArray;
			QBuffer webp_buffer( &webp_byteArray );
			temp_image.save( &webp_buffer, image_type.c_str(), 90 );

			TracyCZoneEnd( tracy_SaveImage );

			const auto dest_root { config::paths::images::getPath() / std::to_string( game_id ) };
			std::filesystem::create_directories( dest_root );

			//Which is smaller?
			if ( webp_buffer.size() < image_byte_array.size() )
			{
				//Buffer is smaller. Meaning webp is smaller. Use it
				const auto hash { hashData( webp_buffer.data(), static_cast< int >( webp_buffer.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + ".webp" ) };

				// We shouldn't need to make a new image since we already have it? (tmp_image)
				// const QImage img { QImage::fromData( webp_byteArray ) };
				temp_image.save( QString::fromStdString( dest.string() ) );

				return dest;
			}
			else
			{
				//Buffer is larger. Meaning webp is worse. Don't use it.
				const auto hash { hashData( image_byte_array.data(), static_cast< int >( image_byte_array.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + path.extension().string() ) };

				const QImage img { QImage::fromData( image_byte_array ) };
				img.save( QString::fromStdString( dest.string() ) );

				return dest;
			}
		}
		else
		{
			atlas::logging::userwarn( fmt::format(
				"importImage failed. Attempted to open file {} which doesn't exist anymore. Wrong permissions?",
				path ) );
			throw std::runtime_error( fmt::format( "Filepath {} does not exist. Unable to add as image", path ) );
		}
	}

	QFuture< std::filesystem::path > importImage( const std::filesystem::path& path, const RecordID game_id )
	{
		return QtConcurrent::run( &( globalPools().image_importers ), &internalImportImage, path, game_id );
	}

} // namespace imageManager
