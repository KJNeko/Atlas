//
// Created by kj16609 on 9/26/23.
//

#include "core/images/blurhash.hpp"

#include <tracy/Tracy.hpp>

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

				data[ idx + RED ] = static_cast< std::uint8_t >( pixel.red() );
				data[ idx + GREEN ] = static_cast< std::uint8_t >( pixel.green() );
				data[ idx + BLUE ] = static_cast< std::uint8_t >( pixel.blue() );
			}
		}

		const auto blurhash_str { blurhash::encode< 5, 5 >( image.width(), image.height(), data.data(), channels ) };

		return blurhash_str;
	}

	QPixmap decodeBlurhash( const std::string& hash, int width, int height )
	{
		ZoneScoped;
		constexpr int channels { 3 };

		std::size_t seed { 0 };
		seed ^= std::hash< std::string > {}( hash ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< unsigned long >( width ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );
		seed ^= std::hash< std::size_t > {}( static_cast< unsigned long >( height ) ) + 0x9e3779b9 + ( seed << 6 )
		      + ( seed >> 2 );

		const auto key { seed };

		if ( auto image_opt = blurhash_cache.find( key ); image_opt.has_value() )
		{
			return image_opt.value();
		}
		else
		{
			const QSize input_size { width, height };
			//Max 100x100 for performance and scale up later
			const QSize load_size { input_size.scaled( 128, 128, Qt::AspectRatioMode::KeepAspectRatio ) };
			QImage image { load_size, QImage::Format::Format_RGB888 };

			auto pixels_cpp { blurhash::decode( hash, image.width(), image.height(), 0, channels ) };

			for ( int y = 0; y < image.height(); ++y )
			{
				const auto y_idx { y * image.width() * channels };
				for ( int x = 0; x < image.width(); ++x )
				{
					const auto x_idx { x * channels };
					const std::size_t idx { static_cast< size_t >( x_idx + y_idx ) };

					const auto color {
						qRgb( pixels_cpp[ idx + RED ], pixels_cpp[ idx + GREEN ], pixels_cpp[ idx + BLUE ] )
					};

					image.setPixel( x, y, color );
				}
			}

			image = image.scaled( width, height, Qt::AspectRatioMode::IgnoreAspectRatio, Qt::SmoothTransformation );

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
		int image_height { 0 };
		int image_width { 0 };

		RapidTransaction() << "SELECT blurhash, image_height, image_width FROM image_blurhash WHERE image_sha256 = ?"
						   << path.stem().u8string()
			>> [ & ]( const std::string hash, int height, int width )
		{
			blur_hash = hash;
			image_height = height;
			image_width = width;
		};

		const QSize image_size { image_width, image_height };
		const QSize final_size { image_size.scaled( size, Qt::KeepAspectRatio ) };

		if ( blur_hash.empty() )
		{
			//No blurhash found. Time to make one
			if ( !std::filesystem::exists( path ) )
				throw AtlasException( format_ns::
				                          format( "Expected path {} to make blurhash. Path does not exist", path ) );

			const auto image { loadImage( path ) };
			blur_hash = createBlurhash( image );

			RapidTransaction()
				<< "INSERT INTO image_blurhash (image_sha256, blurhash, image_width, image_height) VALUES (?,?,?,?)"
				<< path.stem().u8string() << blur_hash << image.width() << image.height();
		}

		return decodeBlurhash( blur_hash, final_size.width(), final_size.height() );
	}

} // namespace atlas::images
