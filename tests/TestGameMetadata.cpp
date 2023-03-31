//
// Created by kj16609 on 3/31/23.
//

#include "GTestBox.hpp"
#include "h95/config.hpp"
#include "h95/database/Database.hpp"
#include "h95/database/GameMetadata.hpp"

class TestGameMetadata : public ::testing::Test
{
	void SetUp() override { Database::initalize( "./data/testing.db" ); }

	void TearDown() override { std::filesystem::remove_all( "./data/" ); }
};

TEST_F( TestGameMetadata, getPath )
{
	GameMetadata metadata { 0, "1.0", "my/game/1.0", "executable.exe", true, 0, 0 };

	GTEST_ASSERT_EQ( config::paths::games::getPath().string(), "./data/games" );
	GTEST_ASSERT_EQ( metadata.getPath().string(), "./data/games/my/game/1.0" );
	GTEST_ASSERT_EQ( metadata.getExecPath().string(), "./data/games/my/game/1.0/executable.exe" );
}
