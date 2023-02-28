//
// Created by kj16609 on 2/25/23.
//

#include "GameEditVersionModel.hpp"

#include <h95/logging.hpp>


QModelIndex GameEditVersionModel::index( int row, int column, [[maybe_unused]] const QModelIndex& parent ) const
{
	return QAbstractItemModel::createIndex( row, column );
}

QModelIndex GameEditVersionModel::parent( [[maybe_unused]] const QModelIndex& index ) const
{
	return {};
}

int GameEditVersionModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_versions.size() );
}

int GameEditVersionModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 5;
}

QVariant GameEditVersionModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		const auto& v_data { m_versions.at(
			static_cast< std::remove_reference_t< decltype( m_versions ) >::size_type >( index.row() ) ) };

		switch ( index.column() )
		{
			case 0:
				return v_data.m_version;
			case 1:
				return QString::fromStdString( v_data.m_game_path.string() );
			case 2:
				return QString::fromStdString( v_data.m_exec_path.string() );
			case 3:
			case 4:
			default:
				return QString( "I fucked up" );
		}
	}
	else
		return {};
}

QVariant GameEditVersionModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( orientation == Qt::Horizontal )
		{
			switch ( section )
			{
				case 0:
					return QString( "Version" );
				case 1:
					return QString( "Path" );
				case 2:
					return QString( "Executable" );
				case 3:
					return QString( "Last played" );
				case 4:
					return QString( "Total playtime" );
				default:
					return {};
			}
		}
		else { return QString::number( section + 1 ); }
	}

	return {};
}

void GameEditVersionModel::removeItem( std::size_t index )
{
	if ( m_versions.empty() ) return;

	beginRemoveRows( {}, static_cast< int >( index ), static_cast< int >( index ) );

	m_versions.erase( m_versions.begin() + static_cast< long >( index ) );

	endRemoveRows();
}
