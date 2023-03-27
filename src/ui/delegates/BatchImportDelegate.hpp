//
// Created by kj16609 on 3/27/23.
//

#ifndef HYDRUS95_BATCHIMPORTDELEGATE_HPP
#define HYDRUS95_BATCHIMPORTDELEGATE_HPP

#include <QAbstractItemDelegate>
#include <QSize>

class BatchImportDelegate final : public QAbstractItemDelegate
{
	void paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& options, const QModelIndex& index )
		const override;
	void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const override;
	void setEditorData( QWidget* editor, const QModelIndex& index ) const override;
};

#endif //HYDRUS95_BATCHIMPORTDELEGATE_HPP
