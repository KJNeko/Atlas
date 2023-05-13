//
// Created by kj16609 on 4/17/23.
//

#include "FilepathModel.hpp"

#include <QIODevice>
#include <QMimeData>

#include "atlas/logging.hpp"

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
	if ( role == Qt::DisplayRole || role == Qt::EditRole )
		return QVariant::fromValue( QString::fromStdString( m_paths[ static_cast< unsigned long >( index.row() ) ]
		                                                        .string() ) );

	return {};
}

Qt::DropActions FilepathModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

bool FilepathModel::removeRows( int row, int count, const QModelIndex& parent )
{
	if ( row < 0 || static_cast< std::size_t >( row + count ) > m_paths.size() ) return false;

	beginRemoveRows( parent, row, row + count - 1 );

	for ( int i = 0; i < count; ++i ) m_paths.erase( m_paths.begin() + row );

	endRemoveRows();
	return true;
}

Qt::ItemFlags FilepathModel::flags( const QModelIndex& index ) const
{
	const Qt::ItemFlags defaults = QAbstractListModel::flags( index );
	if ( index.isValid() )
		return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled | defaults;
	else
		return Qt::ItemIsDropEnabled | defaults;
}

bool FilepathModel::insertRows( int row, int col, const QModelIndex& parent )
{
	if ( row < 0 ) return false;
	if ( row > rowCount( {} ) ) return false;

	beginInsertRows( parent, row, row + col - 1 );

	if ( row == rowCount( {} ) )
	{
		//Insert to end
		for ( int i = 0; i < col; ++i )
		{
			m_paths.push_back( std::filesystem::path() );
		}
	}
	else if ( row == 0 )
	{
		//beg
		for ( int i = 0; i < col; ++i )
		{
			m_paths.insert( m_paths.begin(), std::filesystem::path() );
		}
	}
	else
	{
		for ( int i = 0; i < col; ++i )
		{
			m_paths.insert( m_paths.begin() + row, std::filesystem::path() );
		}
	}

	endInsertRows();
	return true;
}

bool FilepathModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	if ( role == Qt::DisplayRole || role == Qt::EditRole )
	{
		m_paths.at( static_cast< unsigned long >( index.row() ) ) = value.value< QString >().toStdString();
		return true;
	}

	return false;
}

bool FilepathModel::moveRows(
	const QModelIndex& sourceParent,
	int sourceRow,
	int count,
	const QModelIndex& destinationParent,
	int destinationChild )
{
	const auto s_beg = m_paths.begin() + sourceRow;
	const auto s_end = s_beg + count;

	if ( s_end > m_paths.end() ) return false;

	const std::vector< std::filesystem::path > data { s_beg, s_end };

	beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild );

	m_paths.erase( s_beg, s_end );

	if ( destinationChild > static_cast< int >( m_paths.size() ) )
		destinationChild = static_cast< int >( m_paths.size() );

	m_paths.insert( m_paths.begin() + destinationChild, data.begin(), data.end() );

	endMoveRows();

	return true;
}