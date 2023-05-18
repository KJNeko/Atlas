//
// Created by kj16609 on 5/17/23.
//

#include <catch2/catch_test_macros.hpp>

#include "core/imageManager.hpp"

TEST_CASE( "ImageManager", "[images]" )
{
	const auto image { QImage( 100, 100, QImage::Format::Format_RGB32 ) };
	image.save( "test.png" );

	const auto output { imageManager::importImage( "test.png" ) };

	REQUIRE( std::filesystem::exists( output ) );

	const auto image_comp { QImage( QString::fromStdString( output.string() ) ) };
	REQUIRE( image_comp == image );
}