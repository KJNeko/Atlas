//
// Created by kj16609 on 3/2/23.
//

#include <fstream>

#include <QPixmap>
#include <QCryptographicHash>
#include <QFile>


#include "imageManager.hpp"
#include "h95/database/Database.hpp"
#include "config.hpp"

namespace imageManager
{
	std::filesystem::path getImagePath()
	{
		ZoneScoped;
		const auto path {
			std::filesystem::path( getSettings< QString >( "paths/h95_data", "./data" ).toStdString() ) / "images" };

		if ( !std::filesystem::exists( path ) ) std::filesystem::create_directories( path );

		return std::filesystem::canonical( path );
	}


	void cleanOrphans()
	{
		ZoneScoped;
		//Grab all images from the database
		Transaction transaction;

		for ( const auto& path : std::filesystem::directory_iterator( getImagePath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, getImagePath() ).string()
				>> [&]( [[maybe_unused]] int count )
			{
				found = true;
			};

			if ( !found ) std::filesystem::remove( path );
		}
	}

	std::filesystem::path importImage( const std::filesystem::path& path, bool delete_after )
	{
		ZoneScoped;
		if ( std::filesystem::exists( path ) )
		{
			if ( std::ifstream ifs( path ); ifs )
			{
				ZoneScopedN("ifs");
				std::vector< char > data;
				data.resize( std::filesystem::file_size( path ) );
				ifs.read( data.data(), static_cast< long >( data.size() ) );

				QImage image;
				image.loadFromData(
					reinterpret_cast< const unsigned char* >( data.data() ),
					static_cast< int >( data.size() ) );

				QCryptographicHash hash { QCryptographicHash::Sha256 };
				hash.addData(
					{ reinterpret_cast< const char* >( data.data() ), static_cast< qsizetype >( data.size() ) } );

				const std::filesystem::path dest {
					( getImagePath() / hash.result().toHex().toStdString() ).string() + ".webp" };

				//Save as webp
				image.save( QString::fromStdString( dest.string() ) );

				if ( delete_after ) std::filesystem::remove( path );

				return dest;
			}
		}

		return { ":/invalid.jpg" };
	}

}  // namespace imageManager
