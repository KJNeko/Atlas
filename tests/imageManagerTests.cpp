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
#include <iostream>

#include <QImage>

#include "core/imageManager.hpp"

TEST_CASE( "Image import test", "[import]" )
{
	QImage image { 255, 255, QImage::Format_RGB32 };
	image.fill( QColor( 0, 0, 0 ) );
	std::filesystem::create_directory( "./test_files" );
	REQUIRE( image.save( "./test_files/test_image.png" ) );
	REQUIRE( std::filesystem::exists( "./test_files/test_image.png" ) );

	//try import
	const auto new_file { imageManager::importImage( "./test_files/test_image.png" ) };
	REQUIRE(
		std::filesystem::relative( new_file, std::filesystem::current_path() )
		== "data/images/38ac6748b793504c81fb58db37c6b61abd53ca0dd9d65e1b598bdc33a7db6d4d.webp" );
	REQUIRE( std::filesystem::exists( new_file ) );
	REQUIRE( std::filesystem::
	             exists( "./data/images/38ac6748b793504c81fb58db37c6b61abd53ca0dd9d65e1b598bdc33a7db6d4d.webp" ) );
	QImage new_image { QString::fromStdString( new_file.string() ) };

	REQUIRE( image == new_image );

	std::filesystem::remove( new_file );
	std::filesystem::remove( "./test_files/test_image.png" );
}

TEST_CASE( "Import import test - Special characters", "[import]" )
{
	//Fucking windows
	QImage image { 255, 255, QImage::Format_RGB32 };
	image.fill( QColor( 0, 0, 0 ) );
	std::filesystem::create_directory( "./test_files" );
	REQUIRE( image.save( "./test_files/ファックウィンドウ.png" ) );
	REQUIRE( std::filesystem::exists( "./test_files/ファックウィンドウ.png" ) );

	//try import
	const auto new_file { imageManager::importImage( "./test_files/ファックウィンドウ.png" ) };
	REQUIRE(
		std::filesystem::relative( new_file, std::filesystem::current_path() )
		== "data/images/38ac6748b793504c81fb58db37c6b61abd53ca0dd9d65e1b598bdc33a7db6d4d.webp" );
	REQUIRE( std::filesystem::exists( new_file ) );
	REQUIRE( std::filesystem::
	             exists( "./data/images/38ac6748b793504c81fb58db37c6b61abd53ca0dd9d65e1b598bdc33a7db6d4d.webp" ) );

	QImage new_image { QString::fromStdString( new_file.string() ) };

	REQUIRE( image == new_image );

	std::filesystem::remove( new_file );
	std::filesystem::remove( "./test_files/ファックウィンドウ.png" );
}