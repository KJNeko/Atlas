//
// Created by kj16609 on 3/2/23.
//

#ifndef HYDRUS95_GAMEVIEW_HPP
#define HYDRUS95_GAMEVIEW_HPP

#include <QListView>

#include "h95/database/Record.hpp"
#include "GameViewModel.hpp"
#include "GameViewDelegate.hpp"

class GameView final : public QListView
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( GameView )

	public:
	GameView( QWidget* parent = nullptr );

	GameViewModel& model();
	GameViewDelegate& itemDelegate();

	public slots:
	void setRecords( std::vector< Record > records );

	friend class TestGameView;
};


#endif	//HYDRUS95_GAMEVIEW_HPP
