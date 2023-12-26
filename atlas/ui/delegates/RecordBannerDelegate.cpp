//
// Created by kj16609 on 3/29/23.
//

#include "RecordBannerDelegate.hpp"

#include <moc_RecordBannerDelegate.cpp>

#include <QPainter>
#include <QPainterPath>

#include <tracy/Tracy.hpp>

#include "core/config/config.hpp"
#include "core/database/record/GameData.hpp"
#include "core/database/record/Version.hpp"
#include "core/database/record/game/Game.hpp"
#include "core/images/blurhash.hpp"
#include "core/images/images.hpp"
#include "core/utils/QImageBlur.hpp"
#include "ui/models/RecordListModel.hpp"

void RecordBannerDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index )
	const
{
	ZoneScoped;
	painter->save();

	//Get current record based on index
	atlas::records::Game record { index.data().value< atlas::records::Game >() };
	//Set all VARS prio to painting
	const auto banner_size { m_banner_size };
	const SCALE_TYPE aspect_ratio { m_scale_type };
	//TODO: add strip height for both top and bottom. Right now they share.
	const int top_overlay_height { m_top_overlay_height };
	const int bottom_overlay_height( m_bottom_overlay_height );
	//const int overlay_opacity { m_overlay_opacity };
	const bool enable_top_overlay { m_enable_top_overlay };
	const bool enable_bottom_overlay { m_enable_bottom_overlay };

	//For centering Items
	const int x_offset { options.rect.x() + ( ( m_grid_size.width() - m_banner_size.width() ) / 2 ) };
	const int y_offset { options.rect.y() + ( ( m_grid_size.height() - m_banner_size.height() ) / 2 ) };
	//For drop shadow
	const int x_shadow { 0 };
	const int y_shadow { 0 };
	//Set all QRect here
	//Main QRect to hold everything
	const QRect options_rect { x_offset, y_offset, banner_size.width() + x_shadow, banner_size.height() + y_shadow };
	//Top overlay QRect
	const QRect top_rect {
		options_rect.topLeft().x(), options_rect.topLeft().y(), banner_size.width(), top_overlay_height
	};
	//Bottom overlay QRect
	const QRect bottom_rect { options_rect.bottomLeft().x(),
		                      options_rect.bottomLeft().y() - bottom_overlay_height + 1,
		                      banner_size.width(),
		                      bottom_overlay_height };

	//We need to set the correct banner size based on if the overlay is in-line or on-top of the image. If
	//overlay is disabled then dont use it int he calculation
	const int xtop_overlay_margin { m_overlay_layout == ON_TOP    ? 0 :
		                            m_enable_top_overlay == false ? 0 :
		                                                            top_rect.height() };
	const int xbottom_overlay_margin { m_overlay_layout == ON_TOP       ? 0 :
		                               m_enable_bottom_overlay == false ? 0 :
		                                                                  bottom_rect.height() };
	QSize c_banner_size { banner_size.width(), banner_size.height() - xtop_overlay_margin - xbottom_overlay_margin };

	if ( record.hasBanner( Normal ) )
	{
		QFuture< QPixmap > banner { record.requestBanner( c_banner_size, aspect_ratio, CENTER, Normal, USE_FULLSIZE ) };

		QPixmap pixmap;
		if ( banner.isFinished() )
		{
			pixmap = banner.result();

			//Check if we need to add blur background. Draw behind original image
			if ( aspect_ratio == FIT_BLUR_EXPANDING )
			{
				ZoneScopedN( "Blur image" );
				pixmap = blurToSize(
					pixmap,
					c_banner_size.width(),
					c_banner_size.height(),
					m_feather_radius,
					m_blur_radius,
					m_blur_type );
			}
		}
		else
		{
			//m_model can be nullptr in the settings menu. Since we don't have a model that is capable of doing this action. Instead we just have to wait like a good boy.
			if ( m_model != nullptr ) this->m_model->refreshOnFuture( index, std::move( banner ) );

			//Specific case. Do not load thumb for settings images
			if ( record->m_game_id == 1 )
			{
				pixmap = banner.result();
			}
			else
			{
				if ( config::experimental::use_blurhash::get() )
					pixmap = atlas::images::getBlurhash( record.bannerPath( Normal ), c_banner_size );
				else
					pixmap = {};
			}
		}

		//If the image needs to be centered then calculate it. because Qrect only does not take doubles, the center will not be exact.
		const int x_m { aspect_ratio == KEEP_ASPECT_RATIO ? ( c_banner_size.width() - pixmap.width() ) / 2 : 0 };
		const int y_m { aspect_ratio == KEEP_ASPECT_RATIO ? ( c_banner_size.height() - pixmap.height() ) / 2 : 0 };
		//Check if overlay is on top or in-line with banner
		const QRect pixmap_rect {
			options_rect.x() + x_m, options_rect.y() + y_m + xtop_overlay_margin, pixmap.width(), pixmap.height()
		};

		painter->drawPixmap( pixmap_rect, pixmap );
	}

	//Reset the current brush
	painter->setBrush( Qt::NoBrush );
	QPen pen;

	//Draw Overlay
	painter->fillRect( top_rect, enable_top_overlay ? m_top_overlay_color : "transparent" );
	painter->fillRect( bottom_rect, enable_bottom_overlay ? m_bottom_overlay_color : "transparent" );

	//Used for border around the grid capsule
	pen.setBrush( m_enable_capsule_border ? Qt::black : Qt::transparent );
	pen.setWidth( 1 );
	pen.setStyle( Qt::SolidLine );
	painter->setPen( pen );
	painter->drawRect( options_rect );

	//set Pen and Font for default text
	QFont font;
	font.setFamily( m_font_family );
	font.setPixelSize( m_font_size );
	painter->setFont( font );
	painter->setPen( qRgb( 210, 210, 210 ) );

	//All text items need to be stored in a QRect. This makes it easier to detect edge cases and link items together
	//Draw all linkable items first
	QRect m_title_rect;
	QRect m_creator_rect;
	QRect m_version_rect;
	QRect m_engine_rect;
	QRect m_status_rect;
	QRect m_gametype_rect;

	QRect m_rating_rect;
	//ONLY DRAW ITEMS BELOW IF ATLAS DATA IS AVAILABLE
	//If Record == 1 continue: base record
	if ( record->atlas_data.has_value() || record->m_game_id == 1 )
	{
		//Draw Game Status
		if ( m_status_enable )
		{
			m_status_rect = this->drawText(
				painter,
				m_status_x,
				m_status_y,
				options_rect,
				record->m_game_id == 1 ? "Completed" : record->atlas_data.value()->status,
				m_font_size,
				m_font_family,
				m_font_bold ? QFont::Bold : QFont::Normal,
				m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
				m_font_shadow,
				m_padding,
				m_corner_radius,
				m_status_align,
				m_status_link,
				m_status_default ?
					getStatusColor( record->m_game_id == 1 ? "Completed" : record->atlas_data.value()->status ) :
					m_status_bcolor,
				m_status_default ? qRgb( 210, 210, 210 ) : m_status_fcolor,
				"" );
		}
		//Draw Game Type
		if ( m_gametype_enable )
		{
			m_gametype_rect = this->drawText(
				painter,
				m_gametype_x,
				m_gametype_y,
				options_rect,
				record->m_game_id == 1 ? "VN" : record->atlas_data.value()->category,
				m_font_size,
				m_font_family,
				m_font_bold ? QFont::Bold : QFont::Normal,
				m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
				m_font_shadow,
				m_padding,
				m_corner_radius,
				m_gametype_align,
				m_gametype_link,
				m_gametype_default ?
					getGameTypeColor( record->m_game_id == 1 ? "VN" : record->atlas_data.value()->category ) :
					m_gametype_bcolor,
				m_gametype_default ? qRgb( 210, 210, 210 ) : m_gametype_fcolor,
				"" );
		}
	}
	//These are f95 data Specific items
	if ( record->f95_data.has_value() || record->m_game_id == 1 )
	{
		//Rating
		if ( m_rating_enable )
		{
			m_rating_rect = this->drawText(
				painter,
				m_rating_x,
				m_rating_y,
				options_rect,
				record->m_game_id == 1 ? "5.0" : record->f95_data.value()->rating,
				m_font_size,
				m_font_family,
				m_font_bold ? QFont::Bold : QFont::Normal,
				m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
				m_font_shadow,
				m_padding,
				m_corner_radius,
				m_rating_align,
				m_rating_link,
				m_rating_default ? "transparent" : m_rating_bcolor,
				m_rating_default ? qRgb( 210, 210, 210 ) : m_rating_fcolor,
				":/images/assets/rating_icon_blank.svg" );
		}
		if ( m_views_enable )
		{
			this->drawText(
				painter,
				m_views_x,
				m_views_y,
				options_rect,
				record->m_game_id == 1 ? "100" : record->f95_data.value()->views,
				m_font_size,
				m_font_family,
				m_font_bold ? QFont::Bold : QFont::Normal,
				m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
				m_font_shadow,
				m_padding,
				m_corner_radius,
				m_views_align,
				m_views_link,
				m_views_default ? "transparent" : m_views_bcolor,
				m_views_default ? qRgb( 210, 210, 210 ) : m_views_fcolor,
				":/images/assets/views_icon.svg" );
		}
		if ( m_likes_enable )
		{
			this->drawText(
				painter,
				m_likes_x,
				m_likes_y,
				options_rect,
				record->m_game_id == 1 ? "150" : record->f95_data.value()->likes,
				m_font_size,
				m_font_family,
				m_font_bold ? QFont::Bold : QFont::Normal,
				m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
				m_font_shadow,
				m_padding,
				m_corner_radius,
				m_likes_align,
				m_likes_link,
				m_likes_default ? "transparent" : m_likes_bcolor,
				m_likes_default ? qRgb( 210, 210, 210 ) : m_likes_fcolor,
				":/images/assets/likes_white.svg" );
		}
	}
	//Draw Title
	if ( m_title_enable )
	{
		this->drawText(
			painter,
			m_title_x,
			m_title_y,
			options_rect,
			record->atlas_data.has_value() ? record->atlas_data.value()->title : record->m_title,
			m_title_fontsize,
			m_font_family,
			m_font_bold ? QFont::Bold : QFont::Normal,
			m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
			m_font_shadow,
			m_padding,
			m_corner_radius,
			m_title_align,
			m_title_link,
			m_title_default ? "transparent" : m_title_bcolor,
			m_title_default ? qRgb( 210, 210, 210 ) : m_title_fcolor,
			"" );
	}
	//Draw Engine : Use default font
	if ( m_engine_enable )
	{
		this->drawText(
			painter,
			m_engine_x,
			m_engine_y,
			options_rect,
			record->atlas_data.has_value() ? record->atlas_data.value()->engine : record->m_engine,
			m_font_size,
			m_font_family,
			m_font_bold ? QFont::Bold : QFont::Normal,
			m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
			m_font_shadow,
			m_padding,
			m_corner_radius,
			m_engine_align,
			m_engine_link,
			m_engine_default ? getEngineColor( record->m_engine ) : m_engine_bcolor,
			m_engine_default ? qRgb( 210, 210, 210 ) : m_engine_fcolor,
			"" );
	}
	//Draw Version : Use default font
	if ( record->m_versions.size() && m_version_enable )
	{
		const atlas::records::Version latest { record->m_versions.at( 0 ) };
		this->drawText(
			painter,
			m_version_x,
			m_version_y,
			options_rect,
			latest->m_version,
			m_font_size,
			m_font_family,
			m_font_bold ? QFont::Bold : QFont::Normal,
			m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
			m_font_shadow,
			m_padding,
			m_corner_radius,
			m_version_align,
			m_version_link,
			m_version_default ? "transparent" : m_version_bcolor,
			m_version_default ? qRgb( 210, 210, 210 ) : m_version_fcolor,
			"" );
	}

	if ( m_creator_enable )
	{
		this->drawText(
			painter,
			m_creator_x,
			m_creator_y,
			options_rect,
			record->atlas_data.has_value() ? record->atlas_data.value()->creator : record->m_creator,
			m_font_size,
			m_font_family,
			m_font_bold ? QFont::Bold : QFont::Normal,
			m_font_italic ? QFont::StyleItalic : QFont::StyleNormal,
			m_font_shadow,
			m_padding,
			m_corner_radius,
			m_creator_align,
			m_creator_link,
			m_creator_default ? "transparent" : m_creator_bcolor,
			m_creator_default ? qRgb( 210, 210, 210 ) : m_creator_fcolor,
			"" );
	}
	if ( m_favorite_enable )
	{
		this->drawIcon(
			painter,
			m_favorite_x,
			m_favorite_y,
			options_rect,
			20,
			m_corner_radius,
			m_favorite_align,
			":/images/assets/fav_icon.svg" );
	}

	//Set hover color. Must be drawn last
	if ( options.state & QStyle::State_MouseOver )
	{
		painter->fillRect( options_rect, QColor( 0, 0, 255, 50 ) );
	}

	painter->restore();
}

