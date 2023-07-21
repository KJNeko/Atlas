//
// Created by kj16609 on 3/29/23.
//

#include "RecordBannerDelegate.hpp"

#include <moc_RecordBannerDelegate.cpp>

#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QPainter>
#include <QPixmapCache>

#include "core/config.hpp"
#include "core/database/Version.hpp"
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"
#include "core/utils/QImageBlur.hpp"
#include "ui/models/RecordListModel.hpp"

void RecordBannerDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index )
	const
{
	ZoneScoped;
	painter->save();

	//Draw banner if present
	Game record { index.data().value< Game >() };

	//draw test rect
	QRect test_rect { options.rect.x(), options.rect.y(), m_grid_size.width(), m_grid_size.height() };
	//painter->fillRect( test_rect, QColor( 0, 255, 0, 50 ) );
	//painter->drawRect( test_rect );

	const auto banner_size { m_banner_size };

	const SCALE_TYPE aspect_ratio { m_scale_type };
	const int stripe_height { m_strip_height };
	const int overlay_opacity { m_overlay_opacity };
	const bool enable_top_overlay { m_enable_top_overlay };
	const bool enable_bottom_overlay { m_enable_bottom_overlay };

	//For centering Items

	const int x_offset { options.rect.x() + ( ( m_grid_size.width() - m_banner_size.width() ) / 2 ) };
	const int y_offset { options.rect.y() + ( ( m_grid_size.height() - m_banner_size.height() ) / 2 ) };
	const QRect options_rect { x_offset, y_offset, banner_size.width(), banner_size.height() };

	const QRect shadow_rect { x_offset, y_offset, banner_size.width() + 10, banner_size.height() + 10 };

	{
		ZoneScopedN( "Draw banner" );

		const QString key { QString::fromStdString(
			fmt::format( "{}x{}:{}", options.rect.x(), options.rect.y(), record.bannerPath( Normal ) ) ) };

		QFuture< QPixmap > banner { record.requestBanner( banner_size, aspect_ratio, Normal ) };

		QPixmap pixmap;

		if ( !banner.isFinished() )
		{
			//We got the future
			//m_model can be nullptr in the settings menu. Since we don't have a model that is capable of doing this action. Instead we just have to wait like a good boy.
			if ( m_model != nullptr )
				this->m_model->refreshOnFuture( index, std::move( banner ) );
			else
				pixmap = banner.result();
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
		}

		//If the image needs to be centered then calculate it. because Qrect only does not take doubles, the center will not be exact.
		const int x_m { aspect_ratio == KEEP_ASPECT_RATIO ? ( banner_size.width() - pixmap.width() ) / 2 : 0 };
		const int y_m { aspect_ratio == KEEP_ASPECT_RATIO ? ( banner_size.height() - pixmap.height() ) / 2 : 0 };
		const QRect pixmap_rect { options_rect.x() + x_m, options_rect.y() + y_m, pixmap.width(), pixmap.height() };

		//Draw Shadow
		//painter->fillRect( shadow_rect, QColor( 255, 255, 255, 10 ) );
		//painter->drawRect( shadow_rect );
		//Draw Image
		painter->drawPixmap( pixmap_rect, pixmap );
	}

	//Click & Selectec event
	//TODO: add ability to change selected color.
	if ( options.state & QStyle::State_MouseOver )
	{
		painter->fillRect( options_rect, QColor( 0, 0, 255, 50 ) );
		// figure out where banner is and show pop up
		//const int popup_x {banner_size.width()};
		//const int popup_y {options_rect.y() + banner_size.height()/2};
		//const QRect popup_rect {0, 0, 100, 200 };
		//painter->fillRect(popup_rect, QColor(0,0,255,255));
	}

	//Reset the current brush
	painter->setBrush( Qt::NoBrush );
	QPen pen;
	//Used for border around the grid capsule
	pen.setBrush( m_enable_capsule_border ? Qt::black : Qt::transparent );
	pen.setWidth( 1 );
	pen.setStyle( Qt::SolidLine );
	painter->setPen( pen );
	painter->drawRect( options_rect );

	//Draw top
	const QRect top_rect { options_rect.topLeft().x(), options_rect.topLeft().y(), banner_size.width(), stripe_height };

	painter->fillRect( top_rect, QColor( 0, 0, 0, enable_top_overlay ? overlay_opacity : 0 ) );

	//Draw bottom
	const QRect bottom_rect { options_rect.bottomLeft().x(),
		                      options_rect.bottomLeft().y() - stripe_height + 1,
		                      banner_size.width(),
		                      stripe_height };

	painter->fillRect( bottom_rect, QColor( 0, 0, 0, enable_bottom_overlay ? overlay_opacity : 0 ) );

	//set Pen and Font for default text
	QFont font;
	font.setPixelSize( m_font_size );
	font.setFamily( m_font_family );
	painter->setFont( font );
	painter->setPen( qRgb( 210, 210, 210 ) );
	//TODO: Add so the user will be able to change the color. This is the default for all pallets

	//Draw Title
	this->drawText( painter, options_rect, stripe_height, m_title_location, record->m_title );
	//Draw Engine
	this->drawText( painter, options_rect, stripe_height, m_engine_location, record->m_engine );
	//Draw Version
	if ( record->m_versions.size() )
	{
		const Version latest { record->m_versions.at( 0 ) };
		this->drawText( painter, options_rect, stripe_height, m_version_location, latest->m_version );
	}
	else
		this->drawText( painter, options_rect, stripe_height, m_version_location, "No Version" );
	//Draw Creator
	this->drawText( painter, options_rect, stripe_height, m_creator_location, record->m_creator );

	painter->restore();
}

