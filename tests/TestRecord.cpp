//
// Created by kj16609 on 3/2/23.
//

#include <h95/database/Record.hpp>

#include <h95/database/Database.hpp>

#include "GTestBox.hpp"
#include <h95/logging.hpp>

class TestRecord : public ::testing::Test
{
	void SetUp() override { Database::initalize( "./data/testing.db" ); }
	void TearDown() override
	{
		Database::deinit();
		std::filesystem::remove_all( "./data" );
	}
};


TEST_F( TestRecord, createRecord )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };
	const std::uint64_t last_played { 0 };
	const std::uint32_t total_playtime { 0 };
	std::vector< GameMetadata > metadata { { "V1.0", "./bin/", "./bin/Hydrus95.exe", false, 0, 0 } };
	std::filesystem::path banner { "./assets/banner/placeholder.jpg" };
	std::vector< std::filesystem::path > previews { "./assets/banner/placeholder.jpg" };

	Record record { title, creator, engine, last_played, total_playtime, metadata, banner, previews };
}

TEST_F( TestRecord, createExistingRecord )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };
	const std::uint64_t last_played { 0 };
	const std::uint32_t total_playtime { 0 };
	std::vector< GameMetadata > metadata { { "V1.0", "./bin/", "./bin/Hydrus95.exe", false, 0, 0 } };
	std::filesystem::path banner { "./assets/banner/placeholder.jpg" };
	std::vector< std::filesystem::path > previews { "./assets/banner/placeholder.jpg" };

	{
		Record record { title, creator, engine, last_played, total_playtime, metadata, banner, previews };
	}

	{
		ASSERT_THROW(
			Record( title, creator, engine, last_played, total_playtime, metadata, banner, previews ),
			RecordAlreadyExists );
	}
}

TEST_F( TestRecord, select )
{
	std::optional< Record > record;

	{
		QString title { "my title" };
		QString creator { "Some Person" };
		QString engine { "Some Engine" };
		const uint64_t last_played { 0 };
		const std::uint32_t total_playtime { 0 };
		std::vector< GameMetadata > metadata { { "V1.0", "./bin/", "./bin/Hydrus95.exe", false, 0, 0 } };
		std::filesystem::path banner { "./assets/banner/placeholder.jpg" };
		std::vector< std::filesystem::path > previews { "./assets/banner/placeholder.jpg" };


		record = Record( title, creator, engine, last_played, total_playtime, metadata, banner, previews );
	}

	GTEST_ASSERT_TRUE( record.has_value() );

	{
		const Record record_comp { ( *record )->m_id };
		const auto& rec { *record };
		GTEST_ASSERT_EQ( rec->m_id, record_comp->m_id );
		GTEST_ASSERT_EQ( rec->getVersions(), record_comp->getVersions() );
		GTEST_ASSERT_EQ( rec->getEngine(), record_comp->getEngine() );
		GTEST_ASSERT_EQ( rec->getTitle(), record_comp->getTitle() );
		GTEST_ASSERT_EQ( rec->getCreator(), record_comp->getCreator() );
		GTEST_ASSERT_EQ( rec->getPreviewPaths(), record_comp->getPreviewPaths() );
		GTEST_ASSERT_EQ( rec->getBannerPath(), record_comp->getBannerPath() );
		GTEST_ASSERT_EQ( rec, record_comp );
	}
}

TEST_F( TestRecord, selectZero )
{
	ASSERT_THROW( Record( RecordID( 0 ) ), InvalidRecordID );
}

TEST_F( TestRecord, selectNonExisting )
{
	ASSERT_THROW( Record( RecordID( 0 ) ), InvalidRecordID );
}


TEST_F( TestRecord, Example1 )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };
	const uint64_t last_played { 0 };
	const std::uint32_t total_playtime { 0 };
	std::vector< GameMetadata > metadata { { "V1.0", "./bin/", "./bin/Hydrus95.exe", false, 0, 0 } };
	std::filesystem::path banner { "./assets/banner/placeholder.jpg" };
	std::vector< std::filesystem::path > previews { "./assets/banner/placeholder.jpg" };

	Record record { title, creator, engine, last_played, total_playtime, metadata, banner, previews };
}

TEST_F( TestRecord, Example2 )
{
	Transaction transaction;

	QString title { "my title 2" };
	QString creator { "Some Person 2" };
	QString engine { "Some Engine 2" };
	const uint64_t last_played { 0 };
	const std::uint32_t total_playtime { 0 };
	std::vector< GameMetadata > metadata { { "V1.0", "./bin/", "./bin/Hydrus95.exe", false, 0, 0 } };
	std::filesystem::path banner { "./assets/banner/placeholder.jpg" };
	std::vector< std::filesystem::path > previews { "./assets/banner/placeholder.jpg" };

	Record record { title, creator, engine, last_played, total_playtime, metadata, banner, previews, transaction };

	transaction.commit();
}
