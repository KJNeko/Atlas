//
// Created by kj16609 on 3/29/23.
//

#include "RecordBannerDelegate.hpp"

#include <QMenu>
#include <QPainter>

#include "h95/config.hpp"
#include "h95/database/Record.hpp"

void RecordBannerDelegate::paint( QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index )
	const
{
	painter->save();

	//Draw banner if present
	const Record record { index.data().value< Record >() };
	const QPixmap banner { record->getBanner() };
	painter->drawPixmap( options.rect, banner );

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

	const QRect strip_rect { stripe_top_left, stripe_size };

	painter->fillRect( strip_rect, QColor( 0, 0, 0, 50 ) );

	//painter->drawText(); //engine
	painter->drawText( strip_rect, Qt::AlignCenter, record->getTitle() ); //Game name

	const auto latest { record->getLatestVersion() };
	if ( latest.has_value() )
		painter->drawText( strip_rect, Qt::AlignVCenter | Qt::AlignRight, latest.value().m_version ); //latest version
	else
		painter->drawText( strip_rect, Qt::AlignVCenter | Qt::AlignRight, "None" );


	painter->drawRect( options.rect );
	painter->restore();
}

QSize RecordBannerDelegate::sizeHint( const QStyleOptionViewItem &item, const QModelIndex &index ) const
{
	return { config::delegate::banner_x::get(), config::delegate::banner_y::get() };
}
