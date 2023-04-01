//
// Created by kj16609 on 3/29/23.
//

#include "RecordBannerDelegate.hpp"

#include <QMenu>
#include <QPainter>

#include <tracy/Tracy.hpp>

#include "h95/config.hpp"
#include "h95/database/Record.hpp"

void RecordBannerDelegate::paint( QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index )
	const
{
	ZoneScoped;
	painter->save();

	//Draw banner if present
	const Record record { index.data().value< Record >() };

	const auto pixmap { record->getBanner( config::delegate::banner_x::get(), config::delegate::banner_y::get() ) };

	if ( !pixmap.isNull() )
	{
		const QRect pixmap_rect {
			options.rect.center() - QPoint( pixmap.width() / 2, pixmap.height() / 2 ) + QPoint( 2, 2 ), pixmap.size()
		};

		painter->drawPixmap( pixmap_rect, pixmap );
	}

	if ( options.state & QStyle::State_Selected ) painter->fillRect( options.rect, QColor( 0, 0, 255, 50 ) );

	//Draw game name
	//The bottom strip should be about (text_height * 1.2)
	const int stripe_height { static_cast< int >( options.fontMetrics.height() * 1.4 ) };
	//Create new rect for the stripe
	const auto bottom_left { options.rect.bottomLeft() };
	//Shift up by height
	const QPoint stripe_top_left { bottom_left.x(), bottom_left.y() - stripe_height };

	const auto diff { options.rect.bottomRight() - stripe_top_left };
	const QSize stripe_size { diff.x(), diff.y() };

	const QRect strip_rect { stripe_top_left + QPoint( 2, 2 ), stripe_size };

	painter->fillRect( strip_rect, QColor( 0, 0, 0, 200 ) );

	//painter->drawText(); //engine
	painter->drawText( strip_rect, Qt::AlignCenter, record->getTitle() ); //Game name
	painter->drawText( strip_rect, Qt::AlignLeft | Qt::AlignVCenter, record->getCreator() ); //Creator

	if ( record->getVersions().size() > 0 )
	{
		auto &latest { record->getLatestVersion() };
		painter->drawText( strip_rect, Qt::AlignVCenter | Qt::AlignRight, latest.getVersionName() ); //latest version
	}
	else
		painter->drawText( strip_rect, Qt::AlignVCenter | Qt::AlignRight, "None" );

	painter->drawRect( options.rect );
	painter->restore();
}

QSize RecordBannerDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem &item, [[maybe_unused]] const QModelIndex &index ) const
{
	ZoneScoped;
	return { config::delegate::banner_x::get(), config::delegate::banner_y::get() };
}
