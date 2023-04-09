//
// Created by kj16609 on 3/31/23.
//

#include <gtest/gtest.h>

#include "h95/config.hpp"
#include "h95/database/Database.hpp"
#include "h95/database/GameMetadata.hpp"
#include "h95/database/Record.hpp"

class TestGameMetadata : public ::testing::Test
{
	void SetUp() override { Database::initalize( ":memory:" ); }

	void TearDown() override { Database::deinit(); }
};

TEST_F( TestGameMetadata, getPath )
{
	Record record { "TestTitle", "", "" };
	record->addVersion( "1.0", "my/game/1.0", "executable.exe", false, 0 );
	GameMetadata& metadata { record->getLatestVersion() };

	std::filesystem::create_directories( "./data/games" );
	std::filesystem::create_directories( "./data/games/my/game/1.0" );

	GTEST_ASSERT_EQ( config::paths::games::getPath().string(), std::filesystem::canonical( "./data/games" ).string() );
	GTEST_ASSERT_EQ( metadata.getPath().string(), std::filesystem::canonical( "./data/games/my/game/1.0" ) );
	GTEST_ASSERT_EQ(
		metadata.getExecPath().string(), std::filesystem::current_path() / "data/games/my/game/1.0/executable.exe" );
}
