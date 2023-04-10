//
// Created by kj16609 on 4/8/23.
//

#include <filesystem>

#include <gtest/gtest.h>

#include "atlas/utils/FileScanner.hpp"

TEST( EngineDetection, test )
{
	FileScanner scanner { "/home/kj16609/Desktop/BLACK★ACADEMY.v0.1.161/" };

	std::size_t counter { 0 };
	for ( const auto& file : scanner )
	{
		std::cout << "First: " << file.path.string() << ", depth: " << (int)file.depth << std::endl;
		++counter;
		if ( counter > 8 ) break;
	}

	for ( const auto& file : scanner )
	{
		std::cout << "Filename second: " << file.path.string() << ", depth: " << (int)file.depth << std::endl;
		++counter;
	}

	for ( const auto& file : scanner )
	{
		std::cout << "Filename third: " << file.path.string() << ", depth: " << (int)file.depth << std::endl;
		++counter;
	}

	std::cout << "Finished" << std::endl;
}
