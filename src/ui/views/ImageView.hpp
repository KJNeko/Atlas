//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_IMAGEVIEW_HPP
#define ATLASGAMEMANAGER_IMAGEVIEW_HPP

#include <QListView>
#include <QMouseEvent>

#include "atlas/database/RecordData.hpp"

class ImageView : public QListView
{
	Q_OBJECT

  public:

	ImageView( QWidget* parent = nullptr );

  public slots:
	void setPaths( const std::vector< std::filesystem::path >& paths );
};

#endif //ATLASGAMEMANAGER_IMAGEVIEW_HPP
