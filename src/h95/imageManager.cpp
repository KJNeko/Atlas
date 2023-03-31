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

namespace imageManager
{
	std::filesystem::path getImagePath()
	{
		ZoneScoped;
		const auto path { config::paths::images::getPath() };

		if ( !std::filesystem::exists( path ) ) std::filesystem::create_directories( path );

		return std::filesystem::canonical( path );
	}

	void cleanOrphans()
	{
		ZoneScoped;
		//Grab all images from the database
		Transaction transaction;

		spdlog::debug( "Clearing orphans" );

		for ( const auto& path : std::filesystem::directory_iterator( getImagePath() ) )
		{
			if ( !path.is_regular_file() ) continue;
			spdlog::debug( "Testing if file {} exists in DB", path.path() );

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, getImagePath() ).string()
				>> [ & ]( [[maybe_unused]] int count ) { found = true; };

			if ( !found ) std::filesystem::remove( path );
		}
	}

	std::filesystem::path importImage( const std::filesystem::path& path )
	{
		ZoneScoped;
		spdlog::debug( "Importing image {}", path );
		if ( std::filesystem::exists( path ) )
		{
			QImage temp_image;
			temp_image.load( QString::fromStdString( path.string() ) );

			//Save the image to a temp file depending on os
			const std::filesystem::path temp_path { std::filesystem::temp_directory_path() / "h95" / "temp.webp" };
			if ( !std::filesystem::exists( temp_path.parent_path() ) ) create_directories( temp_path.parent_path() );

			temp_image.save( QString::fromStdString( temp_path.string() ), "WEBP", 99 );

			if ( std::ifstream ifs( temp_path ); ifs )
			{
				std::vector< char > data;
				data.resize( std::filesystem::file_size( temp_path ) );
				ifs.read( data.data(), static_cast< long >( data.size() ) );

				QCryptographicHash hash { QCryptographicHash::Sha256 };
				hash.addData( { reinterpret_cast< const char* >( data.data() ),
				                static_cast< qsizetype >( data.size() ) } );

				const std::filesystem::path dest { ( getImagePath() / hash.result().toHex().toStdString() ).string()
					                               + ".webp" };

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
			spdlog::warn( "Failed to open converted webp: Temp:{}, Input:{}", temp_path, path );
		}

		return { ":/invalid.jpg" };
	}

} // namespace imageManager
