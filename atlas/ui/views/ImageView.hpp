//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_IMAGEVIEW_HPP
#define ATLASGAMEMANAGER_IMAGEVIEW_HPP

#include <QListView>
#include <QMouseEvent>

#include "core/database/record/GameData.hpp"

class FilepathModel;
class ImageDelegate;

class ImageView final : public QListView
{
	Q_OBJECT

  public:

	ImageView( QWidget* parent = nullptr );

	FilepathModel* pathmodel();
	FilepathModel* pathmodel() const;
	ImageDelegate* delegate();
	std::vector< std::filesystem::path > selectedItems() const;
	std::vector< std::filesystem::path > paths() const;
	std::vector< QString > pathsQString() const;

  private slots:
	void modelReordered();

  public slots:
	void setPaths( const std::vector< std::filesystem::path >& paths );

  signals:
	void reordered( std::vector< std::filesystem::path > paths );
};

#endif //ATLASGAMEMANAGER_IMAGEVIEW_HPP
