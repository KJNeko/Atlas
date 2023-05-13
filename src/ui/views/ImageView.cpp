//
// Created by kj16609 on 4/17/23.
//

#include "ImageView.hpp"

#include "ui/delegates/ImageDelegate.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageView::setPaths( const std::vector< std::filesystem::path >& paths )
{
	auto model { dynamic_cast< FilepathModel* >( QListView::model() ) };
	model->setFilepaths( paths );
}

ImageView::ImageView( QWidget* parent ) : QListView( parent )
{
	QListView::setModel( new FilepathModel() );
	QListView::setItemDelegate( new ImageDelegate() );

	connect( model(), &FilepathModel::reordered, this, &ImageView::modelReordered );
}

FilepathModel* ImageView::model()
{
	return dynamic_cast< FilepathModel* >( QListView::model() );
}

ImageDelegate* ImageView::delegate()
{
	return dynamic_cast< ImageDelegate* >( QListView::itemDelegate() );
}

void ImageView::modelReordered()
{
	emit reordered( model()->getFilepaths() );
}
