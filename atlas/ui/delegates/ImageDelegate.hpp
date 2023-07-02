//
// Created by kj16609 on 4/17/23.
//

#ifndef ATLASGAMEMANAGER_IMAGEDELEGATE_HPP
#define ATLASGAMEMANAGER_IMAGEDELEGATE_HPP

#include <QAbstractItemDelegate>

class ImageDelegate final : public QAbstractItemDelegate
{
	Q_OBJECT

	void paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
};

#endif //ATLASGAMEMANAGER_IMAGEDELEGATE_HPP
