//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEWDELEGATE_HPP
#define HYDRUS95_RECORDVIEWDELEGATE_HPP

#include <QAbstractItemDelegate>

class RecordViewDelegate final : public QAbstractItemDelegate
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( RecordViewDelegate )

	signals:
	void requestRefreshRecords();

	public:
	RecordViewDelegate( QObject* parent = nullptr ) : QAbstractItemDelegate( parent ) {}

	void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
	bool editorEvent(
		QEvent* event,
		QAbstractItemModel* model,
		const QStyleOptionViewItem& option,
		const QModelIndex& index ) override;
};


#endif	//HYDRUS95_RECORDVIEWDELEGATE_HPP
