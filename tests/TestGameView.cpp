//
// Created by kj16609 on 3/2/23.
//

#include <ui/widgets/listViews/GameView.hpp>

#include <QApplication>

#include "GTestBox.hpp"

class TestGameView : public ::testing::Test
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

	public:
	inline static std::vector< Record >& exposeRecords( GameViewModel& model ) { return model.m_records; }
};

TEST_F( TestGameView, setRecords )
{
	GameView view;

	Transaction transaction;

	std::vector< Record > records { Record::create(
		"Test Record",
		"Some Person",
		"Some Engine",
		{},
		"./assets/placeholder.jpg",
		{},
		transaction ) };

	transaction.commit();

	view.setRecords( std::move( records ) );

	//const auto& delegate {*dynamic_cast<GameViewDelegate*>(view.itemDelegate())};

	GTEST_ASSERT_EQ( TestGameView::exposeRecords( view.model() ).size(), 1 );
}
