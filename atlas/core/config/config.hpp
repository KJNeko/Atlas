//
// Created by kj16609 on 1/12/23.
//

#pragma once
#ifndef ATLAS_CONFIG_HPP
#define ATLAS_CONFIG_HPP

#include <QObject>
#include <QSettings>
#include <QVariant>

#include <filesystem>

#include "ConfigNotification.hpp"
#include "core/exceptions.hpp"
#include "core/logging/formatters.hpp"
#include "core/logging/logging.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-promo"
#endif

//TODO: Add cache
inline QSettings getSettingsObject()
{
	return { "./data/config.ini", QSettings::IniFormat };
}

namespace config
{
	template < typename T >
	std::optional< T > get( const QString& namespace_name, const QString& setting_name )
	{
		auto key { namespace_name + "/" + setting_name };
		if ( !getSettingsObject().contains( key ) ) return std::nullopt;

		return getSettingsObject().value( key ).value< T >();
	}

	template < typename T >
	void set( const QString& namespace_name, const QString& setting_name, const T& value )
	{
		auto key { namespace_name + "/" + setting_name };
		getSettingsObject().setValue( key, value );
	}

} // namespace config

#define STR( val ) #val
#define KEY_VALUE( group, name ) STR( group ) "/" STR( name )

#define SETTINGS_D( group, name, type, default_value )                                                                 \
	namespace config::group::name                                                                                      \
	{                                                                                                                  \
		inline void set( const type& val )                                                                             \
		{                                                                                                              \
			getSettingsObject().setValue( KEY_VALUE( group, name ), val );                                             \
		}                                                                                                              \
                                                                                                                       \
		inline void setDefault()                                                                                       \
		{                                                                                                              \
			set( default_value );                                                                                      \
		}                                                                                                              \
                                                                                                                       \
		inline type get()                                                                                              \
		{                                                                                                              \
			if ( !getSettingsObject().contains( KEY_VALUE( group, name ) ) ) setDefault();                             \
                                                                                                                       \
			if ( const auto settings_obj = getSettingsObject().value( KEY_VALUE( group, name ) );                      \
			     settings_obj.canConvert< type >() )                                                                   \
				return settings_obj.value< type >();                                                                   \
			else                                                                                                       \
				throw SettingsException( "Failed to convert key " KEY_VALUE( group, name ) " to desired type" );       \
		}                                                                                                              \
	}

#define SETTINGS_PATH( group, name, default_path )                                                                     \
	SETTINGS_D( group, name, QString, default_path )                                                                   \
	namespace config::group::name                                                                                      \
	{                                                                                                                  \
		inline std::filesystem::path getPath()                                                                         \
		{                                                                                                              \
			const std::filesystem::path filepath { group::name::get().toStdString() };                                 \
			std::filesystem::create_directories( filepath );                                                           \
			return std::filesystem::canonical( filepath );                                                             \
		}                                                                                                              \
                                                                                                                       \
		inline void setPath( const std::filesystem::path path )                                                        \
		{                                                                                                              \
			group::name::set( QString::fromStdString( path.string() ) );                                               \
		}                                                                                                              \
	}

#define SETTINGS_FILE( group, name, default_path )                                                                     \
	SETTINGS_D( group, name, QString, default_path )                                                                   \
	namespace config::group::name                                                                                      \
	{                                                                                                                  \
		inline std::filesystem::path getPath()                                                                         \
		{                                                                                                              \
			const std::filesystem::path filepath { group::name::get().toStdString() };                                 \
			std::filesystem::create_directories( filepath.parent_path() );                                             \
			return std::filesystem::canonical( filepath );                                                             \
		}                                                                                                              \
                                                                                                                       \
		inline void setPath( const std::filesystem::path path )                                                        \
		{                                                                                                              \
			group::name::set( QString::fromStdString( path.string() ) );                                               \
		}                                                                                                              \
	}

