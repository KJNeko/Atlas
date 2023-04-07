//
// Created by kj16609 on 3/2/23.
//

#include "imageManager.hpp"

#include <fstream>

#include <QCryptographicHash>
#include <QFile>
#include <QPixmap>

#include <tracy/Tracy.hpp>

#include "config.hpp"
#include "h95/database/Database.hpp"
#include "system.hpp"

namespace imageManager
{
	void cleanOrphans()
	{
		ZoneScoped;
		spdlog::info( "Clearing orphan previews/banners" );
		//Grab all images from the database
		Transaction transaction { Transaction::Autocommit };

		for ( const auto& path : std::filesystem::directory_iterator( config::paths::images::getPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, config::paths::images::getPath() ).string()
				>> [ & ]( [[maybe_unused]] int count ) { found = true; };

			if ( !found ) std::filesystem::remove( path );
		}
	}

	std::filesystem::path importImage( const std::filesystem::path& path )
	{
		spdlog::info("Attemping To Load Image");
		ZoneScoped;
		if ( std::filesystem::exists( path ) )
		{
			QString qstr = QString::fromStdString( path.string() );
			QImage temp_image;
			temp_image.load(qstr);
			if(temp_image.load(qstr) == true){spdlog::info("Image Loaded Sucessfully");}

			//Save the image to a temp file depending on os
			const std::filesystem::path temp_path { std::filesystem::temp_directory_path() / "h95" / "temp.webp" };
			spdlog::info("Output Path:{}", temp_path.string());
			if ( !std::filesystem::exists( temp_path.parent_path() ) )
			{
				create_directories( temp_path.parent_path() );
			}

			if constexpr(sys::is_windows)
			{
				temp_image.save( QString::fromStdString( temp_path.string() ), "png", 100 );
			}
			else{
				temp_image.save( QString::fromStdString( temp_path.string() ), "webp", 100 );
			}

			if ( std::ifstream ifs( temp_path ); ifs )
			{
				std::vector< char > data;
				data.resize( std::filesystem::file_size( temp_path ) );
				ifs.read( data.data(), static_cast< long >( data.size() ) );

				QCryptographicHash hash { QCryptographicHash::Sha256 };
				hash.addData( { reinterpret_cast< const char* >( data.data() ),
				                static_cast< qsizetype >( data.size() ) } );

				const std::filesystem::path dest {
					( config::paths::images::getPath() / hash.result().toHex().toStdString() ).string() + ".webp"
				};

				if ( !std::filesystem::exists( dest.parent_path() ) )
				{
					if ( !std::filesystem::create_directories( dest.parent_path() ) )
						throw std::runtime_error( fmt::format( "Failed to create directory {}", dest.parent_path() ) );
				}

				//Copy the temp_file to the destination
				if ( !std::filesystem::exists( dest ) )
					std::filesystem::copy( temp_path, dest );
				else
					return dest;

				if ( !std::filesystem::exists( dest ) ) throw std::runtime_error( "Save failed!" );

				return dest;
			}
			else
				spdlog::warn( "Failed to open converted webp: Temp:{:e}, Input:{:e}", temp_path, path );
		}
		else
			spdlog::warn( "Failed to set open path for new banner at {:ce}", path );

		return { ":/invalid.jpg" };
	}

} // namespace imageManager
