//
// Created by kj16609 on 3/1/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PreviewView.h" resolved

#include "PreviewView.hpp"

#include <QUrl>

QModelIndex PreviewViewModel::index( int row, int column, [[maybe_unused]] const QModelIndex& parent ) const
{
	return QAbstractListModel::index( row, column, parent );
}

QModelIndex PreviewViewModel::parent( [[maybe_unused]] const QModelIndex& index ) const
{
	return {};
}

int PreviewViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_previews.size() );
}

QVariant PreviewViewModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		return QUrl::fromLocalFile(
			QString::fromStdString( m_previews.at( static_cast< std::size_t >( index.row() ) ).string() ) );
	}
	else
		return {};
}

std::vector< std::filesystem::path > PreviewViewModel::previews()
{
	return m_previews;
}

void PreviewViewModel::setPreviews( std::vector< std::filesystem::path > previews )
{
	m_previews = std::move( previews );
}

void PreviewViewModel::addPreview( std::filesystem::path path )
{
	beginInsertRows( {}, rowCount(), rowCount() );
	m_previews.emplace_back( std::move( path ) );
	endInsertRows();
}

void PreviewViewModel::removePreview( std::size_t index )
{
	beginRemoveRows( {}, static_cast< int >( index ), static_cast< int >( index ) );

	m_previews.erase( m_previews.begin() + static_cast< long >( index ) );

	endRemoveRows();
}


PreviewView::PreviewView( QWidget* parent ) : QListView( parent )
{
	QListView::setModel( new PreviewViewModel() );
}

PreviewView::~PreviewView() {}

std::vector< std::filesystem::path > PreviewView::previews() const
{
	return dynamic_cast<PreviewViewModel*>(model())->previews();
}

void PreviewView::setPreviews( std::vector< std::filesystem::path > paths )
{
	dynamic_cast<PreviewViewModel*>(model())->setPreviews(std::move(paths));
}
