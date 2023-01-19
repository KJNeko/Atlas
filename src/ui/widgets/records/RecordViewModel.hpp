//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEWMODEL_HPP
#define HYDRUS95_RECORDVIEWMODEL_HPP

#include <QAbstractListModel>
#include <QAbstractItemModel>

#include "h95/database/Record.hpp"

class RecordViewModel : public QAbstractListModel
{
	std::vector< Record > records {};

	Q_DISABLE_COPY_MOVE( RecordViewModel )

	public:
	RecordViewModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) {}

	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	inline Record& at( const int index )
	{
		return records.at( static_cast< decltype( records )::size_type >( index ) );
	}
	inline void setRecords( const std::vector< Record >& records_in ) { records = records_in; }
	inline void addRecord( const Record& record ) { records.insert( records.begin(), record ); }
};


#endif	//HYDRUS95_RECORDVIEWMODEL_HPP
