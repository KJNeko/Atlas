//
// Created by kj16609 on 4/17/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_FILEPATHMODEL_HPP
#define ATLASGAMEMANAGER_FILEPATHMODEL_HPP

#include <QAbstractItemModel>
#include <QFuture>

#include <deque>
#include <filesystem>

#include "core/images/ImageLoader.hpp"

class FilepathModel final : public QAbstractListModel
{
	Q_OBJECT

	std::vector< std::filesystem::path > m_paths {};
	std::unordered_map< int, std::unique_ptr< atlas::images::ImageLoader > > loaders {};

  public:

	enum Roles
	{
		StartUserRoles = Qt::UserRole,
		Pixmap,
		Filepath
	};

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

	void refreshOnLoader( QPersistentModelIndex index, std::unique_ptr< atlas::images::ImageLoader > loader );

	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

	Qt::ItemFlags flags( const QModelIndex& index ) const override;

	std::vector< std::filesystem::path > getFilepaths() const;
	void setFilepaths( const std::vector< std::filesystem::path >& paths );

	void reloadRecord( QPersistentModelIndex index );

  signals:
	void reordered();
};

#endif //ATLASGAMEMANAGER_FILEPATHMODEL_HPP
