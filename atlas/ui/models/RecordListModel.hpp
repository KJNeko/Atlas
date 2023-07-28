//
// Created by kj16609 on 3/15/23.
//

#ifndef ATLAS_RECORDLISTMODEL_HPP
#define ATLAS_RECORDLISTMODEL_HPP

#include <iostream>

#include <QAbstractListModel>
#include <QFutureWatcher>

#include "core/database/record/Game.hpp"

struct ImageLoader final : public QObject
{
	Q_OBJECT

	QPersistentModelIndex m_index;
	QFuture< QPixmap > m_future;
	QFutureWatcher< QPixmap > watcher {};

  public:

	ImageLoader( QPersistentModelIndex index, const QFuture< QPixmap > future );

	void triggerReady();

	~ImageLoader() { m_future.cancel(); }

  signals:
	void imageReady( QPersistentModelIndex index );
};

class RecordListModel final : public QAbstractListModel
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( RecordListModel )

	std::vector< atlas::records::Game > m_records {};

	//Map of active loading images
	QThread loading_thread {};
	std::unordered_map< int, ImageLoader* > loaders {};

  public:

	enum RecordListModelRoles
	{
		Raw = Qt::ItemDataRole::UserRole,
		NormalBanner,
		WideBanner,
		LogoBanner,
		CoverBanner,
	};

	RecordListModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) { loading_thread.start(); }

	int rowCount( const QModelIndex& index = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void refreshOnFuture( QPersistentModelIndex index, QFuture< QPixmap > );
	void killLoaders();

  public slots:
	void setRecords( std::vector< atlas::records::Game > records );

	/**
	 * @param record
	 * @param place_at where to place in the list (place_at > m_records.size()) will place at the end
	 */
	void addRecord( atlas::records::Game record, const std::size_t place_at = 0 );

	void removeRecord( QPersistentModelIndex index );

	void reloadRecord( QPersistentModelIndex index );

  signals:
	void recordsChanged( std::vector< atlas::records::Game > records );
	void emitReloadRecord( QPersistentModelIndex index );
};

#endif //ATLAS_RECORDLISTMODEL_HPP
