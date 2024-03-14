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
	inline std::optional< T > get( const QString& namespace_name, const QString& setting_name )
	{
		auto key { namespace_name + "/" + setting_name };
		if ( !getSettingsObject().contains( key ) ) return std::nullopt;

		return getSettingsObject().value( key ).value< T >();
	}

	template < typename T >
	inline void set( const QString& namespace_name, const QString& setting_name, const T& value )
	{
		auto key { namespace_name + "/" + setting_name };
		getSettingsObject().setValue( key, value );
	}

	template <>
	inline void set<
		std::string_view >( const QString& namespace_name, const QString& setting_name, const std::string_view& value )
	{
		auto key { namespace_name + "/" + setting_name };
		getSettingsObject().setValue( key, QString::fromStdString( std::string( value ) ) );
	}

} // namespace config

namespace config
{
	using Character = std::string::value_type;

	template < std::size_t N >
		requires( N > 0 )
	struct TString
	{
		Character str[ N ];

		constexpr TString( const char ( &literal )[ N ] ) { std::ranges::copy( literal, str ); }

		operator std::string_view() const noexcept { return std::string_view( str, N - 1 ); }

		operator QString() const { return QString::fromStdString( std::string( str, N ) ); }
	};

	template < TString group, TString name, typename T >
	struct Setting
	{
		//static std::optional< T > value {};

		void static setValue( const T value ) { ::config::set< T >( group, name, value ); }

		void static set( const T value ) { setValue( value ); }

		std::optional< T > static getOptValue() { return ::config::get< T >( group, name ); }

		inline static T getValue()
		{
			if ( auto opt = getOptValue(); opt.has_value() )
				return opt.value();
			else
				//TODO: Add key into this message
				throw SettingsException(
					std::string( "Failed to get value for key: " ) + std::string( group ) + ":" + std::string( name ) );
		}

		// Here for the defines
		inline static T get() { return getValue(); }
	};

	template < TString group, TString name, typename T, const T default_value >
	struct SettingD : Setting< group, name, T >
	{
		inline static T getValue()
		{
			if ( auto opt = Setting< group, name, T >::getOptValue(); opt.has_value() )
				return opt.value();
			else
				return default_value;
		}

		// Here for the defines
		inline static T get() { return getValue(); }
	};

	template < TString group, TString name, typename _, TString default_value >
	struct SettingDQString : Setting< group, name, QString >
	{
		using T = QString;

		inline static T getValue()
		{
			if ( auto opt = Setting< group, name, T >::getOptValue(); opt.has_value() )
				return opt.value();
			else
				return default_value;
		}

		// Here for the defines
		inline static T get() { return getValue(); }
	};

	template < TString group, TString name, TString default_value >
	struct SettingDefaultString : Setting< group, name, std::string_view >

	{
		using T = std::string_view;

		inline static T getValue()
		{
			if ( auto opt = Setting< group, name, T >::getOptValue(); opt.has_value() )
				return opt.value();
			else
				return default_value;
		}

		// Here for the defines
		inline static T get() { return getValue(); }
	};

	template < TString group, TString name, TString default_path >
	struct SettingDPath : SettingDefaultString< group, name, default_path >
	{
		static void setDefault() { SettingDefaultString< group, name, default_path >::set( default_path ); }

		inline static std::filesystem::path getPath()
		{
			return { SettingDefaultString< group, name, default_path >::getValue() };
		}
	};

} // namespace config

#define SETTINGS( group, name, type )                                                                                  \
	namespace config::group                                                                                            \
	{                                                                                                                  \
		using name = Setting< #group, #name, type >;                                                                   \
	}

#define SETTINGS_DSTR( group, name, type, default_val )                                                                \
	namespace config::group                                                                                            \
	{                                                                                                                  \
		using name = SettingDQString< #group, #name, type, default_val >;                                              \
	}

#define SETTINGS_D( group, name, type, default_val )                                                                   \
	namespace config::group                                                                                            \
	{                                                                                                                  \
		using name = SettingD< #group, #name, type, default_val >;                                                     \
	}

