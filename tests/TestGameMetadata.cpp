//
// Created by kj16609 on 3/2/23.
//

#include <h95/database/GameMetadata.hpp>

#include "GTestBox.hpp"


class TestGameMetadata : public ::testing::Test
{
	void SetUp() override { Database::initalize( "./data/testing.db" ); }
	void TearDown() override
	{
		Database::deinit();
		std::filesystem::remove( "./data/testing.db" );
	}
};

TEST_F( TestGameMetadata, insertMetadata )
{
	Transaction transaction;
	GameMetadata::insert( 1, { "v1.0", ".", "./Hydrus95" }, transaction );
	transaction.commit();
}

TEST_F( TestGameMetadata, insertDuplicateMetadata )
{
	//Test prep
	{
		Transaction transaction;
		GameMetadata::insert( 1, { "v1.0", ".", "./Hydrus95" }, transaction );
		transaction.commit();
	}


	{
		Transaction transaction;
		EXPECT_THROW( GameMetadata::insert( 1, { "v1.0", ".", "./Hydrus95" }, transaction ), MetadataAlreadyExists );
	}
}

TEST_F( TestGameMetadata, selectMetadata )
{
	std::optional< GameMetadata > ref { std::nullopt };

	//Test prep
	{
		Transaction transaction;
		ref = GameMetadata::insert( 1, { "v1.0", ".", "./Hydrus95" }, transaction );
		transaction.commit();
	}

	GTEST_ASSERT_TRUE( ref.has_value() );

	{
		Transaction transaction;
		std::vector< GameMetadata > metadata { GameMetadata::select( 1, transaction ) };
		transaction.commit();

		GTEST_ASSERT_EQ( metadata.size(), 1 );
		GTEST_ASSERT_EQ( metadata.at( 0 ), ref.value() );
	}
}

TEST_F( TestGameMetadata, selectInvalidMetadata )
{
	Transaction transaction;
	GTEST_ASSERT_EQ( GameMetadata::select( 4, transaction ).size(), 0 );
	transaction.commit();
}