//
// Created by kj16609 on 3/4/23.
//

#include "GameViewModel.hpp"

int GameViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_records.size() );
}

QVariant GameViewModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		return QVariant::fromValue( &( m_records.at( static_cast< std::size_t >( index.row() ) ) ) );
	}
	else
		return QVariant::fromValue(nullptr);
}

void GameViewModel::setRecords( std::vector< Record > records )
{
	beginResetModel();
	m_records = std::move( records );
	endResetModel();
}
