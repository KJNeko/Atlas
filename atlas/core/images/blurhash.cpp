//
// Created by kj16609 on 9/26/23.
//

#include "core/images/blurhash.hpp"

#include <blurhash-cxx.hpp>

#include "core/database/RapidTransaction.hpp"
#include "core/utils/ImageCache/ImageCache.hpp"
#include "loader.hpp"

inline static atlas::cache::ImageCache blurhash_cache;

namespace atlas::images
{

	std::string createBlurhash( const QPixmap& pixmap )
	{
		return createBlurhash( pixmap.toImage() );
	}

	std::string createBlurhash( QImage image )
	{
		image = image.convertToFormat( QImage::Format_RGB888 );

		const int channels { 3 };

		std::vector< std::uint8_t > data;
		data.resize( static_cast< unsigned long >( image.width() * image.height() * channels ) );

		for ( int y = 0; y < image.height(); ++y )
		{
			const int y_idx { y * image.width() * channels };
			for ( int x = 0; x < image.width(); ++x )
			{
				//Decode pixel to RGB
				const QColor pixel { image.pixel( x, y ) };
				const int x_idx { x * channels };
				const std::size_t idx { static_cast< std::size_t >( y_idx + x_idx ) };

				data[ idx + 0 ] = static_cast< std::uint8_t >( pixel.red() );
				data[ idx + 1 ] = static_cast< std::uint8_t >( pixel.green() );
				data[ idx + 2 ] = static_cast< std::uint8_t >( pixel.blue() );
			}
		}

		const auto blurhash_str { blurhash::encode<
			8,
			8 >( image.width(), image.height(), reinterpret_cast< const std::uint8_t* >( image.bits() ), channels ) };

		return blurhash_str;
	}

	std::string createBlurhash( const std::filesystem::path& path )
	{
		ZoneScoped;
		if ( !std::filesystem::exists( path ) ) return {};
		auto image { loadImage( path ) };
		return createBlurhash( image );
	}

	QPixmap decodeBlurhash( const std::string& hash, int width, int height )
	{
		ZoneScoped;
		const int channels { 3 };

		//std::vector< std::uint8_t > pixels_cpp;
		//pixels_cpp.resize( width * height * 3 );
		//decodeToArray( hash.data(), width, height, 0, channels, pixels_cpp.data() );

		const auto key { format_ns::format( "{}-{}x{}", hash, width, height ) };

		if ( auto image_opt = blurhash_cache.find( key ); image_opt.has_value() )
		{
			return image_opt.value();
		}
		else
		{
			auto pixels_cpp { blurhash::decode( hash, width, height, 0, channels ) };

			QImage image { width, height, QImage::Format::Format_RGB888 };
			for ( int y = 0; y < height; ++y )
			{
				const auto y_idx { y * width * channels };
				for ( int x = 0; x < width; ++x )
				{
					const auto x_idx { x * channels };
					const std::size_t idx { static_cast< size_t >( x_idx + y_idx ) };

					const auto color { qRgb( pixels_cpp[ idx + 0 ], pixels_cpp[ idx + 1 ], pixels_cpp[ idx + 2 ] ) };

					image.setPixel( x, y, color );
				}
			}

			const auto pixmap { QPixmap::fromImage( image ) };

			blurhash_cache.insert( key, pixmap );

			return pixmap;
		}
	}

	QPixmap getBlurhash( const std::filesystem::path& path, const QSize size )
	{
		ZoneScoped;
		//Check if the blurhash exists inside of the database
		std::string blur_hash;

		RapidTransaction() << "SELECT blurhash FROM image_blurhash WHERE image_sha256 = ?" << path.stem().u8string()
			>> blur_hash;

		if ( blur_hash.empty() )
		{
			//No blurhash found. Time to make one
			if ( !std::filesystem::exists( path ) )
				throw AtlasException( format_ns::
				                          format( "Expected path {} to make blurhash. Path does not exist", path ) );

			blur_hash = createBlurhash( path );

			RapidTransaction() << "INSERT INTO image_blurhash (image_sha256, blurhash) VALUES (?,?)"
							   << path.stem().u8string() << blur_hash;
		}

		return decodeBlurhash( blur_hash, size.width(), size.height() );
	}

} // namespace atlas::images
