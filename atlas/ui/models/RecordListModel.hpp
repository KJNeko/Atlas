//
// Created by kj16609 on 3/15/23.
//

#pragma once
#ifndef ATLAS_RECORDLISTMODEL_HPP
#define ATLAS_RECORDLISTMODEL_HPP

#include <QAbstractListModel>
#include <QFutureWatcher>

#include <iostream>

#include "core/database/record/game/Game.hpp"
#include "core/images/ImageLoader.hpp"

class RecordListModel final : public QAbstractListModel
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( RecordListModel )

	std::vector< atlas::records::Game > m_records {};

	//Map of active loading images
	QThread loading_thread {};
	std::unordered_map< int, std::unique_ptr< atlas::images::ImageLoader > > loaders {};

  public:

	enum Roles
	{
		Raw = Qt::ItemDataRole::UserRole,
		Banner,
	};

	RecordListModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) { loading_thread.start(); }

	QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
	int rowCount( const QModelIndex& index = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void refreshOnLoader( QPersistentModelIndex index, std::unique_ptr< atlas::images::ImageLoader > loader );
	void killOutOfView( const int first, const int last );

  public slots:
	void setRecords( std::vector< atlas::records::Game > records );

	/**
	 * @param record
	 * @param place_at where to place in the list (place_at > m_records.size()) will place at the end
	 */
	void addRecord( atlas::records::Game record, const std::size_t place_at = 0 );

	void removeRecord( QPersistentModelIndex index );

	void reloadRecord( QPersistentModelIndex index );

	void killLoaders();

  signals:
	void recordsChanged( std::vector< atlas::records::Game > records );
	void emitReloadRecord( QPersistentModelIndex index );
};

#endif //ATLAS_RECORDLISTMODEL_HPP
