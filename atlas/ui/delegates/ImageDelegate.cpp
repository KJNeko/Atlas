//
// Created by kj16609 on 4/17/23.
//

#include "ImageDelegate.hpp"

#include <moc_ImageDelegate.cpp>

#include <QPainter>
#include <QFuture>

#include <tracy/Tracy.hpp>

#include <filesystem>
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"

#include "core/config.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageDelegate::paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const
{
	ZoneScoped;
	atlas::records::Game record { index.data().value< atlas::records::Game >() };

	
	const std::filesystem::path path { index.data( Qt::DisplayRole ).value< QString >().toStdString() };


	if ( std::filesystem::exists( path ) )
	{
		QPixmap pixmap { QString::fromStdString( path.string() ) };

		pixmap = pixmap.scaled( item.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

		//Reset the current brush
		painter->setBrush( Qt::NoBrush );
		QPen pen;
		//Used for border around the grid capsule
		pen.setBrush( Qt::transparent );
		pen.setWidth( 1 );
		pen.setStyle( Qt::SolidLine );
		painter->setPen( pen );

		if ( item.state & QStyle::State_Selected )
		{
			painter->fillRect( item.rect, item.palette.highlight() );
		}

		painter->drawPixmap( item.rect.center() - pixmap.rect().center(), pixmap );
	}
	else
	{
		painter->drawText( item.rect, Qt::AlignCenter, "Filepath does not exist" );
	}

	painter->drawRect( item.rect );
	
}

QSize ImageDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
}
