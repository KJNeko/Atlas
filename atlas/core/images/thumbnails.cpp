//
// Created by kj16609 on 9/22/23.
//

#include "thumbnails.hpp"

#include "core/exceptions.hpp"
#include "loader.hpp"

namespace atlas::images
{

	std::filesystem::path createThumbnail( const std::filesystem::path& image_path )
	{
		atlas::logging::debug( "Creating thumbnail for {}", image_path );
		if ( !std::filesystem::exists( image_path ) )
			throw AtlasException( format_ns::format( "Invalid path: {} does not exist", image_path ) );

		//Create thumbnail
		const auto dest { thumbnailPath( image_path ) };
		QPixmap thumb { async::loadPixmap( image_path ).result() };

		thumb = thumb.scaled(
			config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get(), Qt::IgnoreAspectRatio );

		if ( !thumb.save( QString::fromStdString( dest.string() ), "webp", 95 ) )
			throw AtlasException( format_ns::format( "Failed to save thumbnail for {} to {}", image_path, dest ) );

		return dest;
	}

	std::filesystem::path thumbnailPath( const std::filesystem::path& image )
	{
		return image.parent_path() / ( image.stem().string() + ".thumb" );
	}

	QPixmap thumbnail( const std::filesystem::path& path )
	{
		logging::debug( "Gathering thumbnail for {}", path );
		if ( path.extension() == ".thumb" )
			throw AtlasException(
				"Invalid path provided to thumbnail. Must not provide the thumbnail path. Must provide the real image path" );

		if ( !std::filesystem::exists( path ) )
			throw AtlasException( format_ns::
			                          format( "Tried to provide a thumbnail for a non existent image: {}", path ) );

		const auto thumb_path { thumbnailPath( path ) };

		if ( !std::filesystem::exists( path ) ) (void)createThumbnail( path );

		QPixmap image;
		image.load( QString::fromStdWString( thumb_path.wstring() ) );
		return image;
	}

} // namespace atlas::images
