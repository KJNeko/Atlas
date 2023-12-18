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
	const int x_strip_height { m_strip_height };
	const int y_strip_height( m_strip_height );
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
		options_rect.topLeft().x(), options_rect.topLeft().y(), banner_size.width(), x_strip_height
	};
	//Bottom overlay QRect
	const QRect bottom_rect { options_rect.bottomLeft().x(),
		                      options_rect.bottomLeft().y() - y_strip_height + 1,
		                      banner_size.width(),
		                      y_strip_height };

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
		QFuture< QPixmap > banner { record.requestBanner( c_banner_size, aspect_ratio, Normal, USE_THUMBNAIL ) };

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

	//Set hover color
	//Set hover color
	if ( options.state & QStyle::State_MouseOver )
	{
		painter->fillRect( options_rect, QColor( 0, 0, 255, 50 ) );
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

	//Draw Overlay
	painter->fillRect( top_rect, enable_top_overlay ? m_overlay_color : "transparent" );
	painter->fillRect( bottom_rect, enable_bottom_overlay ? m_overlay_color : "transparent" );

	//set Pen and Font for default text
	QFont font;
	font.setFamily( m_font_family );
	font.setPixelSize( m_font_size );
	painter->setFont( font );
	painter->setPen( qRgb( 210, 210, 210 ) );
	//TODO: Add so the user will be able to change the color. This is the default for all pallets

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
			0,
			m_title_bcolor );
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
			10,
			getEngineColor( record->m_engine, m_engine_default_colors ) );
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
			10,
			m_creator_bcolor );
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
			10,
			m_version_bcolor );
		//this->drawText( painter, options_rect, stripe_height, m_version_location, latest->m_version );
	}
	else
	{
		//this->drawText( painter, options_rect, stripe_height, m_version_location, "No Version" );
		//Draw Creator
		//this->drawText( painter, options_rect, stripe_height, m_creator_location, record->m_creator );
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
void RecordBannerDelegate::drawText(
	QPainter* painter,
	const int x,
	const int y,
	const QRect rect,
	const QString& str,
	const int font_size,
	const QString font_family,
	const int padding,
	QColor backgroundColor ) const
{
	//painter->save();
	//Calculate rect size for text
	QFont font;
	font.setPixelSize( font_size );
	font.setFamily( font_family );
	QFontMetrics fm( font );
	int t_width = fm.horizontalAdvance( str ) + padding;
	int t_height = fm.height() + padding;

	painter->setFont( font ); //Override default
	painter->setRenderHint( QPainter::Antialiasing ); //Set so pixels look better

	//Create rec with 10px margin, rect is relative to current banner rect
	QRectF text_rect { rect.topLeft() + QPoint( x, y ), QSize( t_width, t_height ) };
	//QPainterPath path;
	//Draw Bounding Rect
	painter->setPen( QPen( backgroundColor ) ); //no pen
	painter->setBrush( QBrush( backgroundColor ) );
	painter->drawRoundedRect( text_rect, 5, 5 );
	//Reset pet to draw text
	painter->setPen( qRgb( 210, 210, 210 ) );
	//Alight text in center of Rect
	painter->drawText( text_rect, Qt::AlignHCenter | Qt::AlignVCenter, str );
	//painter->restore();

	//qInfo() << "STATS: " << str << " x_loc:" << text_rect.x() << " y_loc:" << text_rect.y();
	//const QSize size { rect.width(), strip_size };
	//const QRect text_rect { rect.topLeft() + QPoint( 10, 0 ), size };
	//painter->drawText( text_rect, Qt::AlignLeft | Qt::AlignVCenter, str );
	return;
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
	m_overlay_layout = config::grid_ui::overlayLayout::get();
	m_overlay_color = config::grid_ui::overlayColor::get();
	m_feather_radius = config::grid_ui::featherRadius::get();
	m_blur_radius = config::grid_ui::blurRadius::get();
	m_blur_type = config::grid_ui::blurType::get();
	m_enable_capsule_border = config::grid_ui::enableCapsuleBorder::get();
	m_font_size = config::grid_ui::fontSize::get();
	m_font_family = config::grid_ui::font::get();
	//BANNER START
	//title
	m_title_enable = { config::grid_ui::title_enable::get() };
	m_title_x = { config::grid_ui::title_x::get() };
	m_title_y = { config::grid_ui::title_y::get() };
	m_title_align = { config::grid_ui::title_align::get() };
	m_title_bcolor = { colorFromString( config::grid_ui::title_bcolor::get() ) };
	m_title_fcolor = { colorFromString( config::grid_ui::title_fcolor::get() ) };
	m_title_fontsize = { config::grid_ui::title_font_size::get() };
	m_title_bold = { config::grid_ui::title_bold_enable::get() };
	m_title_italics = { config::grid_ui::title_italics_enable::get() };
	//engine
	m_engine_enable = { config::grid_ui::engine_enable::get() };
	m_engine_x = { config::grid_ui::engine_x::get() };
	m_engine_y = { config::grid_ui::engine_y::get() };
	m_engine_align = { config::grid_ui::engine_align::get() };
	m_engine_bcolor = { colorFromString( config::grid_ui::engine_bcolor::get() ) };
	m_engine_fcolor = { colorFromString( config::grid_ui::engine_fcolor::get() ) };
	m_engine_default_colors = { config::grid_ui::engine_default_color::get() };
	m_engine_bold = { config::grid_ui::engine_bold_enable::get() };
	m_engine_italics = { config::grid_ui::engine_italics_enable::get() };
	//version
	m_version_enable = { config::grid_ui::version_enable::get() };
	m_version_x = { config::grid_ui::version_x::get() };
	m_version_y = { config::grid_ui::version_y::get() };
	m_version_align = { config::grid_ui::version_align::get() };
	m_version_bcolor = { colorFromString( config::grid_ui::version_bcolor::get() ) };
	m_version_fcolor = { colorFromString( config::grid_ui::version_fcolor::get() ) };
	m_version_bold = { config::grid_ui::version_bold_enable::get() };
	m_version_italics = { config::grid_ui::version_italics_enable::get() };
	//creator
	m_creator_enable = { config::grid_ui::creator_enable::get() };
	m_creator_x = { config::grid_ui::creator_x::get() };
	m_creator_y = { config::grid_ui::creator_y::get() };
	m_creator_align = { config::grid_ui::creator_align::get() };
	m_creator_bcolor = { colorFromString( config::grid_ui::creator_bcolor::get() ) };
	m_creator_fcolor = { colorFromString( config::grid_ui::creator_fcolor::get() ) };
	m_creator_bold = { config::grid_ui::creator_bold_enable::get() };
	m_creator_italics = { config::grid_ui::creator_italics_enable::get() };
	//status
	m_status_enable = { config::grid_ui::status_enable::get() };
	m_status_x = { config::grid_ui::status_x::get() };
	m_status_y = { config::grid_ui::status_y::get() };
	m_status_align = { config::grid_ui::status_align::get() };
	m_status_link = { config::grid_ui::status_link::get() };
	//m_status_bold { config::grid_ui::status_bold_enable::get() };
	//m_status_italics { config::grid_ui::status_italics_enable::get() };
	//gametype
	m_gametype_enable = { config::grid_ui::gametype_enable::get() };
	m_gametype_x = { config::grid_ui::gametype_x::get() };
	m_gametype_y = { config::grid_ui::gametype_y::get() };
	m_gametype_align = { config::grid_ui::gametype_align::get() };
	m_gametype_link = { config::grid_ui::status_link::get() };
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
  m_overlay_layout { config::grid_ui::overlayLayout::get() },
  m_overlay_color { config::grid_ui::overlayColor::get() },
  m_feather_radius { config::grid_ui::featherRadius::get() },
  m_blur_radius { config::grid_ui::blurRadius::get() },
  m_blur_type { config::grid_ui::blurType::get() },
  m_enable_capsule_border { config::grid_ui::enableCapsuleBorder::get() },
  m_font_size { config::grid_ui::fontSize::get() },
  m_font_family { config::grid_ui::font::get() },
  //START BANNER
  //title
  m_title_enable { config::grid_ui::title_enable::get() },
  m_title_x { config::grid_ui::title_x::get() },
  m_title_y { config::grid_ui::title_y::get() },
  m_title_align { config::grid_ui::title_align::get() },
  m_title_bcolor { colorFromString( config::grid_ui::title_bcolor::get() ) },
  m_title_fcolor { colorFromString( config::grid_ui::title_fcolor::get() ) },
  m_title_fontsize { config::grid_ui::title_font_size::get() },
  m_title_bold { config::grid_ui::title_bold_enable::get() },
  m_title_italics { config::grid_ui::title_italics_enable::get() },
  //engine
  m_engine_enable { config::grid_ui::engine_enable::get() },
  m_engine_x { config::grid_ui::engine_x::get() },
  m_engine_y { config::grid_ui::engine_y::get() },
  m_engine_align { config::grid_ui::engine_align::get() },
  m_engine_bcolor { colorFromString( config::grid_ui::engine_bcolor::get() ) },
  m_engine_fcolor { colorFromString( config::grid_ui::engine_fcolor::get() ) },
  m_engine_default_colors { config::grid_ui::engine_default_color::get() },
  m_engine_bold { config::grid_ui::engine_bold_enable::get() },
  m_engine_italics { config::grid_ui::engine_italics_enable::get() },
  //version
  m_version_enable { config::grid_ui::version_enable::get() },
  m_version_x { config::grid_ui::version_x::get() },
  m_version_y { config::grid_ui::version_y::get() },
  m_version_align { config::grid_ui::version_align::get() },
  m_version_bcolor { colorFromString( config::grid_ui::version_bcolor::get() ) },
  m_version_fcolor { colorFromString( config::grid_ui::version_fcolor::get() ) },
  m_version_bold { config::grid_ui::version_bold_enable::get() },
  m_version_italics { config::grid_ui::version_italics_enable::get() },
  //creator
  m_creator_enable { config::grid_ui::creator_enable::get() },
  m_creator_x { config::grid_ui::creator_x::get() },
  m_creator_y { config::grid_ui::creator_y::get() },
  m_creator_align { config::grid_ui::creator_align::get() },
  m_creator_bcolor { colorFromString( config::grid_ui::creator_bcolor::get() ) },
  m_creator_fcolor { colorFromString( config::grid_ui::creator_fcolor::get() ) },
  m_creator_bold { config::grid_ui::creator_bold_enable::get() },
  m_creator_italics { config::grid_ui::creator_italics_enable::get() },
  //status
  m_status_enable { config::grid_ui::status_enable::get() },
  m_status_x { config::grid_ui::status_x::get() },
  m_status_y { config::grid_ui::status_y::get() },
  m_status_align { config::grid_ui::status_align::get() },
  m_status_link { config::grid_ui::status_link::get() },
  //m_status_bold { config::grid_ui::status_bold_enable::get() },
  //m_status_italics { config::grid_ui::status_italics_enable::get() },
  //gametype
  m_gametype_enable { config::grid_ui::gametype_enable::get() },
  m_gametype_x { config::grid_ui::gametype_x::get() },
  m_gametype_y { config::grid_ui::gametype_y::get() },
  m_gametype_align { config::grid_ui::gametype_align::get() },
  m_gametype_link { config::grid_ui::status_link::get() },

  //m_gametype_bold { config::grid_ui::gametype_bold_enable::get() },
  //m_gametype_italics { config::grid_ui::gametype_italics_enable::get() },

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

QColor RecordBannerDelegate::getEngineColor( QString engine, bool isEnabled ) const
{
	QColor backgroundColor;
	const QString ename = engine.toUpper();

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
	return isEnabled ? backgroundColor : "transparent";
}

QColor RecordBannerDelegate::colorFromString( QString str )
{
	QColor color;
#if ( QT_VERSION >= QT_VERSION_CHECK( 6, 4, 0 ) )
	return QColor::fromString( str );
#else
	return color.setNamedColor( str );
#endif
}