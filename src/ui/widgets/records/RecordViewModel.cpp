//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewModel.hpp"
QVariant RecordViewModel::data( const QModelIndex& index, int role ) const
{
	if(role == Qt::DisplayRole)
		return QVariant::fromValue(&records.at(static_cast<decltype(records)::size_type>(index.row())));

	return {};
}

int RecordViewModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast<int>(records.size());
}
