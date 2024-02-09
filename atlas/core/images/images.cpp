//
// Created by kj16609 on 3/2/23.
//

#include "images.hpp"

#include <QFile>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImageReader>
#include <QImageWriter>
#include <QPainter>
#include <QPixmap>

#include <tracy/Tracy.hpp>

#include <fstream>

#include "core/config/config.hpp"
#include "core/database/RapidTransaction.hpp"

namespace atlas::images
{
	void cleanOrphans()
	{
		ZoneScoped;
		atlas::logging::debug( "Clearing orphan previews/banners" );
		//Grab all images from the database
		RapidTransaction transaction {};

		for ( const auto& path : std::filesystem::directory_iterator( config::paths::images::getPath() ) )
		{
			if ( !path.is_regular_file() ) continue;

			bool found { false };
			transaction << "SELECT count(*) FROM images WHERE path = ?"
						<< std::filesystem::relative( path, config::paths::images::getPath() ).u8string()
				>> [ & ]( [[maybe_unused]] int count ) noexcept { found = true; };

			if ( !found ) std::filesystem::remove( path );
		}
	}

	QImage blurImage( const QImage& image, const qreal blur_radius, bool quality_blur, bool alpha_only, int transposed )
	{
		const QPixmap pixmap { QPixmap::fromImage( image ) };
		return blurPixmap( pixmap, blur_radius, quality_blur, alpha_only, transposed ).toImage();
	}

	QPixmap blurPixmap(
		const QPixmap& pixmap,
		qreal blur_radius,
		bool quality_blur,
		[[maybe_unused]] bool alpha_only,
		[[maybe_unused]] int transposed )
	{
		//TODO: Figure out if alpha_only and transposed are things we can use here
		QGraphicsScene scene;
		QGraphicsPixmapItem item;
		item.setPixmap( pixmap );
		scene.addItem( &item );
		QGraphicsBlurEffect* blur_effect { new QGraphicsBlurEffect };
		blur_effect->setBlurRadius( blur_radius );
		blur_effect
			->setBlurHints( quality_blur ? QGraphicsBlurEffect::QualityHint : QGraphicsBlurEffect::PerformanceHint );
		item.setGraphicsEffect( blur_effect );
		QPixmap result { pixmap.size() };
		result.fill( Qt::transparent );
		QPainter painter { &result };
		scene.render( &painter );
		return result;
	}

} // namespace atlas::images
