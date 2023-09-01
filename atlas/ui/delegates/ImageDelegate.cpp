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

	spdlog::info( "col:{}",index.column() );
	spdlog::info( "row:{}", index.row() );
	//const std::filesystem::path path { index.data( Qt::DisplayRole ).value< QString >().toStdString() };

	QtFuture::makeReadyFuture( record.requestPreviewIndex( index.row()) );
	//QFuture< QPixmap > preview{ record.requestPreviewIndex( index.row())};
	//QFuture< QPixmap > banner { record.requestBanner( item.rect.size(), Qt::KeepAspectRatio, Normal ) };

		QPixmap pixmap;

		/*if ( !banner.isFinished() )
		{
			//We got the future
			//m_model can be nullptr in the settings menu. Since we don't have a model that is capable of doing this action. Instead we just have to wait like a good boy.
			if ( m_model != nullptr )
				this->m_model->refreshOnFuture( index, std::move( banner ) );
			
			//Specific case. Do not load thumb for settings images
			if(record->m_game_id == 1)
			{
				pixmap = banner.result();
			}
			else{
			//Add experimental feature
				if(config::experimental::loading_preview::get())
				{
					pixmap = record.requestThumbnail( banner_size, Normal );
					QImage srcImg { pixmap.toImage() };
					pixmap.fill( Qt::transparent );
					{
						QPainter paintert( &pixmap );
						qt_blurImage( &paintert, srcImg, 100, true, false ); //blur radius
					}
				}
			}
		}
		else
		{
			ZoneScopedN( "Get image from variant" );
			//We got the banner and should continue as normal			
			pixmap = banner.result();

			//Check if we need to add blur background. Draw behind original image
			if ( aspect_ratio == FIT_BLUR_EXPANDING )
			{
				ZoneScopedN( "Blur image" );
				pixmap = blurToSize(
					pixmap, banner_size.width(), banner_size.height(), m_feather_radius, m_blur_radius, m_blur_type );
			}
		}*/
/*
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
	*/
}

QSize ImageDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
}
