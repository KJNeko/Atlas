//
// Created by kj16609 on 3/4/23.
//

#ifndef HYDRUS95_GAMEVIEWDELEGATE_HPP
#define HYDRUS95_GAMEVIEWDELEGATE_HPP

#include <QAbstractItemDelegate>

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


#endif	//HYDRUS95_GAMEVIEWDELEGATE_HPP
