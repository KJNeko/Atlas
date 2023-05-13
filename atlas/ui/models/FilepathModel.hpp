//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_FILEPATHMODEL_HPP
#define ATLASGAMEMANAGER_FILEPATHMODEL_HPP

#include <filesystem>

#include <QAbstractItemModel>

class FilepathModel final : public QAbstractListModel
{
	Q_OBJECT

	std::vector< std::filesystem::path > m_paths {};

  public:

	FilepathModel( QObject* parent = nullptr );

	QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
	QModelIndex parent( const QModelIndex& child ) const override;
	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	Qt::DropActions supportedDropActions() const override;

	bool insertRows( int row, int col, const QModelIndex& parent = QModelIndex() ) override;
	bool removeRows( int row, int count, const QModelIndex& parent = QModelIndex() ) override;
	bool moveRows(
		const QModelIndex& sourceParent,
		int sourceRow,
		int count,
		const QModelIndex& destinationParent,
		int destinationChild ) override;

	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

	Qt::ItemFlags flags( const QModelIndex& index ) const override;

	std::vector< std::filesystem::path > getFilepaths() const;

  public slots:
	void setFilepaths( const std::vector< std::filesystem::path >& filepaths );

  signals:
	void reordered();
};

#endif //ATLASGAMEMANAGER_FILEPATHMODEL_HPP
