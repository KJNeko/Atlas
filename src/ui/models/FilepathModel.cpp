//
// Created by kj16609 on 4/17/23.
//

#include "FilepathModel.hpp"

void FilepathModel::setFilepaths( const std::vector< std::filesystem::path >& filepaths )
{
	beginResetModel();
	this->m_paths = filepaths;
	endResetModel();
}

FilepathModel::FilepathModel( QObject* parent ) : QAbstractListModel( parent )
{}

QModelIndex FilepathModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
		return QModelIndex();
	else
		return createIndex( row, column );
}

QModelIndex FilepathModel::parent( [[maybe_unused]] const QModelIndex& child ) const
{
	return QModelIndex();
}

int FilepathModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_paths.size() );
}

QVariant FilepathModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
		return QVariant::fromStdVariant( std::variant<
										 std::filesystem::path >( m_paths.at( static_cast<
																			  unsigned long >( index.row() ) ) ) );
	else
		return QVariant();
}
