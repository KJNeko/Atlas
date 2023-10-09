//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_IMAGEVIEW_HPP
#define ATLASGAMEMANAGER_IMAGEVIEW_HPP

#include <QListView>
#include <QMouseEvent>

#include "core/database/record/GameData.hpp"
#include "ui/delegates/ImageDelegate.hpp"
#include "ui/models/FilepathModel.hpp"

class ImageView final : public QListView
{
	Q_OBJECT

  public:

	ImageView( QWidget* parent = nullptr );

	std::vector< std::filesystem::path > selectedItems() const;
	std::vector< std::filesystem::path > paths() const;
	std::vector< QString > pathsQString() const;

	FilepathModel* filepathModel() const { return dynamic_cast< FilepathModel* >( QListView::model() ); }

	ImageDelegate* delegate() const { return dynamic_cast< ImageDelegate* >( QListView::itemDelegate() ); }

  private slots:
	void modelReordered();

  public slots:
	void setPaths( const std::vector< std::filesystem::path >& paths );

  signals:
	void reordered( std::vector< std::filesystem::path > paths );
};

#endif //ATLASGAMEMANAGER_IMAGEVIEW_HPP
