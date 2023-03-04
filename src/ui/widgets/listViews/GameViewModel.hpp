//
// Created by kj16609 on 3/4/23.
//

#ifndef HYDRUS95_GAMEVIEWMODEL_HPP
#define HYDRUS95_GAMEVIEWMODEL_HPP

#include <QAbstractListModel>

#include <h95/database/Record.hpp>

class GameViewModel final : public QAbstractListModel
{
	std::vector< Record > m_records {};

	public:
	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void setRecords( std::vector< Record > records );

	friend class TestGameView;
};


#endif	//HYDRUS95_GAMEVIEWMODEL_HPP
