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
		if ( std::ifstream ifs( path ); ifs )
		{
			std::vector< char > data {};
			data.resize( std::filesystem::file_size( path ) );
			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			ifs.read( data.data(), static_cast< int >( data.size() ) );
			TracyCZoneEnd( tracy_ImageLoad );

			TracyCZoneN( tracy_ProcessImage, "Process image", true );
			QImage temp_image {
				QImage::fromData( reinterpret_cast< unsigned char* >( data.data() ), static_cast< int >( data.size() ) )
			};
			TracyCZoneEnd( tracy_ProcessImage );

			const auto hashData = []( const char* data_ptr, const int size ) -> QByteArray
			{
				ZoneScopedN( "Hash" );

				QCryptographicHash hash { QCryptographicHash::Sha256 };

				hash.addData( { data_ptr, size } );

				return hash.result();
			};

			std::string image_type { config::images::image_type::get().toStdString() };

			TracyCZoneN( tracy_SaveImage, "Image save to buffer", true );
			QByteArray byteArray;
			QBuffer buffer( &byteArray );
			temp_image.save( &buffer, image_type.c_str(), 99 );
			
			TracyCZoneEnd( tracy_SaveImage );

			const auto dest_root { config::paths::images::getPath() };

			//Which is smaller?
			if ( static_cast< std::uint64_t >( buffer.size() ) < data.size() )
			{
				//Buffer is smaller. Meaning webp is smaller. Use it
				const auto hash { hashData( buffer.data(), static_cast< int >( buffer.size() ) ) };
				const auto dest { dest_root / ( hash.toHex().toStdString() + ".webp" ) };

				//Copy buffer to file
				if ( std::filesystem::exists( dest ) ) return dest;
				if ( std::ofstream ofs( dest ); ofs )
				{
					ofs.write( buffer.data(), buffer.size() );
					return dest;
				}
				else
					throw std::runtime_error( fmt::format( "Failed to open desination {}", dest ) );
			}
			else
			{
				//Buffer is larger. Meaning webp is worse. Don't use it.
				const auto hash { hashData( data.data(), static_cast< int >( data.size() ) ) };

				const auto dest { dest_root / ( hash.toHex().toStdString() + path.extension().string() ) };

				if ( std::filesystem::exists( dest ) ) return dest;
				std::filesystem::copy( path, dest );
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
