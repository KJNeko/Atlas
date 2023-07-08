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

#include <tracy/TracyC.h>

#include "config.hpp"
#include "core/database/Transaction.hpp"
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

	std::filesystem::path importImage( const std::filesystem::path& path )
	{
		ZoneScoped;
		//VERIFY THIS IS NEEDED
		if ( std::ifstream ifs( path ); ifs )
		{
			QImage temp_image;
			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			const bool load_success { temp_image.load( QString::fromStdString( path.string() ) ) };
			TracyCZoneEnd( tracy_ImageLoad );
			if ( !load_success ) throw std::runtime_error( fmt::format( "Failed to import image {}", path ) );

			QByteArray byteArray;
			QBuffer buffer( &byteArray );
			const bool tsave { temp_image.save( &buffer, path.extension().string().substr( 1 ).c_str(), 100 ) };
			spdlog::debug( tsave );

			spdlog::debug( "Image Loaded: {}", load_success );

			TracyCZoneN( tracy_ProcessImage, "Process image", true );
			TracyCZoneEnd( tracy_ProcessImage );

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
			temp_image.save( &webp_buffer, image_type.c_str(), 99 );

			TracyCZoneEnd( tracy_SaveImage );

			const auto dest_root { config::paths::images::getPath() };

			//Which is smaller?
			spdlog::debug( "webp:{} <-> original:{}", webp_buffer.size(), buffer.size() );

			if ( webp_buffer.size() < buffer.size() )
			{
				//Buffer is smaller. Meaning webp is smaller. Use it
				const auto hash { hashData( webp_buffer.data(), static_cast< int >( webp_buffer.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + ".webp" ) };

				QImage img = QImage::fromData( webp_byteArray );
				img.save( QString::fromStdString( dest.string() ) );

				return dest;
			}
			else
			{
				//Buffer is larger. Meaning webp is worse. Don't use it.
				const auto hash { hashData( buffer.data(), static_cast< int >( buffer.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + path.extension().string() ) };

				QImage img = QImage::fromData( byteArray );
				img.save( QString::fromStdString( dest.string() ) );

				return dest;
			}
		}
		else
		{
			spdlog::warn( "Filepath {} does not exist. Unable to import image", path );
			throw std::runtime_error( fmt::format( "Filepath {} does not exist. Unable to add as image", path ) );
		}
	}

} // namespace imageManager
