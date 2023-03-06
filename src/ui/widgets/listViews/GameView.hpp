//
// Created by kj16609 on 3/2/23.
//

#ifndef HYDRUS95_GAMEVIEW_HPP
#define HYDRUS95_GAMEVIEW_HPP

#include <QListView>
#include <QAbstractListModel>
#include <QAbstractItemDelegate>

#include "h95/database/Record.hpp"

class GameViewModel final : public QAbstractListModel
{
	std::vector< Record > m_records {};

	public:
	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void setRecords( std::vector< Record > records );

	friend class TestGameView;
};

class GameViewDelegate final : public QAbstractItemDelegate
{
	public:
	void paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;

	bool editorEvent(
		QEvent* event,
		QAbstractItemModel* model,
		const QStyleOptionViewItem& option,
		const QModelIndex& index ) override;

	friend class TestGameView;
};

class GameView final : public QListView
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( GameView )

	public:
	GameView( QWidget* parent = nullptr );

	GameViewModel& model();
	GameViewDelegate& itemDelegate();

	public slots:
	void setRecords( std::vector< Record > records );

	friend class TestGameView;
};


#endif	//HYDRUS95_GAMEVIEW_HPP
