//
// Created by kj16609 on 5/25/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_GAMELIST_HPP
#define ATLASGAMEMANAGER_GAMELIST_HPP

#include <QListView>

#include "core/database/record/game/Game.hpp"
#include "ui/models/RecordListModel.hpp"

//! A list that displays a list of games.
class GameList final : public QListView
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GameList )

  public:

	explicit GameList( QWidget* parent = nullptr );
	~GameList() override = default;
};

#endif //ATLASGAMEMANAGER_GAMELIST_HPP
