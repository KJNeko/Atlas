//
// Created by kj16609 on 3/2/23.
//

#include <QApplication>
#include <QPixmap>

#include <gtest/gtest.h>

#include "atlas/core/database/Database.hpp"
#include "atlas/core/imageManager.hpp"
#include "atlas/core/logging.hpp"

#ifdef ATLAS_TEST_UI
class TestImageManager : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication* app { nullptr };

	void SetUp() override
	{
		app = new QApplication( argc, argv );
		Database::initalize( ":memory:" );
	}

	void TearDown() override
	{
		delete app;
		Database::deinit();
		std::filesystem::remove_all( "./data/" );
	}
};

TEST_F( TestImageManager, importPreview )
{
	QImage image { ":/images/assets/search.svg" };
	std::filesystem::create_directories( "./assets/banner" );
	image.save( "./assets/banner/placeholder.jpg" );

	GTEST_ASSERT_TRUE( std::filesystem::exists( "./assets/banner/placeholder.jpg" ) );

	const auto output { imageManager::importImage( "./assets/banner/placeholder.jpg" ) };

	GTEST_ASSERT_TRUE( std::filesystem::exists( output ) );
}

TEST_F( TestImageManager, importNonExistant )
{
	GTEST_ASSERT_EQ( imageManager::importImage( "./my/fake/image.jpg" ), ":/invalid.jpg" );
}

TEST_F( TestImageManager, clearOrhpans )
{
	QImage image { ":/images/assets/search.svg" };
	std::filesystem::create_directories( "./assets/banner" );
	image.save( "./data/images/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" );

	imageManager::cleanOrphans();

	GTEST_ASSERT_FALSE( std::filesystem::exists(
		"./images/data/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" ) );
}
#endif