QSize RecordBannerDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return m_grid_size;
}

void RecordBannerDelegate::
	drawText( QPainter* painter, const QRect& rect, const int strip_size, const LOCATION location, const QString& str )
		const
{
	ZoneScoped;
	if ( location != NONE )
	{
		const QSize size { rect.width(), strip_size };

		switch ( location )
		{
			case TOP_LEFT:
				{
					const QRect text_rect { rect.topLeft() + QPoint( 10, 0 ), size };
					painter->drawText( text_rect, Qt::AlignLeft | Qt::AlignVCenter, str );
					return;
				}
			case TOP_CENTER:
				{
					const QRect text_rect { rect.topLeft(), size };
					painter->drawText( text_rect, Qt::AlignCenter, str );
					return;
				}
			case TOP_RIGHT:
				{
					const QRect text_rect { rect.topLeft() + QPoint( -10, 0 ), size };
					painter->drawText( text_rect, Qt::AlignRight | Qt::AlignVCenter, str );
					return;
				}
			case BOTTOM_LEFT:
				{
					const QRect text_rect { rect.bottomLeft() + QPoint( 10, -strip_size ), size };
					painter->drawText( text_rect, Qt::AlignLeft | Qt::AlignVCenter, str );
					return;
				}
			case BOTTOM_CENTER:
				{
					const QRect text_rect { rect.bottomLeft() + QPoint( 0, -strip_size ), size };
					painter->drawText( text_rect, Qt::AlignCenter, str );
					return;
				}
			case BOTTOM_RIGHT:
				{
					const QRect text_rect { rect.bottomLeft() + QPoint( -10, -strip_size ), size };
					painter->drawText( text_rect, Qt::AlignRight | Qt::AlignVCenter, str );
					return;
				}
			default:
				break;
		}
	}
}