#define SETTINGS_PATH( group, name, _default )                                                                         \
	namespace config::group                                                                                            \
	{                                                                                                                  \
		using name = SettingDPath< #group, #name, _default >;                                                          \
	}

#define SETTINGS_GEO( name ) SETTINGS( geometry, name, QByteArray )
#define SETTINGS_STATE( name ) SETTINGS( state, name, QByteArray )

#define CONFIG_ATTACH_THIS                                                                                             \
	connect(                                                                                                           \
		&( config::internal::getNotifier() ),                                                                          \
		&config::ConfigNotification::notification,                                                                     \
		this,                                                                                                          \
		&std::remove_pointer_t< decltype( this ) >::reloadConfig )

SETTINGS_PATH( paths, database, "./data" )
SETTINGS_PATH( paths, images, "./data/images" )
SETTINGS_PATH( paths, games, "./data/games" )
SETTINGS_PATH( paths, theme, "./themes/dark.qss" )

SETTINGS_DSTR( importer, pathparse, QString, "{creator}/{title}/{version}" )
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

enum BannerType : std::int8_t
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
SETTINGS_D( ui, image_layout, SCALE_TYPE, KEEP_ASPECT_RATIO_BY_EXPANDING ) // Default is keep aspect ratio
SETTINGS_D( ui, blur_type, BLUR_TYPE, FEATHER_IMAGE )
SETTINGS_D( ui, blur_strength, int, 30 )
SETTINGS_D( ui, feather_radius, int, 30 )
SETTINGS_D( ui, bannersize_x, int, 537 )
SETTINGS_D( ui, bannersize_y, int, 251 )
SETTINGS_D( ui, banner_spacing, int, 5 )
SETTINGS_DSTR( ui, selected_color, QString, "transparent" )
SETTINGS_D( ui, enable_banner_border, bool, true )
SETTINGS_DSTR( ui, border_color, QString, "transparent" )

