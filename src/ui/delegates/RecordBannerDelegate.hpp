//
// Created by kj16609 on 3/29/23.
//

#ifndef HYDRUS95_RECORDBANNERDELEGATE_HPP
#define HYDRUS95_RECORDBANNERDELEGATE_HPP

#include <QAbstractItemDelegate>

class RecordBannerDelegate : public QAbstractItemDelegate
{
	void paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
};

#endif //HYDRUS95_RECORDBANNERDELEGATE_HPP
