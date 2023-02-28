//
// Created by kj16609 on 2/25/23.
//

#ifndef HYDRUS95_GAMEEDITVERSIONMODEL_HPP
#define HYDRUS95_GAMEEDITVERSIONMODEL_HPP

#include <vector>

#include <QAbstractTableModel>

#include <h95/database/GameMetadata.hpp>

class GameEditVersionModel : public QAbstractItemModel
{
	Q_DISABLE_COPY_MOVE( GameEditVersionModel )

	std::vector< GameMetadata >& m_versions;

	public:
	GameEditVersionModel( std::vector< GameMetadata >& versions, QObject* parent = nullptr ) :
	  QAbstractItemModel( parent ),
	  m_versions( versions )
	{
	}

	QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
	QModelIndex parent( const QModelIndex& index ) const override;

	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

	void removeItem( std::size_t index );

	void addRow( GameMetadata metadata );
};


#endif	//HYDRUS95_GAMEEDITVERSIONMODEL_HPP
