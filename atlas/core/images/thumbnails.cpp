//
// Created by kj16609 on 9/22/23.
//

#include "thumbnails.hpp"

#include "core/exceptions.hpp"
#include "loader.hpp"

inline static atlas::cache::ImageCache thumb_cache;

namespace atlas::images
{
	std::filesystem::path createThumbnail( const std::filesystem::path& image_path )
	{
		atlas::logging::debug( "Creating thumbnail for {}", image_path );
		if ( !std::filesystem::exists( image_path ) )
			throw AtlasException( format_ns::format( "Invalid path: {} does not exist", image_path ) );

		QPixmap thumb { loadPixmap( image_path ) };

		thumb =
			thumb.scaled( config::images::thumbnail_x::get(), config::images::thumbnail_y::get(), Qt::KeepAspectRatio );

		const auto dest { thumbnailPath( image_path ) };
		if ( !thumb.save( QString::fromStdString( dest.string() ), "webp", 95 ) )
			throw AtlasException( format_ns::format( "Failed to save thumbnail for {} to {}", image_path, dest ) );

		return dest;
	}

	std::filesystem::path thumbnailPath( const std::filesystem::path& image )
	{
		if ( !std::filesystem::exists( image ) )
			throw AtlasException(
				format_ns::format( "attempted to get a thumbanil path for a non existant path: {}", image ) );
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

		if ( auto opt = thumb_cache.find( thumb_path.string() ); opt.has_value )
			return opt.value();
		else
		{
			logging::debug( "Thumbnail path should be: {}", thumb_path );

			if ( !std::filesystem::exists( thumb_path ) )
				(void)createThumbnail( path );
			else
				logging::debug( "Thumbnail already existed" );

			auto image { loadPixmap( thumb_path ) };

			thumb_cache.insert( thumb_path.string(), image );

			return image;
		}
	}

} // namespace atlas::images
