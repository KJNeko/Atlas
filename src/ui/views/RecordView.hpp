//
// Created by kj16609 on 3/29/23.
//

#ifndef ATLAS_RECORDVIEW_HPP
#define ATLAS_RECORDVIEW_HPP

#include <QListView>

#include "atlas/Types.hpp"
#include "atlas/database/Record.hpp"
#include "atlas/database/RecordData.hpp"

enum DelegateType
{
	NO_MODE = 0,
	BANNER_VIEW = 1,
};

class RecordView : public QListView
{
	Q_OBJECT

  private:

	DelegateType current_render_mode { NO_MODE };

  public:

	RecordView( QWidget* parent = nullptr );

	void mouseDoubleClickEvent( QMouseEvent* event ) override;

	void reloadConfig();

  signals:
	void openDetailedView( const Record record );

  public slots:
	void addRecords( const std::vector< RecordID > records );
	void setRecords( const std::vector< Record > records );
	void setRenderMode( const DelegateType type );
	void on_customContextMenuRequested( const QPoint& pos );
};

#endif //ATLAS_RECORDVIEW_HPP