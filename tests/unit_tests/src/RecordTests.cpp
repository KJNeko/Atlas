//
// Created by kj16609 on 1/15/23.
//

#include <gtest/gtest.h>

#include <backend/Record.hpp>
#include <backend/database.hpp>


class RecordCreation : public ::testing::Test
{
	inline static bool failed_last { false };

	void SetUp() override
	{
		if ( failed_last ) GTEST_SKIP() << "Skipping due to failed test previously";

		database::initalize();
	}

	void TearDown() override
	{
		if ( this->HasFailure() || this->IsSkipped() )
		{
			failed_last = true;
			return;
		}

		std::filesystem::remove( "./data/config.ini" );
		std::filesystem::remove( "./data/hydrus95.db" );
	}
};

TEST_F( RecordCreation, GameRecord )
{
	GameMetadata g_metadata;
	g_metadata.creator_name = "Test";
	g_metadata.executable_path = "no.exe";
	g_metadata.folder_path = "your/moms/house";
	g_metadata.h95_ownerhip = false;
	g_metadata.version_string = "v1.96.420";

	Record record { Record::create( GAME_RECORD, { g_metadata } ) };
	ASSERT_GT( record.m_id, 0 );

	Record db_record { Record::select( record.m_id ) };
	ASSERT_EQ( record, db_record );
}

TEST_F( RecordCreation, GameRecordIncomplete )
{
	GameMetadata g_metadata;
	g_metadata.creator_name = "Test";
	g_metadata.version_string = "v1.0";

	Record record { Record::create( GAME_RECORD, { g_metadata } ) };
	ASSERT_GT( record.m_id, 0 );

	Record db_record { Record::select( record.m_id ) };
	ASSERT_EQ( record, db_record );
}
