//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_IMAGEVIEW_HPP
#define ATLASGAMEMANAGER_IMAGEVIEW_HPP

#include <QListView>
#include <QMouseEvent>

#include "core/database/record/RecordData.hpp"

class FilepathModel;
class ImageDelegate;

class ImageView : public QListView
{
	Q_OBJECT

  public:

	ImageView( QWidget* parent = nullptr );

	FilepathModel* model();
	ImageDelegate* delegate();
	std::vector< std::filesystem::path > selectedItems() const;

  private slots:
	void modelReordered();

  public slots:
	void setPaths( const std::vector< std::filesystem::path >& paths );

  signals:
	void reordered( std::vector< std::filesystem::path > paths );
};

#endif //ATLASGAMEMANAGER_IMAGEVIEW_HPP
