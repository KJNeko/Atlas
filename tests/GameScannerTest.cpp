//
// Created by kj16609 on 7/4/23.
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
#include <fstream>
#include <vector>

#include "core/utils/FileScanner.hpp"

TEST_CASE( "File Scanner", "[import]" )
{
	std::filesystem::remove_all( "./testing_files" );
	std::vector< std::filesystem::path > test_files {
		"./testing_files/games/some_creator/my_game/game.exe",
		"./testing_files/games/some_creator/my_game/dummy.dummy",
		"./testing_files/games/some_creator/second_game/game.exe",
		"./testing_files/games/some_creator/second_game/dummy.dummy",
		"./testing_files/games/some_creator/second_game/empty_folder/",
		"./testing_files/games/some_creator/second_game/empty_folder_r/empty_folder_again/",
		"./testing_files/games/some_creator/second_game/folder_r/folder_again/dummy.dummy",
	};

	for ( const auto& file : test_files )
	{
		std::filesystem::create_directories( file.parent_path() );

		if ( file.extension() == ".exe" )
		{
			//Copy our exe as a dummy
			std::filesystem::copy( "./AtlasTests", file );
		}
		else if ( file.extension() == ".dummy" )
		{
			//Fill with rand data
			if ( std::ofstream ofs( file ); ofs )
			{
				std::array< char, 4096 > data; //Budget rand
				ofs.write( data.data(), data.size() );
			}
		}
		else if ( file.string().ends_with( '/' ) )
		{
			//Empty folder
			std::filesystem::create_directory( file );
		}
	}

	std::vector< std::filesystem::path > expected {
		"games",
		"games/some_creator",
		"games/some_creator/my_game",
		"games/some_creator/second_game",
		"games/some_creator/my_game/game.exe",
		"games/some_creator/my_game/dummy.dummy",
		"games/some_creator/second_game/game.exe",
		"games/some_creator/second_game/dummy.dummy",
		"games/some_creator/second_game/folder_r",
		"games/some_creator/second_game/empty_folder",
		"games/some_creator/second_game/empty_folder_r",
		"games/some_creator/second_game/folder_r/folder_again",
		"games/some_creator/second_game/empty_folder_r/empty_folder_again",
		"games/some_creator/second_game/folder_r/folder_again/dummy.dummy"
	};

	FileScanner scanner { "./testing_files" };

	std::vector< std::filesystem::path > output;

	for ( const auto& file : scanner ) output.emplace_back( file.relative );

	REQUIRE( expected == output );

	std::filesystem::remove_all( "./testing_files" );
}