#define SETTINGS( group, name, type )                                                                                  \
	namespace config::group::name                                                                                      \
	{                                                                                                                  \
		inline bool hasValue()                                                                                         \
		{                                                                                                              \
			return getSettingsObject().contains( KEY_VALUE( group, name ) );                                           \
		}                                                                                                              \
                                                                                                                       \
		inline type get()                                                                                              \
		{                                                                                                              \
			return { getSettingsObject().value( KEY_VALUE( group, name ) ).value< type >() };                          \
		}                                                                                                              \
                                                                                                                       \
		inline void set( const type val )                                                                              \
		{                                                                                                              \
			getSettingsObject().setValue( KEY_VALUE( group, name ), val );                                             \
		}                                                                                                              \
	}

#define SETTINGS_DE( group, name, type, default_value )                                                                \
	namespace config::group::name                                                                                      \
	{                                                                                                                  \
		namespace de_internal                                                                                          \
		{                                                                                                              \
			inline void set( const int& val )                                                                          \
			{                                                                                                          \
				getSettingsObject().setValue( KEY_VALUE( group, name ), val );                                         \
			}                                                                                                          \
                                                                                                                       \
			inline void setDefault()                                                                                   \
			{                                                                                                          \
				set( static_cast< int >( default_value ) );                                                            \
			}                                                                                                          \
                                                                                                                       \
			inline int get()                                                                                           \
			{                                                                                                          \
				if ( !getSettingsObject().contains( KEY_VALUE( group, name ) ) ) setDefault();                         \
                                                                                                                       \
				if ( const auto settings_obj = getSettingsObject().value( KEY_VALUE( group, name ) );                  \
				     settings_obj.canConvert< int >() )                                                                \
					return settings_obj.value< int >();                                                                \
				else                                                                                                   \
					throw SettingsException( "Failed to convert key " KEY_VALUE( group, name ) " to desired type" );   \
			}                                                                                                          \
                                                                                                                       \
			SETTINGS_D( group, name, int, default_value )                                                              \
		}                                                                                                              \
                                                                                                                       \
		inline type get()                                                                                              \
		{                                                                                                              \
			return static_cast< type >( de_internal::get() );                                                          \
		}                                                                                                              \
                                                                                                                       \
		inline void set( const type val )                                                                              \
		{                                                                                                              \
			de_internal::set( static_cast< int >( val ) );                                                             \
		}                                                                                                              \
	}

#define SETTINGS_GEO( name )                                                                                           \
	namespace config::geometry::name                                                                                   \
	{                                                                                                                  \
		inline bool hasValue()                                                                                         \
		{                                                                                                              \
			return getSettingsObject().contains( KEY_VALUE( geometry, name ) );                                        \
		}                                                                                                              \
                                                                                                                       \
		inline QByteArray get()                                                                                        \
		{                                                                                                              \
			return { getSettingsObject().value( KEY_VALUE( geometry, name ) ).toByteArray() };                         \
		}                                                                                                              \
                                                                                                                       \
		inline void set( const QByteArray val )                                                                        \
		{                                                                                                              \
			getSettingsObject().setValue( KEY_VALUE( geometry, name ), val );                                          \
		}                                                                                                              \
	}

#define SETTINGS_STATE( name )                                                                                         \
	namespace config::state::name                                                                                      \
	{                                                                                                                  \
		inline bool hasValue()                                                                                         \
		{                                                                                                              \
			return getSettingsObject().contains( KEY_VALUE( state, name ) );                                           \
		}                                                                                                              \
                                                                                                                       \
		inline QByteArray get()                                                                                        \
		{                                                                                                              \
			return getSettingsObject().value( KEY_VALUE( state, name ) ).toByteArray();                                \
		}                                                                                                              \
                                                                                                                       \
		inline void set( const QByteArray val )                                                                        \
		{                                                                                                              \
			getSettingsObject().setValue( KEY_VALUE( state, name ), val );                                             \
		}                                                                                                              \
	}

#define CONFIG_ATTACH_THIS                                                                                             \
	connect(                                                                                                           \
		&( config::internal::getNotifier() ),                                                                          \
		&config::ConfigNotification::notification,                                                                     \
		this,                                                                                                          \
		&std::remove_pointer_t< decltype( this ) >::reloadConfig )

SETTINGS_PATH( paths, database, "./data" )
SETTINGS_PATH( paths, images, "./data/images" )
SETTINGS_PATH( paths, games, "./data/games" )
SETTINGS_FILE( paths, theme, "./themes/dark.qss" )

