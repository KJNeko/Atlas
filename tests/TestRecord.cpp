//
// Created by kj16609 on 3/2/23.
//

#include <gtest/gtest.h>

#include "atlas/database/Database.hpp"
#include "atlas/database/RecordData.hpp"
#include "atlas/logging.hpp"

class TestRecord : public ::testing::Test
{
	void SetUp() override { Database::initalize( ":memory:" ); }

	void TearDown() override { Database::deinit(); }
};

TEST_F( TestRecord, createRecord )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };

	Record record { title, creator, engine };
	record->addVersion( "V1.0", "./bin/", "Hydrus95.exe", false, 0 );
}

TEST_F( TestRecord, createExistingRecord )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };

	{
		Record record { title, creator, engine };
	}

	{
		ASSERT_THROW( Record( title, creator, engine ), RecordAlreadyExists );
	}
}

TEST_F( TestRecord, select )
{
	std::optional< Record > record;

	{
		QString title { "my title" };
		QString creator { "Some Person" };
		QString engine { "Some Engine" };

		record = Record( title, creator, engine );
	}

	GTEST_ASSERT_TRUE( record.has_value() );

	{
		const Record record_comp { ( *record )->getID() };
		const auto& rec { *record };
		GTEST_ASSERT_EQ( rec->getID(), record_comp->getID() );
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

#define PREPARE_RECORD_TEST                                                                                            \
	Record record { []() -> Record                                                                                     \
		            {                                                                                                  \
						QString title { "my title" };                                                                  \
						QString creator { "Some Person" };                                                             \
						QString engine { "Some Engine" };                                                              \
                                                                                                                       \
						return Record( title, creator, engine );                                                       \
					}() };

#define TEST_RECORD_EQ                                                                                                 \
	{                                                                                                                  \
		const Record record_comp { record->getID() };                                                                  \
		GTEST_ASSERT_EQ( record->getID(), record_comp->getID() );                                                      \
		GTEST_ASSERT_EQ( record->getVersions(), record_comp->getVersions() );                                          \
		GTEST_ASSERT_EQ( record->getEngine(), record_comp->getEngine() );                                              \
		GTEST_ASSERT_EQ( record->getTitle(), record_comp->getTitle() );                                                \
		GTEST_ASSERT_EQ( record->getCreator(), record_comp->getCreator() );                                            \
		GTEST_ASSERT_EQ( record->getPreviewPaths(), record_comp->getPreviewPaths() );                                  \
		GTEST_ASSERT_EQ( record->getBannerPath(), record_comp->getBannerPath() );                                      \
		GTEST_ASSERT_EQ( record, record_comp );                                                                        \
	}

TEST_F( TestRecord, setTitle )
{
	PREPARE_RECORD_TEST

	const auto test_value { "Some Creator" };

	record->setTitle( test_value );

	ASSERT_EQ( record->getTitle(), test_value );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, setCreator )
{
	PREPARE_RECORD_TEST

	const auto test_value { "Some Creator" };

	record->setCreator( test_value );

	ASSERT_EQ( record->getCreator(), test_value );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, setEngine )
{
	PREPARE_RECORD_TEST

	const auto test_value { "Some Creator" };

	record->setEngine( test_value );

	ASSERT_EQ( record->getEngine(), test_value );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, setLastPlayed )
{
	PREPARE_RECORD_TEST

	const std::uint64_t test_value { 12345 };

	record->setLastPlayed( test_value );

	ASSERT_EQ( record->getLastPlayed(), test_value );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, setTotalPlaytime )
{
	PREPARE_RECORD_TEST

	const std::uint64_t test_value { 69420 };

	record->setTotalPlaytime( test_value );

	ASSERT_EQ( record->getTotalPlaytime(), test_value );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, addVersion )
{
	PREPARE_RECORD_TEST

	record->addVersion( "V69.420", "creator/title1/v1.0", "Hydrus95.exe", true, 0 );
	record->addVersion( "v1.0", "creator/title2/v1.0", "Game.exe", true, 0 );

	ASSERT_EQ( record->getVersions().size(), 2 );
	ASSERT_EQ( record->getVersions().at( 1 ).getVersionName().toStdString(), "v1.0" );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, removeVersion )
{
	PREPARE_RECORD_TEST

	record->addVersion( "V69.420", ".", "./Hydrus95.exe", true, 0 );

	record->removeVersion( record->getVersions().at( 0 ) );

	ASSERT_EQ( record->getVersions().size(), 0 );

	TEST_RECORD_EQ
}

TEST_F( TestRecord, sync )
{
	PREPARE_RECORD_TEST

	Transaction trans { Transaction::Autocommit };

	record->setTitle( "Some new title", trans );

	trans.abort();

	record->sync();

	TEST_RECORD_EQ
}

TEST_F( TestRecord, Example1 )
{
	QString title { "my title" };
	QString creator { "Some Person" };
	QString engine { "Some Engine" };

	Record record { title, creator, engine };
}

TEST_F( TestRecord, Example2 )
{
	Transaction transaction { Transaction::Autocommit };

	QString title { "my title 2" };
	QString creator { "Some Person 2" };
	QString engine { "Some Engine 2" };

	Record record { title, creator, engine, transaction };

	transaction.commit();
}
