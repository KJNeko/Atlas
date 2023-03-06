//
// Created by kj16609 on 3/2/23.
//

#include <h95/database/Database.hpp>
#include <h95/imageManager.hpp>
#include <QPixmap>
#include <QApplication>

#include "GTestBox.hpp"

class TestImageManager : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication* app { nullptr };

	void SetUp() override
	{
		app = new QApplication( argc, argv );
		Database::initalize( "./data/testing.db" );
	}

	void TearDown() override
	{
		delete app;
		Database::deinit();
		std::filesystem::remove( "./data/testing.db" );
	}
};

TEST_F( TestImageManager, importPreview )
{
	const auto output {imageManager::importImage( "./assets/banner/placeholder.jpg" )};

	GTEST_ASSERT_TRUE(std::filesystem::canonical("./data/images/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.webp") == output);

	GTEST_ASSERT_TRUE( std::filesystem::exists(
		"./data/images/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.webp" ) );
}

TEST_F( TestImageManager, importNonExistant )
{
	GTEST_ASSERT_EQ( imageManager::importImage( "./my/fake/image.jpg" ), ":/invalid.jpg" );
}

TEST_F( TestImageManager, clearOrhpans )
{
	imageManager::cleanOrphans();

	GTEST_ASSERT_FALSE( std::filesystem::exists(
		"./data/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.webp" ) );
}
