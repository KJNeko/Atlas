//
// Created by kj16609 on 4/17/23.
//

#include "ImageDelegate.hpp"

#include <moc_ImageDelegate.cpp>

#include <QFuture>
#include <QPainter>

#include <tracy/Tracy.hpp>

#include "core/config/config.hpp"
#include "core/images/thumbnails.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageDelegate::paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const
{
	ZoneScoped;
	QFuture< QPixmap > future { index.data( FilepathModel::PixmapRole ).value< QFuture< QPixmap > >() };

	if ( item.state & QStyle::State_Selected )
	{
		painter->fillRect( item.rect, item.palette.highlight() );
	}

	if ( future.isFinished() )
	{
		const auto& pixmap { future.result() };
		//Reset the current brush
		painter->setBrush( Qt::NoBrush );
		QPen pen;
		//Used for border around the grid capsule
		pen.setBrush( Qt::transparent );
		pen.setWidth( 1 );
		pen.setStyle( Qt::SolidLine );
		painter->setPen( pen );

		painter->drawPixmap( item.rect.center() - pixmap.rect().center(), pixmap );
	}
	else
	{
		m_model->refreshOnFuture( index, future );

		const auto pixmap {
			atlas::images::thumbnail( index.data( FilepathModel::FilepathRole ).value< std::filesystem::path >() )
		};

		painter->drawPixmap( item.rect.center() - pixmap.rect().center(), pixmap );
	}

	painter->drawRect( item.rect );
}

QSize ImageDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
}