SETTINGS_D( importer, pathparse, QString, "{creator}/{title}/{version}" )
SETTINGS_D( importer, skipFilesize, bool, false )
SETTINGS_D( importer, searchGameInfo, bool, true )
SETTINGS_D( importer, downloadBanner, bool, false )
SETTINGS_D( importer, downloadVNDB, bool, false )
SETTINGS_D( importer, moveImported, bool, false )

SETTINGS_D( logging, level, int, 2 )

SETTINGS_GEO( main_window )
SETTINGS_GEO( batch_import_dialog )

SETTINGS_STATE( main_window )

SETTINGS_D( app, use_system_theme, bool, false )

enum SCALE_TYPE : int
{
	IGNORE_ASPECT_RATIO = Qt::IgnoreAspectRatio,
	KEEP_ASPECT_RATIO = Qt::KeepAspectRatio,
	KEEP_ASPECT_RATIO_BY_EXPANDING = Qt::KeepAspectRatioByExpanding,
	FIT_BLUR_EXPANDING = 3, //TODO: Remove due to confusion
	FIT_BLUR_STRETCH = 4 //TODO: Remove due to confusion
};

enum BLUR_TYPE : int
{
	BACKGROUND_ONLY = 0,
	FEATHER_IMAGE = 1,
	SQUARE = 2
};

enum PreviewType
{
	PREVIEW_UNKNOWN = 0,
	PREVIEW_BANNER = 1,
	PREVIEW_PREVIEW = 2,
	PREVIEW_BANNER_WIDE = 3,
	PREVIEW_COVER = 4,
	PREVIEW_LOGO = 5
};

enum BannerType
{
	Error = -1,
	Normal = 0,
	Wide = 1,
	Cover = 2,
	Logo = 3,
	SENTINEL // Used to determine the number of banner types
};

enum BannerMask
{
	MASK_NORMAL = 1 << 0,
	MASK_WIDE = 1 << 1,
	MASK_COVER = 1 << 2,
	MASK_LOGO = 1 << 3,
};

enum OverlayType
{
	ON_TOP = 0,
	IN_LINE = 1
};

enum Alignment : int
{
	LEFT = 0,
	RIGHT = 1,
	CENTER = 2,
	TOP = 3,
	BOTTOM = 4,
};

enum Link : int
{
	DISABLED = 0,
	TITLE = 1,
	CREATOR = 2,
	VERSION = 3,
	ENGINE = 4,
	STATUS = 5,
	GAMETYPE = 6,
	RATING = 7,
	VIEWS = 8,
	LIKES = 9,
	FAVORITE = 10,
	UPDATEICON = 11
};

//Images
SETTINGS_DE( ui, image_layout, SCALE_TYPE, KEEP_ASPECT_RATIO_BY_EXPANDING ) // Default is keep aspect ratio
SETTINGS_DE( ui, blur_type, BLUR_TYPE, FEATHER_IMAGE )
SETTINGS_D( ui, blur_strength, int, 30 )
SETTINGS_D( ui, feather_radius, int, 30 )
SETTINGS_D( ui, bannersize_x, int, 537 )
SETTINGS_D( ui, bannersize_y, int, 251 )
SETTINGS_D( ui, banner_spacing, int, 5 )
SETTINGS_D( ui, selected_color, QString, "transparent" )
SETTINGS_D( ui, enable_banner_border, bool, true )
SETTINGS_D( ui, border_color, QString, "transparent" )

SETTINGS_D( ui, windowHeight, int, 780 )
SETTINGS_D( ui, windowWidth, int, 1520 )
SETTINGS_D( ui, itemViewWidth, int, 1266 )
SETTINGS_D( ui, itemViewHeight, int, 694 )
//Font
SETTINGS_D( ui, font, QString, "" )
SETTINGS_D( ui, font_size, int, 11 )
SETTINGS_D( ui, font_bold, bool, true )
SETTINGS_D( ui, font_italic, bool, false )
SETTINGS_D( ui, font_shadow, bool, true )
SETTINGS_D( ui, bounding_box, int, 4 )
SETTINGS_D( ui, corner_radius, int, 2 )

