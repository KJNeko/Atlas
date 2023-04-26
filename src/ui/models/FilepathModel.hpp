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

  public slots:
	void setFilepaths( const std::vector< std::filesystem::path >& filepaths );
};

#endif //ATLASGAMEMANAGER_FILEPATHMODEL_HPP
