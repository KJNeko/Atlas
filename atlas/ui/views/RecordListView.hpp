//
// Created by kj16609 on 3/29/23.
//

#pragma once
#ifndef ATLAS_RECORDVIEW_HPP
#define ATLAS_RECORDVIEW_HPP

#include <QListView>
#include <QScrollEvent>

#include "core/Types.hpp"
#include "core/database/record/GameData.hpp"
#include "core/database/record/game/Game.hpp"

enum DelegateType
{
	NO_MODE = 0,
	BANNER_VIEW = 1,
};

class RecordListView final : public QListView
{
	Q_OBJECT

  private:

	DelegateType current_render_mode { NO_MODE };

  public:

	RecordListView( QWidget* parent = nullptr );

	void mouseDoubleClickEvent( QMouseEvent* event ) override;

	void reloadConfig();

	void paintEvent( QPaintEvent* event ) override;
	void wheelEvent( QWheelEvent* event ) override;
	void scrollContentsBy( int dx, int dy ) override;

  signals:
	void openDetailedView( const atlas::records::Game record );

  public slots:
	void addRecords( const std::vector< RecordID > records );
	void setRecords( const std::vector< atlas::records::Game > records );
	void setRenderMode( const DelegateType type );
	void on_customContextMenuRequested( const QPoint& pos );
};

#endif //ATLAS_RECORDVIEW_HPP