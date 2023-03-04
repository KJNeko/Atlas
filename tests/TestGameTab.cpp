//
// Created by kj16609 on 3/4/23.
//

#include "GTestBox.hpp"
#include "ui/widgets/listViews/GameView.hpp"

#include <QApplication>
#include <h95/database/Database.hpp>

#include <ui/tabs/GameTab.hpp>
#include <QPushButton>

#include <QTest>

class TestGameTab : public ::testing::Test
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

TEST_F( TestGameTab, TriggerSearchZero )
{
	GameTab tab;
	tab.show();

	auto ptr_list { tab.findChildren< QPushButton* >( "searchButton" ) };

	GTEST_ASSERT_EQ( ptr_list.size(), 1 );

	auto* ptr { ptr_list.takeFirst() };

	QTest::mouseClick( ptr, Qt::LeftButton );

	//Check that the list was updated

	GameView* game_view { tab.findChild< GameView* >( "gameView" ) };

	GTEST_ASSERT_EQ( game_view->model().rowCount(), 0 );
}

TEST_F( TestGameTab, TriggerSearch )
{
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

	GameTab tab;

	tab.show();

	auto ptr_list { tab.findChildren< QPushButton* >( "searchButton" ) };

	GTEST_ASSERT_EQ( ptr_list.size(), 1 );

	auto* ptr { ptr_list.takeFirst() };

	QTest::mouseClick( ptr, Qt::LeftButton );

	//Check that the list was updated

	GameView* game_view { tab.findChild< GameView* >( "gameView" ) };

	GTEST_ASSERT_EQ( game_view->model().rowCount(), 4 );
}
