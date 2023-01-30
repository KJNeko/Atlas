//
// Created by kj16609 on 1/29/23.
//

#include "h95/Importer.hpp"

#include <gtest/gtest.h>
#include <QApplication>
#include <spdlog/spdlog.h>

#include "h95/database/database.hpp"
#include "h95/Importer.hpp"


class ImportTest : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication app { argc, argv };

	inline static bool failed_last { false };

	void SetUp() override
	{
		if ( failed_last ) GTEST_SKIP() << "Skipping due to failed test previously";

		spdlog::set_level( spdlog::level::debug );

		database::initalize();
	}

	void TearDown() override
	{
		if ( this->HasFailure() || this->IsSkipped() ) failed_last = true;

		std::filesystem::remove( "./data/config.ini" );
		std::filesystem::remove( "./data/hydrus95.db" );
	}
};


TEST_F( ImportTest, GameImport )
{
	const std::filesystem::path testing_game { "/run/media/kj16609/WINHDD/NSFW/Miss Neko 3" };

	Importer importer;
	importer.setSource( testing_game );
	importer.setDestination( "./data/games/Miss Neko 3" );
	importer.import_game( "Miss Neko 3", "Someone", "", "IDK", false );

	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3" ), true );

	std::filesystem::remove_all( "./data/games/Miss Neko 3" );
}

TEST_F( ImportTest, GameImportBanner )
{
	const std::filesystem::path testing_game { "/run/media/kj16609/WINHDD/NSFW/Miss Neko 3" };

	Importer importer;
	importer.setSource( testing_game );
	importer.setBanner( testing_game / "banner.jpeg" );
	importer.setDestination( "./data/games/Miss Neko 3" );
	importer.import_game( "Miss Neko 3", "Someone", "", "IDK", false );

	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3" ), true );
	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3/h95/banner.jpeg" ), true );
	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3/banner.jpeg" ), false );

	std::filesystem::remove_all( "./data/games/Miss Neko 3" );
}

TEST_F( ImportTest, GameImportFull )
{
	const std::filesystem::path root { "/run/media/kj16609/WINHDD/NSFW/Miss Neko 3" };

	const std::vector< std::filesystem::path > previews {
		{ root / "previews" / "Miss_Neko_3_20_12_2022_12_00_04.png" },
		{ root / "previews" / "Miss_Neko_3_20_12_2022_12_02_53.png" },
		{ root / "previews" / "Miss_Neko_3_20_12_2022_12_03_19.png" },
		{ root / "previews" / "Miss_Neko_3_20_12_2022_12_03_48.png" },
		{ root / "previews" / "ss_1c15bb0331b5b5f173e1224ea18d48d481be4380.1920x1080.jpg" },
		{ root / "previews" / "ss_5b6ff3961d80c320fe2dd13551e2a17487dfff94.1920x1080.jpg" } };

	const std::filesystem::path testing_game { "/run/media/kj16609/WINHDD/NSFW/Miss Neko 3" };

	Importer importer;
	importer.setSource( testing_game );
	importer.setBanner( testing_game / "banner.jpeg" );
	importer.setPreviews(previews);
	importer.setDestination( "./data/games/Miss Neko 3" );
	importer.import_game( "Miss Neko 3", "Someone", "", "IDK", false );

	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3" ), true );
	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3/h95/banner.jpeg" ), true );
	ASSERT_EQ( std::filesystem::exists( "./data/games/Miss Neko 3/banner.jpeg" ), false );

	const std::filesystem::path dest_root {"./data/games/Miss Neko 3/h95/previews"};

	ASSERT_EQ(std::filesystem::exists(dest_root / "Miss_Neko_3_20_12_2022_12_00_04.png"), true);
	ASSERT_EQ(std::filesystem::exists(dest_root / "Miss_Neko_3_20_12_2022_12_02_53.png"), true);
	ASSERT_EQ(std::filesystem::exists(dest_root / "Miss_Neko_3_20_12_2022_12_03_19.png"), true);
	ASSERT_EQ(std::filesystem::exists(dest_root / "Miss_Neko_3_20_12_2022_12_03_48.png"), true);
	ASSERT_EQ(std::filesystem::exists(dest_root / "ss_1c15bb0331b5b5f173e1224ea18d48d481be4380.1920x1080.jpg"), true);
	ASSERT_EQ(std::filesystem::exists(dest_root / "ss_5b6ff3961d80c320fe2dd13551e2a17487dfff94.1920x1080.jpg"), true);

	const std::filesystem::path dest {"./data/games/Miss Neko 3/h95/previews"};

	std::filesystem::remove_all( "./data/games/Miss Neko 3" );
}


