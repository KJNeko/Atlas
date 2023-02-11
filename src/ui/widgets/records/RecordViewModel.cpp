//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewModel.hpp"

#include <tracy/Tracy.hpp>

QVariant RecordViewModel::data( const QModelIndex& index, int role ) const
{
	ZoneScoped;
	if ( role == Qt::DisplayRole )
	{
		return QVariant::fromValue( &( records.at( static_cast< unsigned long >( index.row() ) ) ) );
	}

	return {};
}

int RecordViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	ZoneScoped;
	return static_cast< int >( records.size() );
}

void RecordViewModel::setRecords( const std::vector< Record >& records_in )
{
	ZoneScoped;
	beginResetModel();

	records = records_in;

	endResetModel();
}

void RecordViewModel::addRecord( const Record& record )
{
	ZoneScoped;
	beginInsertRows( {}, 0, 0 );

	records.insert( records.begin(), record );

	endInsertRows();
}
