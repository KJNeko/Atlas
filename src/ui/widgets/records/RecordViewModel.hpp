//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEWMODEL_HPP
#define HYDRUS95_RECORDVIEWMODEL_HPP

#include <QAbstractListModel>
#include <QAbstractItemModel>

#include "h95/database/Record.hpp"

struct ModelData
{
	const Record* record { nullptr };
};

Q_DECLARE_METATYPE( ModelData )

class RecordViewModel : public QAbstractListModel
{
	std::vector< Record > records {};

	Q_DISABLE_COPY_MOVE( RecordViewModel )

	public:
	RecordViewModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) {}

	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void setRecords( const std::vector< Record >& records_in );
	void addRecord( const Record& record );
};


#endif	//HYDRUS95_RECORDVIEWMODEL_HPP
