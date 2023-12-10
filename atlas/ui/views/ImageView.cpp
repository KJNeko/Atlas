//
// Created by kj16609 on 4/17/23.
//

#include "ImageView.hpp"

#include <moc_ImageView.cpp>

#include "ui/delegates/ImageDelegate.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageView::setPaths( const std::vector< std::filesystem::path >& paths )
{
	filepathModel()->killLoaders();
	filepathModel()->setFilepaths( paths );
}

ImageView::ImageView( QWidget* parent ) : QListView( parent )
{
	QListView::setModel( new FilepathModel() );
	QListView::setItemDelegate( new ImageDelegate( filepathModel() ) );

	connect( filepathModel(), &FilepathModel::reordered, this, &ImageView::modelReordered );
}

void ImageView::modelReordered()
{
	emit reordered( paths() );
}

std::vector< std::filesystem::path > ImageView::selectedItems() const
{
	auto model { filepathModel() };

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
	return filepathModel()->getFilepaths();
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