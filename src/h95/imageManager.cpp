//
// Created by kj16609 on 3/2/23.
//

#include <QSize>
#include <QPixmap>
#include <QPixmapCache>
#include <QCryptographicHash>

#include <fstream>
#include <QFile>

#include "imageManager.hpp"
#include "h95/database/Database.hpp"
#include "config.hpp"

enum ImageType : uint8_t
{
	IMAGE_BANNER = 1,
	IMAGE_PREVIEW = 2,
};

namespace imageManager
{
	std::filesystem::path getBannerPath()
	{
		return getSettings< QString >( "images/banner_path", "./data/banners" ).toStdString();
	}

	std::filesystem::path getPreviewPath()
	{
		return getSettings< QString >( "images/preview_path", "./data/previews" ).toStdString();
	}


	void cleanOrphans()
	{
		//Grab all images from the database
		Transaction transaction;

		for ( const auto& path : std::filesystem::directory_iterator( getBannerPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction.ref() << "SELECT type FROM images WHERE path = ?"
							  << std::filesystem::relative( path, getBannerPath() ).string()
				>> [&]( [[maybe_unused]] const uint8_t type )
			{
				found = true;
			};

			if ( !found ) std::filesystem::remove( path );
		}

		for ( const auto& path : std::filesystem::directory_iterator( getPreviewPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction.ref() << "SELECT type FROM images WHERE path = ?"
							  << std::filesystem::relative( path, getPreviewPath() ).string()
				>> [&]( [[maybe_unused]] const uint8_t type )
			{
				found = true;
			};

			if ( !found ) std::filesystem::remove( path );
		}
	}


	std::filesystem::path
		importImage( const std::filesystem::path& path, const std::filesystem::path& dest, bool delete_after = false )
	{
		auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

		if ( !std::filesystem::exists( path ) ) throw std::runtime_error( "Path does not exist" );

		QFile file { path };
		file.open( QFile::OpenModeFlag::ReadOnly );
		const auto data { file.readAll() };
		hasher.addData( data );

		const auto dest_path { dest / ( hasher.result().toHex( 0 ).toStdString() + path.extension().string() ) };

		file.copy( dest_path );
		if ( delete_after ) std::filesystem::remove( path );

		return dest_path;
	}

	std::filesystem::path importBanner( const std::filesystem::path& path, bool delete_after )
	{
		const auto banner_path { getBannerPath() };
		std::filesystem::create_directories( banner_path );

		return importImage( path, banner_path, delete_after );
	}

	std::filesystem::path importPreview( const std::filesystem::path& path, bool delete_after )
	{
		const auto preview_path { getPreviewPath() };
		std::filesystem::create_directories( preview_path );

		return importImage( path, preview_path, delete_after );
	}

	QPixmap fetchImage( const std::filesystem::path& path, const QSize size )
	{
		const auto key { QString::fromStdString(
			path.string() + std::to_string( size.width() ) + std::to_string( size.height() ) ) };

		QPixmap pixmap;
		if ( QPixmapCache::find( key, &pixmap ) ) { return pixmap; }
		else
		{
			pixmap.load( QString::fromStdString( path.string() ) );
			if ( !size.isEmpty() ) pixmap.scaled( size, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation );

			QPixmapCache::insert( key, pixmap );
			return pixmap;
		}
	}
}  // namespace imageManager