void RecordBannerDelegate::reloadConfig()
{
	ZoneScoped;
	m_grid_size = calculateSize(
		config::grid_ui::itemViewWidth::get(),
		config::grid_ui::bannerSizeX::get(),
		config::grid_ui::bannerSizeY::get(),
		config::grid_ui::bannerSpacing::get() );
	m_scale_type = config::grid_ui::imageLayout::get();
	m_strip_height = config::grid_ui::overlayHeight::get();
	m_overlay_opacity = config::grid_ui::overlayOpacity::get();
	m_enable_top_overlay = config::grid_ui::enableTopOverlay::get();
	m_enable_bottom_overlay = config::grid_ui::enableBottomOverlay::get();
	m_feather_radius = config::grid_ui::featherRadius::get();
	m_blur_radius = config::grid_ui::blurRadius::get();
	m_blur_type = config::grid_ui::blurType::get();
	m_enable_capsule_border = config::grid_ui::enableCapsuleBorder::get();
	m_font_size = config::grid_ui::fontSize::get();
	m_font_family = config::grid_ui::font::get();
	m_title_location = config::grid_ui::titleLocation::get();
	m_engine_location = config::grid_ui::engineLocation::get();
	m_version_location = config::grid_ui::versionLocation::get();
	m_creator_location = config::grid_ui::creatorLocation::get();
	m_grid_spacing = config::grid_ui::bannerSpacing::get();
	m_banner_size = { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
	m_window_height = config::grid_ui::windowHeight::get();
	m_window_width = config::grid_ui::windowWidth::get();
	m_center_widgets = config::grid_ui::centerWidgets::get();
}

RecordBannerDelegate::RecordBannerDelegate( RecordListModel* model, QWidget* parent ) :
  QAbstractItemDelegate( parent ),
  m_grid_size { calculateSize(
	  config::grid_ui::itemViewWidth::get(),
	  config::grid_ui::bannerSizeX::get(),
	  config::grid_ui::bannerSizeY::get(),
	  config::grid_ui::bannerSpacing::get() ) },
  m_scale_type { config::grid_ui::imageLayout::get() },
  m_strip_height { config::grid_ui::overlayHeight::get() },
  m_overlay_opacity { config::grid_ui::overlayOpacity::get() },
  m_enable_top_overlay { config::grid_ui::enableTopOverlay::get() },
  m_enable_bottom_overlay { config::grid_ui::enableBottomOverlay::get() },
  m_feather_radius { config::grid_ui::featherRadius::get() },
  m_blur_radius { config::grid_ui::blurRadius::get() },
  m_blur_type { config::grid_ui::blurType::get() },
  m_enable_capsule_border { config::grid_ui::enableCapsuleBorder::get() },
  m_font_size { config::grid_ui::fontSize::get() },
  m_font_family { config::grid_ui::font::get() },
  m_title_location { config::grid_ui::titleLocation::get() },
  m_engine_location { config::grid_ui::engineLocation::get() },
  m_version_location { config::grid_ui::versionLocation::get() },
  m_creator_location { config::grid_ui::creatorLocation::get() },
  m_grid_spacing { config::grid_ui::bannerSpacing::get() },
  m_banner_size { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() },
  m_window_height { config::grid_ui::windowHeight::get() },
  m_window_width { config::grid_ui::windowWidth::get() },
  m_center_widgets { config::grid_ui::centerWidgets::get() },
  m_model( model )

{
	CONFIG_ATTACH_THIS;
}

QSize RecordBannerDelegate::
	calculateSize( const int window_width, const int banner_width, const int banner_height, const int banner_spacing )
{
	//spdlog::debug( "Window_width:{} | Banner_width:{} | Banner_height:{} | Spacing:{}", window_width, banner_width, banner_height, banner_spacing );
	ZoneScoped;

	int record_viewport = window_width;
	int item_count { static_cast< int >( record_viewport / static_cast< double >( banner_width ) ) };
	int banner_total_width { ( item_count ) * ( banner_width ) };
	double banner_offset { ( record_viewport - banner_total_width ) / static_cast< double >( item_count ) };
	int banner_viewport { ( static_cast< int >( banner_offset ) * item_count ) + banner_total_width };
	//Because Qt does not like floating points, we need to make sure the banner viewport is 1px less that what is available.
	if ( banner_viewport >= record_viewport )
	{
		banner_offset -= 1;
	}
	//spdlog::debug( "record_viewport:{} | banner_viewport:{} | item_count:{} | banner_offset:{}", record_viewport, banner_viewport, item_count, static_cast< int >( banner_offset ) );

	//USED TO CENTER WIDGETS
	QSize qsize { m_center_widgets ? banner_width + static_cast< int >( banner_offset ) : banner_width + banner_spacing,
		          banner_height + banner_spacing };

	return qsize;
}
