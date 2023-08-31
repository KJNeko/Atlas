//
// Created by kj16609 on 3/26/23.
//

#ifndef ATLAS_BATCHIMPORTMODEL_HPP
#define ATLAS_BATCHIMPORTMODEL_HPP

#include <QAbstractTableModel>

#include <filesystem>

#include "core/import/GameImportData.hpp"

enum ImportColumns
{
	TITLE,
	CREATOR,
	ENGINE,
	VERSION,
	EXECUTABLES,
	SIZE,
	FOLDER_PATH,
	HAS_GL_LINK
};

class BatchImportModel final : public QAbstractTableModel
{
	std::vector< GameImportData > m_data {};

  public:

	const std::vector< GameImportData >& getData() const { return m_data; }

	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	int columnCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
	QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
	Qt::ItemFlags flags( const QModelIndex& index ) const override;
	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;
	void sort( int idx, Qt::SortOrder order = Qt::AscendingOrder ) override;

	void clearData();

  public slots:
	void addGame( GameImportData data );
	void addGames( std::vector< GameImportData > data );

	friend class BatchImportDelegate;
};

#endif //ATLAS_BATCHIMPORTMODEL_HPP