SETTINGS_D( ui, windowHeight, int, 780 )
SETTINGS_D( ui, windowWidth, int, 1520 )
SETTINGS_D( ui, itemViewWidth, int, 1266 )
SETTINGS_D( ui, itemViewHeight, int, 694 )
//Font
SETTINGS_DSTR( ui, font, QString, "" )
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
SETTINGS_DSTR( ui, top_overlay_bcolor, QString, "#c8000000" )
SETTINGS_DSTR( ui, bottom_overlay_bcolor, QString, "#c8000000" )
//TITLE
SETTINGS_D( ui, title_enable, bool, true )
SETTINGS_D( ui, title_x, int, 268 )
SETTINGS_D( ui, title_y, int, 228 )
SETTINGS_D( ui, title_align, int, CENTER )
SETTINGS_D( ui, title_default, bool, true )
SETTINGS_DSTR( ui, title_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, title_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, title_font_size, int, 14 )
SETTINGS_D( ui, title_link, int, DISABLED )
//ENGINE
SETTINGS_D( ui, engine_enable, bool, true )
SETTINGS_D( ui, engine_x, int, 10 )
SETTINGS_D( ui, engine_y, int, 230 )
SETTINGS_D( ui, engine_align, int, LEFT )
SETTINGS_DSTR( ui, engine_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, engine_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, engine_default, bool, true )
SETTINGS_D( ui, engine_link, int, DISABLED )
//VERSION
SETTINGS_D( ui, version_enable, bool, true )
SETTINGS_D( ui, version_x, int, 527 )
SETTINGS_D( ui, version_y, int, 6 )
SETTINGS_D( ui, version_align, int, RIGHT )
SETTINGS_DSTR( ui, version_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, version_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, version_default, bool, true )
SETTINGS_D( ui, version_link, int, DISABLED )
//CREATOR
SETTINGS_D( ui, creator_enable, bool, true )
SETTINGS_D( ui, creator_x, int, 10 )
SETTINGS_D( ui, creator_y, int, 6 )
SETTINGS_D( ui, creator_align, int, LEFT )
SETTINGS_DSTR( ui, creator_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, creator_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, creator_default, bool, true )
SETTINGS_D( ui, creator_link, int, DISABLED )
//STATUS
SETTINGS_D( ui, status_enable, bool, false )
SETTINGS_D( ui, status_x, int, 527 )
SETTINGS_D( ui, status_y, int, 230 )
SETTINGS_D( ui, status_align, int, RIGHT )
SETTINGS_DSTR( ui, status_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, status_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, status_default, bool, true )
SETTINGS_D( ui, status_link, int, DISABLED )
//GAME TYPE
SETTINGS_D( ui, gametype_enable, bool, false )
SETTINGS_D( ui, gametype_x, int, 10 )
SETTINGS_D( ui, gametype_y, int, 206 )
SETTINGS_D( ui, gametype_align, int, LEFT )
SETTINGS_DSTR( ui, gametype_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, gametype_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, gametype_default, bool, true )
SETTINGS_D( ui, gametype_link, int, DISABLED )
//RATING
SETTINGS_D( ui, rating_enable, bool, false )
SETTINGS_D( ui, rating_x, int, 320 )
SETTINGS_D( ui, rating_y, int, 6 )
SETTINGS_D( ui, rating_align, int, LEFT )
SETTINGS_DSTR( ui, rating_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, rating_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, rating_default, bool, true )
SETTINGS_D( ui, rating_link, int, DISABLED )
//VIEWS
SETTINGS_D( ui, views_enable, bool, false )
SETTINGS_D( ui, views_x, int, 220 )
SETTINGS_D( ui, views_y, int, 6 )
SETTINGS_D( ui, views_align, int, LEFT )
SETTINGS_DSTR( ui, views_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, views_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, views_default, bool, true )
SETTINGS_D( ui, views_link, int, DISABLED )
//LIKES
SETTINGS_D( ui, likes_enable, bool, false )
SETTINGS_D( ui, likes_x, int, 270 )
SETTINGS_D( ui, likes_y, int, 6 )
SETTINGS_D( ui, likes_align, int, LEFT )
SETTINGS_DSTR( ui, likes_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, likes_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, likes_default, bool, true )
SETTINGS_D( ui, likes_link, int, DISABLED )
//FAVORITE
SETTINGS_D( ui, favorite_enable, bool, false )
SETTINGS_D( ui, favorite_x, int, 500 )
SETTINGS_D( ui, favorite_y, int, 205 )
SETTINGS_D( ui, favorite_align, int, RIGHT )
SETTINGS_DSTR( ui, favorite_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, favorite_fcolor, QString, "#d2d2d2" )
SETTINGS_D( ui, favorite_default, bool, true )
SETTINGS_D( ui, favorite_link, int, DISABLED )
//UPDATE ICON
SETTINGS_D( ui, updateicon_enable, bool, false )
SETTINGS_D( ui, updateicon_x, int, 10 )
SETTINGS_D( ui, updateicon_y, int, 2 )
SETTINGS_D( ui, updateicon_align, int, LEFT )
SETTINGS_DSTR( ui, updateicon_bcolor, QString, "transparent" )
SETTINGS_DSTR( ui, updateicon_fcolor, QString, "#d2d2d2" )
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

SETTINGS_D( ui, titleLocation, LOCATION, BOTTOM_CENTER ) //0:none, 1:top, 2:bottom
SETTINGS_D( ui, engineLocation, LOCATION, BOTTOM_LEFT )
SETTINGS_D( ui, versionLocation, LOCATION, BOTTOM_RIGHT )
SETTINGS_D( ui, creatorLocation, LOCATION, TOP_LEFT )

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

SETTINGS_DSTR( app, font, QString, "" )
SETTINGS_D( app, fontsize, int, 10 )
SETTINGS_PATH( app, theme, "./themes" )

SETTINGS_DSTR( general, update_channel, QString, "stable" )

SETTINGS_D( general, first_boot, bool, false );

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

namespace atlas::config
{
	using namespace ::config;

	void init();
	QSettings getConfigObject();
} // namespace atlas::config

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif //ATLAS_CONFIG_HPP