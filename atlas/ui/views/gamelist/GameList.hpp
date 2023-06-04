//
// Created by kj16609 on 5/25/23.
//

#ifndef ATLASGAMEMANAGER_GAMELIST_HPP
#define ATLASGAMEMANAGER_GAMELIST_HPP

#include <QListView>

#include "core/database/record/Record.hpp"
#include "ui/models/RecordListModel.hpp"

//! A list that displays a list of games.
class GameList : public QListView
{
	Q_OBJECT

  public:

	explicit GameList( QWidget* parent = nullptr );
	~GameList() override = default;
};

#endif //ATLASGAMEMANAGER_GAMELIST_HPP