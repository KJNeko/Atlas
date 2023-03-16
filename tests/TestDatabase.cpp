//
// Created by kj16609 on 3/2/23.
//

#include "GTestBox.hpp"
#include "h95/database/Database.hpp"

TEST( TestDatabase, testTransactionUnloaded )
{
	EXPECT_THROW( []() { Transaction trans; }(), TransactionInvalid );
}

TEST( TestDatabase, testInit )
{
	if ( std::filesystem::exists( "./data/" ) ) std::filesystem::remove_all( "./data/" );

	Database::initalize( "./data/testing.db" );
	Database::deinit();
	std::filesystem::remove_all( "./data/" );

	SUCCEED();
}

TEST( TestDatabase, testTransaction )
{
	Database::initalize( "./data/testing.db" );
	{
		Transaction transaction;
	}
	Database::deinit();
	std::filesystem::remove_all( "./data" );
}

TEST( TestDatabase, testUpdate )
{
	GTEST_SKIP() << "To be implemented later";
}

TEST( TestDatabase, testDeadlock )
{
	Database::initalize( "./data/testing.db" );

	EXPECT_THROW(
		[]()
		{
			Transaction transaction_1;
			Transaction transaction_2;
		}(),
		std::runtime_error );
}