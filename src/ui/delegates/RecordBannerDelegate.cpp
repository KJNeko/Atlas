//
// Created by kj16609 on 3/29/23.
//

#include "RecordBannerDelegate.hpp"

#include <QMenu>
#include <QPainter>

#include <tracy/Tracy.hpp>

#include "atlas/config.hpp"
#include "atlas/database/Record.hpp"

void RecordBannerDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index )
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

	const QRect strip_rect { stripe_top_left + QPoint( 0, 0 ), stripe_size };

	painter->fillRect( strip_rect, QColor( 0, 0, 0, 200 ) );

	//To manipulate text from going to far right or far left, we need to add padding before passing to drawText
	std::string engine_str = "  " + record->getEngine().toStdString();
	painter->setPen( qRgb( 210, 210, 210 ) );
	painter->drawText( strip_rect, Qt::AlignCenter, record->getTitle() ); //Game name
	painter->drawText( strip_rect, Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString( engine_str ) ); //Engine

	if ( record->getVersions().size() > 0 )
	{
		auto& latest { record->getLatestVersion() };
		std::string version_str = latest.getVersionName().toStdString() + "  ";
		painter->drawText(
			strip_rect, Qt::AlignVCenter | Qt::AlignRight, QString::fromStdString( version_str ) ); //latest version
	}
	else
		painter->drawText( strip_rect, Qt::AlignVCenter | Qt::AlignRight, "None" );

	painter->drawRect( options.rect );
	painter->restore();
}

QSize RecordBannerDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return { config::delegate::banner_x::get(), config::delegate::banner_y::get() };
}
