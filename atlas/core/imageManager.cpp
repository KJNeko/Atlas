//
// Created by kj16609 on 3/2/23.
//

#include "imageManager.hpp"

#include <fstream>

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
		if ( std::filesystem::exists( path ) )
		{
			const QString qstr { QString::fromStdString( path.string() ) };
			QImage temp_image;

			TracyCZoneN( tracy_ImageLoad, "Image load", true );
			if ( !temp_image.load( qstr ) )
			{
				spdlog::error( "Failed to load image {}", path.string() );
				TracyCZoneEnd( tracy_ImageLoad );
				return {};
			}
			TracyCZoneEnd( tracy_ImageLoad );

			const auto hash_file = []( const std::filesystem::path& hash_path ) -> QByteArray
			{
				ZoneScopedN( "Hash file" );
				if ( std::ifstream ifs( hash_path ); ifs )
				{
					std::array< std::byte, 4096 > buffer {};
					QCryptographicHash hash { QCryptographicHash::Sha256 };

					while ( !ifs.eof() && !ifs.fail() )
					{
						ifs.read( reinterpret_cast< char* >( buffer.data() ), static_cast< long >( buffer.size() ) );

						std::size_t read { static_cast< std::size_t >( ifs.gcount() ) };

						hash.addData( { reinterpret_cast< const char* >( buffer.data() ),
						                static_cast< qsizetype >( read ) } );
					}

					return hash.result();
				}
				else
					return {};
			};

			//Save the image to a temp file depending on os
			const std::filesystem::path temp_path {
				std::filesystem::temp_directory_path() / "atlas"
				/ fmt::format( "temp_{}.webp", hash_file( path ).toHex().toStdString() )
			};

			if ( !std::filesystem::exists( temp_path.parent_path() ) )
				std::filesystem::create_directories( temp_path.parent_path() );

			std::string image_type { config::images::image_type::get().toStdString() };

			TracyCZoneN( tracy_SaveImage, "Image save", true );
			temp_image.save( QString::fromStdString( temp_path.string() ), image_type.c_str(), 99 );
			TracyCZoneEnd( tracy_SaveImage );

			if ( !std::filesystem::exists( temp_path ) )
			{
				spdlog::warn(
					"Path did not exist during image import! Path: {:ce}\n Parent: {:ce}",
					temp_path,
					temp_path.parent_path() );
			}

			const auto dest_root { config::paths::images::getPath() };

			if ( std::filesystem::file_size( temp_path ) > std::filesystem::file_size( path ) )
			{
				//File is worse. So we keep the smaller one.
				std::filesystem::remove( temp_path );

				const auto image_hash { hash_file( path ) };

				const auto dest_path { dest_root / ( image_hash.toHex().toStdString() + path.extension().string() ) };

				TracyCZoneN( tracy_CopyImage, "Image copy (original)", true );
				if ( !std::filesystem::exists( dest_path ) ) std::filesystem::copy( path, dest_path );
				TracyCZoneEnd( tracy_CopyImage );

				if ( !std::filesystem::exists( dest_path ) )
				{
					spdlog::error( "Failed to copy {} to {}", path.string(), dest_path.string() );
					return {};
				}

				return std::filesystem::relative( dest_path, std::filesystem::current_path() );
			}
			else
			{
				//Keep the temp file
				const auto image_hash { hash_file( temp_path ) };

				const auto dest_path { dest_root / ( image_hash.toHex().toStdString() + ".webp" ) };

				TracyCZoneN( tracy_CopyImage, "Image copy (temp)", true );
				if ( !std::filesystem::exists( dest_path ) ) std::filesystem::copy( temp_path, dest_path );
				TracyCZoneEnd( tracy_CopyImage );

				std::filesystem::remove( temp_path );

				if ( !std::filesystem::exists( dest_path ) )
				{
					spdlog::error( "Failed to copy {} to {}", temp_path.string(), dest_path.string() );
					return {};
				}

				return std::filesystem::relative( dest_path, std::filesystem::current_path() );
			}
		}
		else
			spdlog::warn( "Failed to set open path for new banner at {:ce}", path );

		return { ":/invalid.jpg" };
	}

} // namespace imageManager