QSize RecordBannerDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	ZoneScoped;
	return m_grid_size;
}

//painter, text x loc, text y loc, rect of relative banner, string
QRect RecordBannerDelegate::drawText(
	QPainter* painter,
	const int x,
	const int y,
	const QRect rect,
	const QString& str,
	const int font_size,
	const QString font_family,
	const QFont::Weight font_weight,
	const QFont::Style font_style,
	const bool text_shadow,
	const int padding,
	const int corner_radius,
	const int align,
	const int link,
	QColor backgroundColor,
	QColor foregroundColor,
	QString pixmap_str ) const
{
	//Calculate rect size for text
	QFont font;
	font.setPixelSize( font_size );
	font.setFamily( font_family );
	font.setWeight( font_weight );
	QFontMetrics fm( font );
	int t_width = fm.horizontalAdvance( str ) + padding;
	int t_height = fm.height() + padding;
	int icon_size = fm.height();
	int icon_margin = 4;
	//The align overload picks which side of the rect to draw from
	int align_overload = align == LEFT ? 0 : align == RIGHT ? -t_width : -t_width / 2;

	painter->setFont( font ); //Override default
	painter->setRenderHint( QPainter::Antialiasing ); //Set so pixels look better

	//Create rec with 10px margin, rect is relative to current banner rect
	QRect text_rect { rect.topLeft() + QPoint( x + align_overload, y ), QSize( t_width, t_height ) };
	//To print color behind image
	QRect text_rect_ext { rect.topLeft() + QPoint( x + align_overload - icon_size - icon_margin, y ),
		                  QSize( t_width + icon_size + icon_margin, t_height ) };
	//QPainterPath path;
	//Draw Bounding Rect
	painter->setPen( QPen( backgroundColor ) ); //no pen
	painter->setBrush( QBrush( backgroundColor ) );
	painter->drawRoundedRect( pixmap_str != "" ? text_rect_ext : text_rect, corner_radius, corner_radius );
	//Text shadow
	if ( text_shadow )
	{
		painter->setRenderHint( QPainter::Antialiasing );
		painter->setPen( Qt::black );
		painter->drawText( text_rect.adjusted( 1, 1, 1, 1 ), Qt::AlignHCenter | Qt::AlignVCenter, str );
	}
	//Reset pet to draw text
	painter->setPen( foregroundColor );
	//Alight text in center of Rect
	painter->drawText( text_rect, Qt::AlignHCenter | Qt::AlignVCenter, str );

	//If an image is available then draw it. TODO: NEED TO FIX THIS SO ITS PART OF THE MAIN RECT
	QPixmap pixmap;
	if ( pixmap_str != "" )
	{
		pixmap = QPixmap( pixmap_str );
		QRect pixmap_rect { rect.topLeft()
			                    + QPoint( x + align_overload - icon_size - icon_margin + padding / 2, y + padding / 2 ),
			                QSize( icon_size, icon_size ) };
		painter->drawPixmap(
			pixmap_rect, pixmap.scaled( icon_size, icon_size, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
	}
	return text_rect;
}

void RecordBannerDelegate::drawIcon(
	QPainter* painter,
	const int x,
	const int y,
	const QRect rect,
	const int size,
	const int align,
	const int link,
	QString pixmap_str ) const
{
	const QPixmap pixmap { pixmap_str };

	//The align overload picks which side of the rect to draw from
	int align_overload = align == LEFT ? 0 : align == RIGHT ? size : size / 2;

	painter->setRenderHint( QPainter::Antialiasing ); //Set so pixels look better

	QRect pixmap_rect { rect.topLeft() + QPoint( x + align_overload, y ), QSize( size, size ) };
	painter->drawPixmap( pixmap_rect, pixmap.scaled( size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
}

void RecordBannerDelegate::reloadConfig()
{
	ZoneScoped;
	m_grid_size = calculateSize(
		config::grid_ui::itemViewWidth::get(),
		config::grid_ui::bannerSizeX::get(),
		config::grid_ui::bannerSizeY::get(),
		config::grid_ui::bannerSpacing::get() );
	m_banner_size = { config::grid_ui::bannerSizeX::get(), config::grid_ui::bannerSizeY::get() };
	m_scale_type = config::grid_ui::imageLayout::get();
	m_top_overlay_height = config::grid_ui::top_overlay_height::get();
	m_bottom_overlay_height = config::grid_ui::bottom_overlay_height::get();
	m_enable_top_overlay = config::grid_ui::enable_top_overlay::get();
	m_enable_bottom_overlay = config::grid_ui::enable_bottom_overlay::get();
	m_overlay_layout = config::grid_ui::overlay_layout::get();
	m_top_overlay_color = config::grid_ui::top_overlay_bcolor::get();
	m_bottom_overlay_color = config::grid_ui::bottom_overlay_bcolor::get();
	m_feather_radius = config::grid_ui::featherRadius::get();
	m_blur_radius = config::grid_ui::blurRadius::get();
	m_blur_type = config::grid_ui::blurType::get();
	m_enable_capsule_border = config::grid_ui::enableCapsuleBorder::get();
	m_font_size = config::grid_ui::font_size::get();
	m_font_family = config::grid_ui::font::get();
	m_font_bold = config::grid_ui::font_bold::get();
	m_font_italic = config::grid_ui::font_italic::get();
	m_font_shadow = config::grid_ui::font_shadow::get();
	m_padding = config::grid_ui::padding::get();
	m_corner_radius = config::grid_ui::corner_radius::get();
	//BANNER START
	//title
	m_title_enable = { config::grid_ui::title_enable::get() };
	m_title_x = { config::grid_ui::title_x::get() };
	m_title_y = { config::grid_ui::title_y::get() };
	m_title_align = { config::grid_ui::title_align::get() };
	m_title_bcolor = { colorFromString( config::grid_ui::title_bcolor::get() ) };
	m_title_fcolor = { colorFromString( config::grid_ui::title_fcolor::get() ) };
	m_title_fontsize = { config::grid_ui::title_font_size::get() };
	m_title_default = { config::grid_ui::title_default::get() };
	m_title_link = { config::grid_ui::title_link::get() };
	//engine
	m_engine_enable = { config::grid_ui::engine_enable::get() };
	m_engine_x = { config::grid_ui::engine_x::get() };
	m_engine_y = { config::grid_ui::engine_y::get() };
	m_engine_align = { config::grid_ui::engine_align::get() };
	m_engine_bcolor = { colorFromString( config::grid_ui::engine_bcolor::get() ) };
	m_engine_fcolor = { colorFromString( config::grid_ui::engine_fcolor::get() ) };
	m_engine_default = { config::grid_ui::engine_default::get() };
	//version
	m_version_enable = { config::grid_ui::version_enable::get() };
	m_version_x = { config::grid_ui::version_x::get() };
	m_version_y = { config::grid_ui::version_y::get() };
	m_version_align = { config::grid_ui::version_align::get() };
	m_version_bcolor = { colorFromString( config::grid_ui::version_bcolor::get() ) };
	m_version_fcolor = { colorFromString( config::grid_ui::version_fcolor::get() ) };
	m_version_default = { config::grid_ui::version_default::get() };
	//creator
	m_creator_enable = { config::grid_ui::creator_enable::get() };
	m_creator_x = { config::grid_ui::creator_x::get() };
	m_creator_y = { config::grid_ui::creator_y::get() };
	m_creator_align = { config::grid_ui::creator_align::get() };
	m_creator_bcolor = { colorFromString( config::grid_ui::creator_bcolor::get() ) };
	m_creator_fcolor = { colorFromString( config::grid_ui::creator_fcolor::get() ) };
	m_creator_default = { config::grid_ui::creator_default::get() };
	//status
	m_status_enable = { config::grid_ui::status_enable::get() };
	m_status_x = { config::grid_ui::status_x::get() };
	m_status_y = { config::grid_ui::status_y::get() };
	m_status_align = { config::grid_ui::status_align::get() };
	m_status_bcolor = { colorFromString( config::grid_ui::status_bcolor::get() ) };
	m_status_fcolor = { colorFromString( config::grid_ui::status_fcolor::get() ) };
	m_status_default = { config::grid_ui::status_default::get() };
	//gametype
	m_gametype_enable = { config::grid_ui::gametype_enable::get() };
	m_gametype_x = { config::grid_ui::gametype_x::get() };
	m_gametype_y = { config::grid_ui::gametype_y::get() };
	m_gametype_align = { config::grid_ui::gametype_align::get() };
	m_gametype_bcolor = { colorFromString( config::grid_ui::gametype_bcolor::get() ) };
	m_gametype_fcolor = { colorFromString( config::grid_ui::gametype_fcolor::get() ) };
	m_gametype_default = { config::grid_ui::gametype_default::get() };
	//rating
	m_rating_enable = { config::grid_ui::rating_enable::get() };
	m_rating_x = { config::grid_ui::rating_x::get() };
	m_rating_y = { config::grid_ui::rating_y::get() };
	m_rating_align = { config::grid_ui::rating_align::get() };
	m_rating_bcolor = { colorFromString( config::grid_ui::rating_bcolor::get() ) };
	m_rating_fcolor = { colorFromString( config::grid_ui::rating_fcolor::get() ) };
	m_rating_default = { config::grid_ui::rating_default::get() };
	//views
	m_views_enable = { config::grid_ui::views_enable::get() };
	m_views_x = { config::grid_ui::views_x::get() };
	m_views_y = { config::grid_ui::views_y::get() };
	m_views_align = { config::grid_ui::views_align::get() };
	m_views_bcolor = { colorFromString( config::grid_ui::views_bcolor::get() ) };
	m_views_fcolor = { colorFromString( config::grid_ui::views_fcolor::get() ) };
	m_views_default = { config::grid_ui::views_default::get() };
	//likes
	m_likes_enable = { config::grid_ui::likes_enable::get() };
	m_likes_x = { config::grid_ui::likes_x::get() };
	m_likes_y = { config::grid_ui::likes_y::get() };
	m_likes_align = { config::grid_ui::likes_align::get() };
	m_likes_bcolor = { colorFromString( config::grid_ui::likes_bcolor::get() ) };
	m_likes_fcolor = { colorFromString( config::grid_ui::likes_fcolor::get() ) };
	m_likes_default = { config::grid_ui::likes_default::get() };
	//favorite
	m_favorite_enable = { config::grid_ui::favorite_enable::get() };
	m_favorite_x = { config::grid_ui::favorite_x::get() };
	m_favorite_y = { config::grid_ui::favorite_y::get() };
	m_favorite_align = { config::grid_ui::favorite_align::get() };
	m_favorite_bcolor = { colorFromString( config::grid_ui::favorite_bcolor::get() ) };
	m_favorite_fcolor = { colorFromString( config::grid_ui::favorite_fcolor::get() ) };
	m_favorite_default = { config::grid_ui::favorite_default::get() };
	//updateicon
	m_updateicon_enable = { config::grid_ui::updateicon_enable::get() };
	m_updateicon_x = { config::grid_ui::updateicon_x::get() };
	m_updateicon_y = { config::grid_ui::updateicon_y::get() };
	m_updateicon_align = { config::grid_ui::updateicon_align::get() };
	m_updateicon_bcolor = { colorFromString( config::grid_ui::updateicon_bcolor::get() ) };
	m_updateicon_fcolor = { colorFromString( config::grid_ui::updateicon_fcolor::get() ) };
	m_updateicon_default = { config::grid_ui::updateicon_default::get() };
}

RecordBannerDelegate::RecordBannerDelegate( RecordListModel* model, QWidget* parent ) :
  QAbstractItemDelegate( parent ),
  m_grid_size { calculateSize(
	  config::grid_ui::itemViewWidth::get(),
	  config::grid_ui::bannerSizeX::get(),
	  config::grid_ui::bannerSizeY::get(),
	  config::grid_ui::bannerSpacing::get() ) },
  m_scale_type { config::grid_ui::imageLayout::get() },
  m_top_overlay_height { config::grid_ui::top_overlay_height::get() },
  m_bottom_overlay_height { config::grid_ui::bottom_overlay_height::get() },
  m_enable_top_overlay { config::grid_ui::enable_top_overlay::get() },
  m_enable_bottom_overlay { config::grid_ui::enable_bottom_overlay::get() },
  m_overlay_layout { config::grid_ui::overlay_layout::get() },
  m_top_overlay_color { config::grid_ui::top_overlay_bcolor::get() },
  m_bottom_overlay_color { config::grid_ui::bottom_overlay_bcolor::get() },
  m_feather_radius { config::grid_ui::featherRadius::get() },
  m_blur_radius { config::grid_ui::blurRadius::get() },
  m_blur_type { config::grid_ui::blurType::get() },
  m_enable_capsule_border { config::grid_ui::enableCapsuleBorder::get() },
  m_font_size { config::grid_ui::font_size::get() },
  m_font_family { config::grid_ui::font::get() },
  m_font_bold { config::grid_ui::font_bold::get() },
  m_font_italic { config::grid_ui::font_italic::get() },
  m_font_shadow { config::grid_ui::font_shadow::get() },
  m_padding { config::grid_ui::padding::get() },
  m_corner_radius { config::grid_ui::corner_radius::get() },
  //START BANNER
  //title
  //title
  m_title_enable { config::grid_ui::title_enable::get() },
  m_title_x { config::grid_ui::title_x::get() },
  m_title_y { config::grid_ui::title_y::get() },
  m_title_align { config::grid_ui::title_align::get() },
  m_title_bcolor { colorFromString( config::grid_ui::title_bcolor::get() ) },
  m_title_fcolor { colorFromString( config::grid_ui::title_fcolor::get() ) },
  m_title_fontsize { config::grid_ui::title_font_size::get() },
  m_title_default { config::grid_ui::title_default::get() },
  m_title_link { config::grid_ui::title_link::get() },
  //engine
  m_engine_enable { config::grid_ui::engine_enable::get() },
  m_engine_x { config::grid_ui::engine_x::get() },
  m_engine_y { config::grid_ui::engine_y::get() },
  m_engine_align { config::grid_ui::engine_align::get() },
  m_engine_bcolor { colorFromString( config::grid_ui::engine_bcolor::get() ) },
  m_engine_fcolor { colorFromString( config::grid_ui::engine_fcolor::get() ) },
  m_engine_default { config::grid_ui::engine_default::get() },
  //version
  m_version_enable { config::grid_ui::version_enable::get() },
  m_version_x { config::grid_ui::version_x::get() },
  m_version_y { config::grid_ui::version_y::get() },
  m_version_align { config::grid_ui::version_align::get() },
  m_version_bcolor { colorFromString( config::grid_ui::version_bcolor::get() ) },
  m_version_fcolor { colorFromString( config::grid_ui::version_fcolor::get() ) },
  m_version_default { config::grid_ui::version_default::get() },
  //creator
  m_creator_enable { config::grid_ui::creator_enable::get() },
  m_creator_x { config::grid_ui::creator_x::get() },
  m_creator_y { config::grid_ui::creator_y::get() },
  m_creator_align { config::grid_ui::creator_align::get() },
  m_creator_bcolor { colorFromString( config::grid_ui::creator_bcolor::get() ) },
  m_creator_fcolor { colorFromString( config::grid_ui::creator_fcolor::get() ) },
  m_creator_default { config::grid_ui::creator_default::get() },
  //status
  m_status_enable { config::grid_ui::status_enable::get() },
  m_status_x { config::grid_ui::status_x::get() },
  m_status_y { config::grid_ui::status_y::get() },
  m_status_align { config::grid_ui::status_align::get() },
  m_status_bcolor { colorFromString( config::grid_ui::status_bcolor::get() ) },
  m_status_fcolor { colorFromString( config::grid_ui::status_fcolor::get() ) },
  m_status_default { config::grid_ui::status_default::get() },
  //gametype
  m_gametype_enable { config::grid_ui::gametype_enable::get() },
  m_gametype_x { config::grid_ui::gametype_x::get() },
  m_gametype_y { config::grid_ui::gametype_y::get() },
  m_gametype_align { config::grid_ui::gametype_align::get() },
  m_gametype_bcolor { colorFromString( config::grid_ui::gametype_bcolor::get() ) },
  m_gametype_fcolor { colorFromString( config::grid_ui::gametype_fcolor::get() ) },
  m_gametype_default { config::grid_ui::gametype_default::get() },
  //rating
  m_rating_enable { config::grid_ui::rating_enable::get() },
  m_rating_x { config::grid_ui::rating_x::get() },
  m_rating_y { config::grid_ui::rating_y::get() },
  m_rating_align { config::grid_ui::rating_align::get() },
  m_rating_bcolor { colorFromString( config::grid_ui::rating_bcolor::get() ) },
  m_rating_fcolor { colorFromString( config::grid_ui::rating_fcolor::get() ) },
  m_rating_default { config::grid_ui::rating_default::get() },
  //views
  m_views_enable { config::grid_ui::views_enable::get() },
  m_views_x { config::grid_ui::views_x::get() },
  m_views_y { config::grid_ui::views_y::get() },
  m_views_align { config::grid_ui::views_align::get() },
  m_views_bcolor { colorFromString( config::grid_ui::views_bcolor::get() ) },
  m_views_fcolor { colorFromString( config::grid_ui::views_fcolor::get() ) },
  m_views_default { config::grid_ui::views_default::get() },
  //likes
  m_likes_enable { config::grid_ui::likes_enable::get() },
  m_likes_x { config::grid_ui::likes_x::get() },
  m_likes_y { config::grid_ui::likes_y::get() },
  m_likes_align { config::grid_ui::likes_align::get() },
  m_likes_bcolor { colorFromString( config::grid_ui::likes_bcolor::get() ) },
  m_likes_fcolor { colorFromString( config::grid_ui::likes_fcolor::get() ) },
  m_likes_default { config::grid_ui::likes_default::get() },
  //favorite
  m_favorite_enable { config::grid_ui::favorite_enable::get() },
  m_favorite_x { config::grid_ui::favorite_x::get() },
  m_favorite_y { config::grid_ui::favorite_y::get() },
  m_favorite_align { config::grid_ui::favorite_align::get() },
  m_favorite_bcolor { colorFromString( config::grid_ui::favorite_bcolor::get() ) },
  m_favorite_fcolor { colorFromString( config::grid_ui::favorite_fcolor::get() ) },
  m_favorite_default { config::grid_ui::favorite_default::get() },
  //updateicon
  m_updateicon_enable { config::grid_ui::updateicon_enable::get() },
  m_updateicon_x { config::grid_ui::updateicon_x::get() },
  m_updateicon_y { config::grid_ui::updateicon_y::get() },
  m_updateicon_align { config::grid_ui::updateicon_align::get() },
  m_updateicon_bcolor { colorFromString( config::grid_ui::updateicon_bcolor::get() ) },
  m_updateicon_fcolor { colorFromString( config::grid_ui::updateicon_fcolor::get() ) },
  m_updateicon_default { config::grid_ui::updateicon_default::get() },

  //END BANNER
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

	//USED TO CENTER WIDGETS
	QSize qsize { m_center_widgets ? banner_width + static_cast< int >( banner_offset ) : banner_width + banner_spacing,
		          banner_height + banner_spacing };

	return qsize;
}

QColor RecordBannerDelegate::getEngineColor( QString str ) const
{
	QColor backgroundColor;
	const QString ename = str.toUpper();

	if ( ename == "ADRIFT" )
	{
		backgroundColor = "#0b77cd";
	}
	else if ( ename == "FLASH" )
	{
		backgroundColor = "#616161";
	}
	else if ( ename == "HTML" )
	{
		backgroundColor = "#54812d";
	}
	else if ( ename == "JAVA" )
	{
		backgroundColor = "#52a6b0";
	}
	else if ( ename == "OTHER" || ename == "OTHERS" )
	{
		backgroundColor = "#6c9c34";
	}
	else if ( ename == "QSP" )
	{
		backgroundColor = "#d22f2f";
	}
	else if ( ename == "RAGS" )
	{
		backgroundColor = "#c77700";
	}
	else if ( ename == "RPGM" || ename == "RPG Maker" )
	{
		backgroundColor = "#0b79d1";
	}
	else if ( ename == "REN'PY" )
	{
		backgroundColor = "#9d46e3";
	}
	else if ( ename == "TADS" )
	{
		backgroundColor = "#0b79d1";
	}
	else if ( ename == "UNITY" )
	{
		backgroundColor = "#ea5201";
	}
	else if ( ename == "UNREAL" || ename == "UNREAL ENGINE" )
	{
		backgroundColor = "#0d47a1";
	}
	else if ( ename == "WEBGL" )
	{
		backgroundColor = "#fe5901";
	}
	else if ( ename == "WOLF RPG" )
	{
		backgroundColor = "#39843c";
	}
	//NON F95
	else if ( ename == "KIRIKIRI" )
	{
		backgroundColor = "#5550ed";
	}
	else if ( ename == "TYANOBUILDER" )
	{
		backgroundColor = "#3e8baf";
	}
	else if ( ename == "VISUAL NOVEL MAKER" )
	{
		backgroundColor = "#26ade4";
	}
	else if ( ename == "NSCRIPTER" )
	{
		backgroundColor = "#4646ff";
	}
	else
	{
		backgroundColor = "#3f4043";
	}

	//Return a color if enabled. If not, return transparent
	return backgroundColor;
}

QColor RecordBannerDelegate::getStatusColor( QString str ) const
{
	QColor color;
	const QString ename = str.toUpper();
	if ( ename == "COMPLETED" )
	{
		color = "#0b79d1";
	}
	else if ( ename == "ABANDONED" )
	{
		color = "#c77700";
	}
	else if ( ename == "ON HOLD" )
	{
		color = "#03a9f4";
	}
	return color;

	return color;
}

QColor RecordBannerDelegate::getGameTypeColor( QString str ) const
{
	QColor color;
	const QString ename = str.toUpper();
	if ( ename == "VN" )
	{
		color = "#d32f2f";
	}
	else if ( ename == "OTHERS" )
	{
		color = "#3f4043";
	}
	return color;
}

QColor RecordBannerDelegate::colorFromString( QString str )
{
	QColor color;
#if ( QT_VERSION >= QT_VERSION_CHECK( 6, 4, 0 ) )
	return QColor::fromString( str );
#else
	color.setNamedColor( str );
	return color;
#endif
}