//FOR BANNER
//Overlay
SETTINGS_D( ui, enable_top_overlay, bool, true )
SETTINGS_D( ui, enable_bottom_overlay, bool, true )
SETTINGS_D( ui, bottom_overlay_height, int, 26 )
SETTINGS_D( ui, top_overlay_height, int, 26 )
SETTINGS_D( ui, overlay_layout, int, 0 )
SETTINGS_D( ui, top_overlay_bcolor, QString, "#c8000000" )
SETTINGS_D( ui, bottom_overlay_bcolor, QString, "#c8000000" )
//TITLE
SETTINGS_D( ui, title_enable, bool, true )
SETTINGS_D( ui, title_x, int, 268 )
SETTINGS_D( ui, title_y, int, 228 )
SETTINGS_D( ui, title_align, int, CENTER )
SETTINGS_D( ui, title_default, bool, true )
SETTINGS_D( ui, title_bcolor, QString, "transparent" )
SETTINGS_D( ui, title_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, title_font_size, int, 14 )
SETTINGS_D( ui, title_link, int, DISABLED )
//ENGINE
SETTINGS_D( ui, engine_enable, bool, true )
SETTINGS_D( ui, engine_x, int, 10 )
SETTINGS_D( ui, engine_y, int, 230 )
SETTINGS_D( ui, engine_align, int, LEFT )
SETTINGS_D( ui, engine_bcolor, QString, "transparent" )
SETTINGS_D( ui, engine_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, engine_default, bool, true )
SETTINGS_D( ui, engine_link, int, DISABLED )
//VERSION
SETTINGS_D( ui, version_enable, bool, true )
SETTINGS_D( ui, version_x, int, 527 )
SETTINGS_D( ui, version_y, int, 6 )
SETTINGS_D( ui, version_align, int, RIGHT )
SETTINGS_D( ui, version_bcolor, QString, "transparent" )
SETTINGS_D( ui, version_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, version_default, bool, true )
SETTINGS_D( ui, version_link, int, DISABLED )
//CREATOR
SETTINGS_D( ui, creator_enable, bool, true )
SETTINGS_D( ui, creator_x, int, 10 )
SETTINGS_D( ui, creator_y, int, 6 )
SETTINGS_D( ui, creator_align, int, LEFT )
SETTINGS_D( ui, creator_bcolor, QString, "transparent" )
SETTINGS_D( ui, creator_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, creator_default, bool, true )
SETTINGS_D( ui, creator_link, int, DISABLED )
//STATUS
SETTINGS_D( ui, status_enable, bool, false )
SETTINGS_D( ui, status_x, int, 527 )
SETTINGS_D( ui, status_y, int, 230 )
SETTINGS_D( ui, status_align, int, RIGHT )
SETTINGS_D( ui, status_bcolor, QString, "transparent" )
SETTINGS_D( ui, status_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, status_default, bool, true )
SETTINGS_D( ui, status_link, int, DISABLED )
//GAME TYPE
SETTINGS_D( ui, gametype_enable, bool, false )
SETTINGS_D( ui, gametype_x, int, 10 )
SETTINGS_D( ui, gametype_y, int, 206 )
SETTINGS_D( ui, gametype_align, int, LEFT )
SETTINGS_D( ui, gametype_bcolor, QString, "transparent" )
SETTINGS_D( ui, gametype_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, gametype_default, bool, true )
SETTINGS_D( ui, gametype_link, int, DISABLED )
//RATING
SETTINGS_D( ui, rating_enable, bool, false )
SETTINGS_D( ui, rating_x, int, 320 )
SETTINGS_D( ui, rating_y, int, 6 )
SETTINGS_D( ui, rating_align, int, LEFT )
SETTINGS_D( ui, rating_bcolor, QString, "transparent" )
SETTINGS_D( ui, rating_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, rating_default, bool, true )
SETTINGS_D( ui, rating_link, int, DISABLED )
//VIEWS
SETTINGS_D( ui, views_enable, bool, false )
SETTINGS_D( ui, views_x, int, 220 )
SETTINGS_D( ui, views_y, int, 6 )
SETTINGS_D( ui, views_align, int, LEFT )
SETTINGS_D( ui, views_bcolor, QString, "transparent" )
SETTINGS_D( ui, views_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, views_default, bool, true )
SETTINGS_D( ui, views_link, int, DISABLED )
//LIKES
SETTINGS_D( ui, likes_enable, bool, false )
SETTINGS_D( ui, likes_x, int, 270 )
SETTINGS_D( ui, likes_y, int, 6 )
SETTINGS_D( ui, likes_align, int, LEFT )
SETTINGS_D( ui, likes_bcolor, QString, "transparent" )
SETTINGS_D( ui, likes_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, likes_default, bool, true )
SETTINGS_D( ui, likes_link, int, DISABLED )
//FAVORITE
SETTINGS_D( ui, favorite_enable, bool, false )
SETTINGS_D( ui, favorite_x, int, 500 )
SETTINGS_D( ui, favorite_y, int, 205 )
SETTINGS_D( ui, favorite_align, int, RIGHT )
SETTINGS_D( ui, favorite_bcolor, QString, "transparent" )
SETTINGS_D( ui, favorite_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, favorite_default, bool, true )
SETTINGS_D( ui, favorite_link, int, DISABLED )
//UPDATE ICON
SETTINGS_D( ui, updateicon_enable, bool, false )
SETTINGS_D( ui, updateicon_x, int, 10 )
SETTINGS_D( ui, updateicon_y, int, 2 )
SETTINGS_D( ui, updateicon_align, int, LEFT )
SETTINGS_D( ui, updateicon_bcolor, QString, "transparent" )
SETTINGS_D( ui, updateicon_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, updateicon_default, bool, true )
SETTINGS_D( ui, updateicon_link, int, DISABLED )

