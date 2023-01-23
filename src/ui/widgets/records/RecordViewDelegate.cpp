//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewDelegate.hpp"
#include "RecordViewModel.hpp"
#include "h95/config.hpp"

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>

#include <tracy/Tracy.hpp>

void RecordViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	ZoneScoped;
	painter->save();

	const auto data { index.data().value< ModelData >() };
	if ( data.record != nullptr )
	{
		const auto& record { *data.record };

		QFont font { option.font };
		QFontMetrics metrics { font };

		const auto banner_width {getSettings<int>("main_view/banner_width", 400)};
		const auto banner_height {getSettings<int>("main_view/banner_height", 300)};

		QPixmap banner { QString::fromStdString( record.m_banner.string() ) };
		banner = banner.scaledToHeight( banner_height - ( metrics.height() * 2 ), Qt::SmoothTransformation );
		if ( banner.width() > banner_width)
			banner = banner.scaledToWidth( banner_width, Qt::SmoothTransformation );

		const QRect pixmap_rect { option.rect.center(), QSize( banner.width(), banner.height() ) };
		painter->drawPixmap(
			pixmap_rect.translated( ( banner.width() / 2 ) * -1, ( ( banner.height() ) / 2 ) * -1 ),
			banner );

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
	ZoneScoped;
	return { getSettings<int>("main_view/item_width", 400), getSettings<int>("main_view/item_height", 300) };
}


bool RecordViewDelegate::editorEvent(
	QEvent* event,
	QAbstractItemModel* model,
	[[maybe_unused]] const QStyleOptionViewItem& option,
	const QModelIndex& index )
{
	ZoneScoped;
	if ( event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent* m_event { dynamic_cast< QMouseEvent* >( event ) };

		if ( m_event->button() == Qt::RightButton )
		{
			QMenu menu;

			menu.addAction( "Manage record", [=]() {} );

			menu.exec( m_event->pos() );

			return true;
		}
		else
			return false;
	}
	return false;
}
