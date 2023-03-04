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

TEST_F( TestImageManager, importBanner )
{
	imageManager::importBanner( "./assets/banner/placeholder.jpg" );

	GTEST_ASSERT_TRUE( std::filesystem::exists(
		"./data/banners/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.jpg" ) );
}


TEST_F( TestImageManager, importPreview )
{
	imageManager::importPreview( "./assets/banner/placeholder.jpg" );

	GTEST_ASSERT_TRUE( std::filesystem::exists(
		"./data/previews/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.jpg" ) );
}

TEST_F( TestImageManager, importNonExistant )
{
	EXPECT_THROW( imageManager::importBanner( "./my/fake/image.jpg" ), std::runtime_error );
}

TEST_F( TestImageManager, fetchImage )
{
	QPixmap pixmap { "./assets/banner/placeholder.jpg" };
	QPixmap pixmap_comp { imageManager::fetchImage( imageManager::importPreview( "./assets/banner/placeholder.jpg" ) ) };
	GTEST_ASSERT_EQ( pixmap.size(), pixmap_comp.size() );
}

TEST_F( TestImageManager, clearOrhpans )
{
	imageManager::cleanOrphans();

	GTEST_ASSERT_FALSE( std::filesystem::exists(
		"./data/previews/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.jpg" ) );
	GTEST_ASSERT_FALSE( std::filesystem::exists(
		"./data/banners/de4fb797c8dabce6c9ee87e7e93d3cc5393e5ff4afe6c85634117cb2128feba7.jpg" ) );
}
