//
// Created by kj16609 on 3/31/23.
//

#include "GTestBox.hpp"
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
	record->addVersion( "1.0", "my/game/1.0", "executable.exe", false );
	GameMetadata& metadata { record->getLatestVersion() };

	GTEST_ASSERT_EQ( config::paths::games::getPath().string(), "./data/games" );
	GTEST_ASSERT_EQ( metadata.getPath().string(), "./data/games/my/game/1.0" );
	GTEST_ASSERT_EQ( metadata.getExecPath( true ).string(), "./data/games/my/game/1.0/executable.exe" );
}
