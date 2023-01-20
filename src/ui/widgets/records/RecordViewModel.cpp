//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewModel.hpp"
QVariant RecordViewModel::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		const ModelData data { &(records.at( static_cast< unsigned long >( index.row() ) )) };
		return QVariant::fromValue( data );
	}

	return {};
}

int RecordViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( records.size() );
}

void RecordViewModel::setRecords( const std::vector< Record >& records_in )
{
	beginResetModel();

	records = records_in;

	endResetModel();
}

void RecordViewModel::addRecord( const Record& record )
{
	beginInsertRows( {}, 0, 0 );

	records.insert( records.begin(), record );

	endInsertRows();
}
