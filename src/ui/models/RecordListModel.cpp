//
// Created by kj16609 on 3/15/23.
//

#include "RecordListModel.hpp"

#include <tracy/Tracy.hpp>

void RecordListModel::setRecords( std::vector< Record > records )
{
	ZoneScoped;
	beginResetModel();
	m_records = std::move( records );
	endResetModel();
	emit recordsChanged( m_records );
}

void RecordListModel::addRecord( Record& record, const std::size_t place_at )
{
	ZoneScoped;
	const int pos { static_cast< int >( std::min( place_at, m_records.size() ) ) };
	beginInsertRows( {}, pos, pos );
	m_records.insert( m_records.begin() + static_cast< int >( pos ), record );
	endInsertRows();
	emit recordsChanged( m_records );
}

void RecordListModel::removeRecord( QPersistentModelIndex index )
{
	ZoneScoped;
	if ( !index.isValid() )
		throw std::runtime_error( "RecordListModel::removeRecord(QPersistentModelIndex): index is not valid" );

	beginRemoveRows( {}, index.row(), index.row() );
	auto itter { m_records.begin() + index.row() };
	m_records.erase( itter );
	endRemoveRows();
	emit recordsChanged( m_records );
}
