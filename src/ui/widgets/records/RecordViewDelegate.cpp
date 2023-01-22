//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewDelegate.hpp"
#include "RecordViewModel.hpp"

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>

void RecordViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	painter->save();

	const auto data { index.data().value< ModelData >() };
	if ( data.record != nullptr )
	{
		const auto& record { *data.record };

		QPixmap banner { QString::fromStdString( record.m_banner.string() ) };
		painter->drawPixmap( option.rect, banner );

		painter->drawRect( option.rect );
		painter->drawText(
			option.rect,
			record.m_title + " [" + record.m_version + "]",
			Qt::AlignHCenter | Qt::AlignTop );
		painter->drawText(
			option.rect,
			"by " + record.m_creator + ( record.m_engine.isEmpty() ? "" : " in " + record.m_engine ),
			Qt::AlignHCenter | Qt::AlignBottom );
	}
	else
		qDebug() << "Record was nullptr when trying to paint";


	painter->restore();
}


QSize RecordViewDelegate::sizeHint(
	[[maybe_unused]] const QStyleOptionViewItem& option,
	[[maybe_unused]] const QModelIndex& index ) const
{
	return { 400, 200 };
}


bool RecordViewDelegate::editorEvent(
	QEvent* event,
	QAbstractItemModel* model,
	[[maybe_unused]] const QStyleOptionViewItem& option,
	const QModelIndex& index )
{
	if ( event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent* m_event { dynamic_cast< QMouseEvent* >( event ) };

		if ( m_event->button() == Qt::RightButton )
		{
			QMenu menu;

			menu.addAction( "Manage record", [=]() {} );

			menu.exec(m_event->pos());

			return true;
		}
		else
			return false;
	}
	return false;
}
