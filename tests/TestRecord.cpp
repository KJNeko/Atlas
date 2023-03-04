//
// Created by kj16609 on 3/2/23.
//

#include <h95/database/Record.hpp>

#include <h95/database/Database.hpp>

#include "GTestBox.hpp"

class TestRecord : public ::testing::Test
{
	void SetUp() override { Database::initalize( "./data/testing.db" ); }
	void TearDown() override
	{
		Database::deinit();
		std::filesystem::remove( "./data/testing.db" );
	}
};

TEST_F( TestRecord, createRecord )
{
	Transaction transaction;
	Record::create(
		"My Title",
		"Some Person",
		"Some Engine",
		{ { "V1.0", "./bin/", "./bin/Hydrus95.exe" } },
		"./assets/banner/placeholder.jpg",
		{ "./assets/banner/placeholder.jpg" },
		transaction );
	transaction.commit();
}

TEST_F( TestRecord, createExistingRecord )
{
	{
		Transaction transaction;
		Record::create(
			"My Title",
			"Some Person",
			"Some Engine",
			{ { "V1.0", "./bin/", "./bin/Hydrus95.exe" } },
			"./assets/banner/placeholder.jpg",
			{ "./assets/banner/placeholder.jpg" },
			transaction );
		transaction.commit();
	}

	{
		Transaction transaction;
		ASSERT_THROW(
			Record::create(
				"My Title",
				"Some Person",
				"Some Engine",
				{ { "v1.5", "./bin/", "./bin/Hydrus95.exe" } },
				"",
				{},
				transaction ),
			RecordAlreadyExists );
	}
}

TEST_F( TestRecord, createDuplicateRecord )
{
	{
		Transaction transaction;
		Record::create(
			"My Title",
			"Some Person",
			"Some Engine",
			{ { "V1.0", "./bin/", "./bin/Hydrus95.exe" } },
			"./assets/placeholder.jpg",
			{ "./assets/placeholder.jpg" },
			transaction );
		transaction.commit();
	}

	{
		Transaction transaction;
		ASSERT_THROW(
			Record::create(
				"My Title",
				"Some Person",
				"Some Engine",
				{ { "v1.0", "./bin/", "./bin/Hydrus95.exe" } },
				"./assets/banner/placeholder.jpg",
				{ "./assets/banner/placeholder.jpg" },
				transaction ),
			RecordAlreadyExists );
	}
}

TEST_F( TestRecord, select )
{
	std::optional< Record > record;

	{
		Transaction transaction;
		record = Record::create(
			"My Title",
			"Some Person",
			"Some Engine",
			{ { "V1.0", "./bin/", "./bin/Hydrus95.exe" } },
			"./assets/banner/placeholder.jpg",
			{ "./assets/banner/placeholder.jpg" },
			transaction );
		transaction.commit();
	}

	GTEST_ASSERT_TRUE( record.has_value() );

	{
		Transaction transaction;

		const auto record_comp { Record::select( record->m_id, transaction ) };
		const auto& rec { *record };
		GTEST_ASSERT_EQ( rec.m_id, record_comp.m_id );
		GTEST_ASSERT_EQ( rec.m_versions, record_comp.m_versions );
		GTEST_ASSERT_EQ( rec.m_engine, record_comp.m_engine );
		GTEST_ASSERT_EQ( rec.m_title, record_comp.m_title );
		GTEST_ASSERT_EQ( rec.m_creator, record_comp.m_creator );
		GTEST_ASSERT_EQ( rec.m_previews, record_comp.m_previews );
		GTEST_ASSERT_EQ( rec.m_banner, record_comp.m_banner );
		GTEST_ASSERT_EQ( rec, record_comp );
	}
}

TEST_F( TestRecord, selectZero )
{
	Transaction transaction;
	ASSERT_THROW( Record::select( 0, transaction ), InvalidRecordID );
}

TEST_F( TestRecord, selectNonExisting )
{
	Transaction transaction;
	ASSERT_THROW( Record::select( 50, transaction ), InvalidRecordID );
}
