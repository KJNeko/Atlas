//
// Created by kj16609 on 3/15/23.
//

#ifndef ATLAS_RECORDLISTMODEL_HPP
#define ATLAS_RECORDLISTMODEL_HPP

#include <QAbstractListModel>

#include "core/database/record/Record.hpp"

class RecordListModel : public QAbstractListModel
{
	Q_OBJECT

	std::vector< Record > m_records {};

  public:

	RecordListModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) {}

	int rowCount( const QModelIndex& index = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

  public slots:
	void setRecords( std::vector< Record > records );

	/**
	 * @param record
	 * @param place_at where to place in the list (place_at > m_records.size()) will place at the end
	 */
	void addRecord( Record record, const std::size_t place_at = 0 );

	void removeRecord( QPersistentModelIndex index );

  signals:
	void recordsChanged( std::vector< Record > records );
};

#endif //ATLAS_RECORDLISTMODEL_HPP
