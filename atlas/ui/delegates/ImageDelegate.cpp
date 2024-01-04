//
// Created by kj16609 on 4/17/23.
//

#include "ImageDelegate.hpp"

#include <moc_ImageDelegate.cpp>

#include <QFuture>
#include <QPainter>

#include <tracy/Tracy.hpp>

#include "core/config/config.hpp"
#include "ui/models/FilepathModel.hpp"

void ImageDelegate::paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const
{
	ZoneScoped;
	QFuture< QPixmap > future { index.data( FilepathModel::Pixmap ).value< QFuture< QPixmap > >() };

	QPixmap banner { index.data( FilepathModel::Roles::Pixmap ).value< QPixmap >() };

	if ( banner.isNull() )
	{
		//Request it be loaded from the model
		std::unique_ptr< atlas::images::ImageLoader > image_loader {
			atlas::images::ImageLoader::loadPixmap( index.data( FilepathModel::Roles::Filepath )
			                                            .value< std::filesystem::path >() )
		};

		image_loader->scaleTo( item.rect.size(), SCALE_TYPE::FIT_BLUR_EXPANDING, Alignment::CENTER );

		if ( m_model != nullptr ) m_model->refreshOnLoader( index, std::move( image_loader ) );
	}

	if ( item.state & QStyle::State_Selected )
	{
		painter->fillRect( item.rect, item.palette.highlight() );
	}

	//If the pixmap is already loaded, draw it
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
	{}

	painter->drawRect( item.rect );
}

QSize ImageDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return { config::ui::bannerSizeX::get(), config::ui::bannerSizeY::get() };
}
