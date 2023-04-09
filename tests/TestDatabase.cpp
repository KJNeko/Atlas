//
// Created by kj16609 on 3/2/23.
//

#include <gtest/gtest.h>

#include "atlas/database/Database.hpp"

TEST( TestDatabase, testTransactionUnloaded )
{
	EXPECT_THROW( []() { Transaction trans { Transaction::NoAutocommit }; }(), TransactionInvalid );
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
		Transaction transaction { Transaction::Autocommit };
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
			Transaction transaction_1 { Transaction::NoAutocommit };
			Transaction transaction_2 { Transaction::NoAutocommit };
		}(),
		std::runtime_error );
}