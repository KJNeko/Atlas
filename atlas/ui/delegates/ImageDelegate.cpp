//
// Created by kj16609 on 4/17/23.
//

#include "ImageDelegate.hpp"

#include <filesystem>

#include <QPainter>

#include "atlas/config.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageDelegate::paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const
{
	const std::filesystem::path path { index.data( Qt::DisplayRole ).value< QString >().toStdString() };

	if ( std::filesystem::exists( path ) )
	{
		QPixmap pixmap { QString::fromStdString( path.string() ) };

		pixmap = pixmap.scaled( item.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

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
	return { config::grid_ui::gridSizeX::get(), config::grid_ui::gridSizeY::get() };
}
