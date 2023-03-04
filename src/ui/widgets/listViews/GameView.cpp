//
// Created by kj16609 on 3/2/23.
//

#include "GameView.hpp"

#include <QPainter>

#include <h95/logging.hpp>

GameView::GameView( QWidget* parent ) : QListView( parent )
{
	this->setModel( new GameViewModel() );
	this->setItemDelegate( new GameViewDelegate() );

	this->setWrapping(true);
	this->setFlow(QListView::LeftToRight);
	this->setMovement(QListView::Free);
	this->setResizeMode(QListView::Adjust);
}

void GameView::setRecords( std::vector< Record > records )
{
	model().setRecords( std::move( records ) );
}

GameViewModel& GameView::model()
{
	return *dynamic_cast< GameViewModel* >( QListView::model() );
}

GameViewDelegate& GameView::itemDelegate()
{
	return *dynamic_cast< GameViewDelegate* >( QListView::itemDelegate() );
}
