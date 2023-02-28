//
// Created by kj16609 on 2/26/23.
//

#include "GameEditImageModel.hpp"

#include <h95/logging.hpp>

QModelIndex GameEditImageModel::index( int row, int column, [[maybe_unused]] const QModelIndex& parent ) const
{
	return QAbstractItemModel::createIndex( row, column );
}

QModelIndex GameEditImageModel::parent( [[maybe_unused]] const QModelIndex& index ) const
{
	return {};
}

int GameEditImageModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( ( std::filesystem::exists( m_banner ) ? 1 : 0 ) + m_previews.size() );
}

int GameEditImageModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 2;
}

QVariant GameEditImageModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		const bool has_banner { !m_banner.empty() && std::filesystem::exists( m_banner ) };

		switch ( index.column() )
		{
			case 0:
			{
				if ( has_banner )
				{
					if ( index.row() == 0 )
					{
						//We have a banner and we are at the 0th position
						return QString::fromStdString( m_banner.string() );
					}
					else
					{
						return QString::fromStdString(
							m_previews.at( static_cast< std::size_t >( index.row() - 1 ) ).string() );
					}
				}
				else
					return QString::fromStdString(
						m_previews.at( static_cast< std::size_t >( index.row() ) ).string() );
			}
			case 1:
			{
				if ( has_banner && index.row() > 0 )
					return QString( "Preview" );
				else
					return QString( "Banner" );
			}
			default:
				return QString( "The fuck?" );
		}
	}
	else
		return {};
}

QVariant GameEditImageModel::headerData( int section, Qt::Orientation oreintation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( oreintation == Qt::Horizontal )
		{
			switch ( section )
			{
				case 0:
					return "Path";
				case 1:
					return "Type";
				default:
					return {};
			}
		}
		else
			return QString::number( section + 1 );
	}
	return {};
}

void GameEditImageModel::removeItems( const std::vector< std::size_t >& indexes )
{
	const bool has_banner { !m_banner.empty() && std::filesystem::exists( m_banner ) };

	std::size_t offset { 0 };

	if ( m_previews.empty() ) return;

	for ( const auto& index : indexes )
	{
		//Is the index the banner or preview?
		//If we are trying to remove the banner then just skip.
		if ( has_banner && index - offset == 0 ) continue;

		beginRemoveRows( {}, static_cast< int >( index - offset ), static_cast< int >( index - offset ) );

		m_previews.erase( m_previews.begin() + static_cast< long >( index - offset ) - ( has_banner ? 1 : 0 ) );
		++offset;

		endRemoveRows();
	}
}

void GameEditImageModel::addPreview( std::string path )
{
	beginInsertRows( {}, rowCount( {} ), rowCount( {} ) );

	m_previews.emplace_back( std::move( path ) );

	endInsertRows();
}
