//
// Created by kj16609 on 7/3/23.
//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#pragma GCC diagnostic pop

#include <filesystem>

#include <QImage>

#include "core/imageManager.hpp"

TEST_CASE( "Image import test", "[import]" )
{
	QImage image { 3, 3, QImage::Format_RGB32 };
	std::filesystem::create_directory( "./test_files" );
	REQUIRE( image.save( "./test_files/test_image.png" ) );
	REQUIRE( std::filesystem::exists( "./test_files/test_image.png" ) );

	//try import
	const auto new_file { imageManager::importImage( "./test_files/test_image.png" ) };
	REQUIRE( std::filesystem::exists( new_file ) );
	std::filesystem::remove( new_file );
	std::filesystem::remove( "./test_files/test_image.png" );
}
