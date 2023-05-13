//
// Created by kj16609 on 4/14/23.
//

#include "QImageBlur.hpp"

#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmapCache>
#include <QWidget>

#include "core/config.hpp"

QPixmap blurPixmap( const QPixmap& pixmap, qreal radius, bool quality )
{
	QGraphicsScene scene;
	QGraphicsPixmapItem item;
	item.setPixmap( pixmap );
	scene.addItem( &item );
	QGraphicsBlurEffect* blur_effect { new QGraphicsBlurEffect };
	blur_effect->setBlurRadius( radius );
	blur_effect->setBlurHints( quality ? QGraphicsBlurEffect::QualityHint : QGraphicsBlurEffect::PerformanceHint );
	item.setGraphicsEffect( blur_effect );
	QPixmap result { pixmap.size() };
	result.fill( Qt::transparent );
	QPainter painter { &result };
	scene.render( &painter );
	return result;
}

QPixmap blurToSize(
	const QPixmap& pixmap,
	const int width,
	const int height,
	const int image_feather_radius,
	const int image_blur_radius,
	const BLUR_TYPE image_blur_type )
{
	if ( pixmap.isNull() ) return QPixmap();
	auto resized { pixmap.toImage().scaled( width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation ) };
	//const auto blurred_image { blurPixmap( pixmap, 25, false, false ) };

	const auto feather_radius { ( image_feather_radius / 1000.0 ) + .001 };

	const auto blur_center { pixmap.rect().center() };
	const auto blur_size { QSize { width, height } };
	const auto blur_x { blur_center.x() - blur_size.width() / 2 };
	const auto blur_y { blur_center.y() - blur_size.height() / 2 };
	auto blurred_image { pixmap.copy( blur_x, blur_y, blur_size.width(), blur_size.height() ) };

	const QRect total_rect { QRect { QPoint { 0, 0 }, QSize { width, height } } };
	const QRect img_rect { total_rect.center() - QPoint { resized.width() / 2, resized.height() / 2 }, resized.size() };
	const QRect blur_rect { total_rect.center() - QPoint { blurred_image.width() / 2, blurred_image.height() / 2 },
		                    blurred_image.size() };

	QImage dest { QPixmap( total_rect.size() ).toImage() };
	dest.fill( Qt::black );

	const QImage alpha_mask = [ & ]() -> QImage
	{
		//Create an alpha mask on the left and right sides of the image
		QImage gradient_img { dest.size(), QImage::Format_Alpha8 };

		gradient_img.fill( Qt::transparent );
		QPainter gradiant_painter { &gradient_img };

		if ( resized.width() <= width - 2 )
		{
			QLinearGradient grad { gradient_img.rect().topLeft(), gradient_img.rect().topRight() };
			grad.setColorAt( 0.00, Qt::transparent );
			grad.setColorAt( feather_radius, Qt::black );
			grad.setColorAt( 1 - feather_radius, Qt::black );
			grad.setColorAt( 1, Qt::transparent );
			gradiant_painter.fillRect( gradient_img.rect(), grad );
		}
		else if ( resized.height() <= height - 2 )
		{
			//Now alpha mask for top/bottom
			QLinearGradient grad { gradient_img.rect().topLeft(), gradient_img.rect().bottomLeft() };
			grad.setColorAt( 0.00, Qt::transparent );
			grad.setColorAt( feather_radius, Qt::black );
			grad.setColorAt( 1 - feather_radius, Qt::black );
			grad.setColorAt( 1, Qt::transparent );
			gradiant_painter.setCompositionMode( QPainter::CompositionMode_DestinationAtop );
			gradiant_painter.fillRect( gradient_img.rect(), grad );
		}
		else
			return QImage();

		return gradient_img;
	}();

	{
		QPainter painter { &dest };

		if ( !alpha_mask.isNull() )
		{
			painter.drawPixmap(
				total_rect.intersected( blur_rect ), blurPixmap( blurred_image, image_blur_radius, true ) );

			switch ( image_blur_type )
			{
				case FEATHER_IMAGE:
					resized.setAlphaChannel( alpha_mask );
					break;

				default:
					break;
			}
		}

		painter.drawImage( img_rect, std::move( resized ) );
	}

	return QPixmap::fromImage( dest );
}
