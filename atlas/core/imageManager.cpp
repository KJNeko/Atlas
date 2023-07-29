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
		//spdlog::debug( path );
		ZoneScoped;
		if ( std::filesystem::exists( path ) )
		{
			//Load file so we have direct access to the bytearray
			QFile file( path );
			if ( !file.open( QFile::ReadOnly ) )
				throw std::runtime_error( fmt::format( "Failed to load image from file: {}", path ) );
			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			const QByteArray byteArray { file.readAll() };
			TracyCZoneEnd( tracy_ImageLoad );
			file.close();

			//Store image inside of QImage
			QImage temp_image;
			const bool load_success { temp_image.loadFromData( byteArray ) };
			if ( !load_success )
				throw std::runtime_error( fmt::format( "Failed to store image from byte array: {}", path.filename() ) );

			const std::string ext { path.extension().string().substr( 1 ) };
			const auto dest_root { config::paths::images::getPath() / std::to_string( game_id ) };
			std::filesystem::create_directories( dest_root );

			const auto hashData = []( const char* data_ptr, const int size ) -> QByteArray
			{
				ZoneScopedN( "Hash" );
				QCryptographicHash hash { QCryptographicHash::Sha256 };

				hash.addData( { data_ptr, size } );

				return hash.result();
			};

			//If GIF then store, do not convert
			if ( ext == "gif" )
			{
				const auto hash { hashData( byteArray, static_cast< int >( byteArray.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + path.extension().string() ) };

				//Qt is stupid and will not save gifs...  so we have to copy it
				const bool file_copied { std::filesystem::copy_file( path, dest ) };
				if ( !file_copied )
					throw std::
						runtime_error( fmt::format( "Unable to save gif to images folder: {}", path.filename() ) );

				return dest;
			}
			else
			{
				const std::string image_type { config::images::image_type::get().toStdString() };

				TracyCZoneN( tracy_SaveImage, "Image save to buffer as WEBP", true );
				QByteArray webp_byteArray;
				QBuffer webp_buffer( &webp_byteArray );
				temp_image.save( &webp_buffer, image_type.c_str(), 90 );
				TracyCZoneEnd( tracy_SaveImage );

				//Which is smaller?
				if ( webp_buffer.size() < byteArray.size() )
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
					const auto hash { hashData( byteArray.data(), static_cast< int >( byteArray.size() ) ) };
					const auto dest { dest_root / ( hash.toHex().toStdString() + path.extension().string() ) };

					const QImage img { QImage::fromData( byteArray ) };
					img.save( QString::fromStdString( dest.string() ) );

					return dest;
				}
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