enum LOCATION : int
{
	NONE = 0,
	TOP_LEFT = 1,
	TOP_CENTER = 2,
	TOP_RIGHT = 3,
	BOTTOM_LEFT = 4,
	BOTTOM_CENTER = 5,
	BOTTOM_RIGHT = 6,
};

SETTINGS_DE( ui, titleLocation, LOCATION, BOTTOM_CENTER ) //0:none, 1:top, 2:bottom
SETTINGS_DE( ui, engineLocation, LOCATION, BOTTOM_LEFT )
SETTINGS_DE( ui, versionLocation, LOCATION, BOTTOM_RIGHT )
SETTINGS_DE( ui, creatorLocation, LOCATION, TOP_LEFT )

SETTINGS_D( ui, favIconLocation, int, 0 )
SETTINGS_D( ui, gameIconLocation, int, 0 )
SETTINGS_D( ui, ratingIconLocation, int, 0 )
SETTINGS_D( ui, downloadIconLocation, int, 0 )
SETTINGS_D( ui, engineIconLocation, int, 0 )

SETTINGS_D( ui, showFavIcon, bool, false )
SETTINGS_D( ui, showGameIcon, bool, false )
SETTINGS_D( ui, showRatingIcon, bool, false )
SETTINGS_D( ui, showDownloadIcon, bool, false )
SETTINGS_D( ui, showEngineIcon, bool, false )
SETTINGS_D( ui, centerWidgets, bool, false )

SETTINGS_D( ui, stretch_banner_images, bool, false )
SETTINGS_D( ui, use_simple_layout, bool, true )

SETTINGS_D( app, font, QString, "" )
SETTINGS_D( app, fontsize, int, 10 )
SETTINGS_FILE( app, theme, "./themes" )

SETTINGS_D( general, update_channel, QString, "stable" )

SETTINGS_D( remote, allow_checks, bool, false )
SETTINGS_D( remote, last_check, int, 0 )
SETTINGS_D(
	remote, check_rate, int, std::chrono::duration_cast< std::chrono::seconds >( std::chrono::hours( 24 ) ).count() )

SETTINGS_D( threads, import_threads, int, 2 )
SETTINGS_D( threads, image_import_threads, int, 4 )
SETTINGS_D( threads, image_loader_threads, int, 2 )
SETTINGS_D( threads, import_pre_loader_threads, int, 4 )

SETTINGS_D( experimental, local_match, bool, false )
SETTINGS_D( experimental, loading_preview, bool, false )
SETTINGS_D( experimental, loading_preview_blur, int, 90 )
SETTINGS_D( experimental, use_blurhash, bool, false )

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif //ATLAS_CONFIG_HPP