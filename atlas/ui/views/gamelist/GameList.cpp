//
// Created by kj16609 on 5/25/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameList.h" resolved

#include "GameList.hpp"

#include "GameListDelegate.hpp"

GameList::GameList( QWidget* parent ) : QListView( parent )
{
	setItemDelegate( new GameListDelegate() );
}
