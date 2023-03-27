//
// Created by kj16609 on 3/2/23.
//

#include <QApplication>
#include <QPixmap>

#include "GTestBox.hpp"
#include "h95/database/Database.hpp"
#include "h95/imageManager.hpp"
#include "h95/logging.hpp"

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
		std::filesystem::remove_all( "./data/" );
	}
};

TEST_F( TestImageManager, importPreview )
{
	QImage image { ":/images/assets/search.png" };
	std::filesystem::create_directories("./assets/banner");
	image.save( "./assets/banner/placeholder.jpg" );

	GTEST_ASSERT_TRUE(std::filesystem::exists("./assets/banner/placeholder.jpg"));

	const auto output { imageManager::importImage( "./assets/banner/placeholder.jpg" ) };

	spdlog::info("{}", output);

	GTEST_ASSERT_TRUE( std::filesystem::exists(
		"./data/images/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" ) );

	GTEST_ASSERT_TRUE(
		std::filesystem::
		canonical( "./data/images/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" )
		== output );
}

TEST_F( TestImageManager, importNonExistant )
{
	GTEST_ASSERT_EQ( imageManager::importImage( "./my/fake/image.jpg" ), ":/invalid.jpg" );
}

TEST_F( TestImageManager, clearOrhpans )
{
	QImage image { ":/images/assets/search.png" };
	std::filesystem::create_directories("./assets/banner");
	image.save( "./data/images/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" );

	imageManager::cleanOrphans();

	GTEST_ASSERT_FALSE( std::filesystem::
	                        exists( "./images/data/271948cc9461f20a5e77218948b22a790afdd6a9fce6f2dc295decfe4aa96536.webp" ) );
}
