//
// Created by kj16609 on 3/2/23.
//

#include <h95/database/Database.hpp>

#include "GTestBox.hpp"

TEST( TestDatabase, testTransactionUnloaded )
{
	EXPECT_THROW(
		[]()
		{
			Transaction trans;
		}(),
		TransactionInvalid );
}

TEST( TestDatabase, testInit )
{
	Database::initalize( "./data/testing.db" );
	Database::deinit();
	std::filesystem::remove( "./data/testing.db" );

	SUCCEED();
}

TEST( TestDatabase, testTransaction )
{

	Database::initalize( "./data/testing.db" );
	{
		Transaction transaction;
	}
	Database::deinit();
	std::filesystem::remove( "./data/testing.db" );
}

TEST( TestDatabase, testUpdate )
{
	GTEST_SKIP() << "To be implemented later";
}
