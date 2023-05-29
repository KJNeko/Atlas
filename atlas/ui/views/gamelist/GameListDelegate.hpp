//
// Created by kj16609 on 5/25/23.
//

#ifndef ATLASGAMEMANAGER_GAMELISTDELEGATE_HPP
#define ATLASGAMEMANAGER_GAMELISTDELEGATE_HPP

#include <QAbstractItemDelegate>

class GameListDelegate final : public QAbstractItemDelegate
{
	Q_OBJECT

  public:

	void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
};

#endif //ATLASGAMEMANAGER_GAMELISTDELEGATE_HPP
