//
// Created by kj16609 on 4/17/23.
//

#include "ImageView.hpp"

#include <moc_ImageView.cpp>

#include "ui/delegates/ImageDelegate.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageView::setPaths( const std::vector< std::filesystem::path >& paths )
{
	auto model { dynamic_cast< FilepathModel* >( QListView::model() ) };
	model->setFilepaths( paths );
}

ImageView::ImageView( QWidget* parent ) : QListView( parent )
{
	auto* filepath_model { new FilepathModel() };

	QListView::setModel( filepath_model );
	QListView::setItemDelegate( new ImageDelegate( filepath_model ) );

	connect( pathmodel(), &FilepathModel::reordered, this, &ImageView::modelReordered );
}

FilepathModel* ImageView::pathmodel() const
{
	return dynamic_cast< FilepathModel* >( QListView::model() );
}

FilepathModel* ImageView::pathmodel()
{
	return dynamic_cast< FilepathModel* >( QListView::model() );
}

ImageDelegate* ImageView::delegate()
{
	return dynamic_cast< ImageDelegate* >( QListView::itemDelegate() );
}

void ImageView::modelReordered()
{
	emit reordered( paths() );
}

std::vector< std::filesystem::path > ImageView::selectedItems() const
{
	auto model { dynamic_cast< FilepathModel* >( QListView::model() ) };

	std::vector< std::filesystem::path > paths;

	for ( const auto& index : selectionModel()->selectedIndexes() )
	{
		const auto& path { model->data( index ).value< std::filesystem::path >() };
		paths.push_back( path );
	}

	return paths;
}

std::vector< std::filesystem::path > ImageView::paths() const
{
	return pathmodel()->getFilepaths();
}

std::vector< QString > ImageView::pathsQString() const
{
	std::vector< QString > pathsQString;
	const auto std_paths { paths() };

	for ( const auto& path : std_paths )
	{
		pathsQString.push_back( QString::fromStdString( path.string() ) );
	}

	return pathsQString;
}