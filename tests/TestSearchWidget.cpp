//
// Created by kj16609 on 3/4/23.
//

#include <h95/database/Database.hpp>

#include <ui/widgets/SearchWidget.hpp>

#include "GTestBox.hpp"

#include <QApplication>
#include <QTest>
#include <QPushButton>

class TestSearchWidget : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication* app { nullptr };

	void SetUp() override
	{
		app = new QApplication( argc, argv );
		Database::initalize( "./data/testing.db" );
	}

	void TearDown() override
	{
		delete app;
		Database::deinit();
		std::filesystem::remove( "./data/testing.db" );
	}
};

TEST_F( TestSearchWidget, emptySearch )
{
	SearchWidget widget;

	std::vector< Record > records;
	QObject::connect(
		&widget,
		&SearchWidget::searchComplete,
		[&]( std::vector< Record > records_in ) { records = std::move( records_in ); } );

	widget.search();

	GTEST_ASSERT_EQ( records.size(), 0 );
}

TEST_F( TestSearchWidget, search )
{
	//Prepare
	{
		Transaction transaction;

		//Import a bunch of test records
		std::vector< Record > records {
			Record::create(
				"Test Record 1",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 2",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 3",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 4",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
		};

		transaction.commit();
	}

	SearchWidget widget;

	std::vector< Record > records;
	QObject::connect(
		&widget,
		&SearchWidget::searchComplete,
		[&]( std::vector< Record > records_in ) { records = std::move( records_in ); } );

	widget.search();

	GTEST_ASSERT_EQ( records.size(), 4 );
}

TEST_F( TestSearchWidget, buttonTest )
{

	//Prepare
	{
		Transaction transaction;

		//Import a bunch of test records
		std::vector< Record > records {
			Record::create(
				"Test Record 1",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 2",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 3",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
			Record::create(
				"Test Record 4",
				"Some Person",
				"Some Engine",
				{},
				"./assets/placeholder.jpg",
				{},
				transaction ),
		};

		transaction.commit();
	}

	SearchWidget widget;

	std::vector< Record > records;
	QObject::connect(
		&widget,
		&SearchWidget::searchComplete,
		[&]( std::vector< Record > records_in ) { records = std::move( records_in ); } );

	QTest::mouseClick( widget.findChild< QPushButton* >( "searchButton" ), Qt::LeftButton );

	GTEST_ASSERT_EQ( records.size(), 4 );
}