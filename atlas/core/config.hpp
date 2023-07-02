//
// Created by kj16609 on 1/12/23.
//

#ifndef ATLAS_CONFIG_HPP
#define ATLAS_CONFIG_HPP

#include <filesystem>

#include <QObject>
#include <QSettings>
#include <QVariant>

#include "ConfigNotification.hpp"
#include "logging.hpp"

/**
 *
 * @page H95Settings Settings list
 *
 * @warning THESE SHOULD NEVER BE MODIFIED MANUALLY IN `config.ini` UNLESS TOLD TOO. These are simply to provide some reference to what they are when developing new modules
 *
 * |Group 		| Key				| Value 	| Default	|
 * |------------|-------------------|-----------|-----------|
 * |			| first_launch 		| boolean 	|  true 	|
 * |			| version_number 	| int 		| 100 		|
 * | main_view	| item_width 		| int 		| 400 		|
 * | main_view	| item_height 		| int 		| 300		|
 * | main_view	| banner_width 		| int 		| 400		|
 * | main_view	| banner_height 	| int 		| 280		|
 * | main_view	| padding			| int		| 4			|
 * | paths 		| data 				| string 	| ./data/   |
 * | paths		| games				| string	| ./data/games	 |
 * | paths		| images			| string	| ./data/images |
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-promo"

//TODO: Add cache
inline QSettings getSettingsObject()
{
	return { "./data/config.ini", QSettings::IniFormat };
}

#define KEY_VALUE( group, name ) QString( #group ) + "/" + #name

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
				throw std::runtime_error(                                                                              \
					fmt::format( "Failed to convert key {} to desired type", KEY_VALUE( group, name ) ) );             \
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
					throw std::runtime_error(                                                                          \
						fmt::format( "Failed to convert key {} to desired type", KEY_VALUE( group, name ) ) );         \
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

#define CONFIG_ATTACH_THIS                                                                                             \
	connect(                                                                                                           \
		&( config::internal::getNotifier() ),                                                                          \
		&config::ConfigNotification::notification,                                                                     \
		this,                                                                                                          \
		&std::remove_pointer_t< decltype( this ) >::reloadConfig )

SETTINGS_PATH( paths, database, "./data" )
SETTINGS_PATH( paths, images, "./data/images" )
SETTINGS_PATH( paths, games, "./data/games" )
SETTINGS_FILE( paths, theme, "./data/themes/default.qss" )

SETTINGS_D( importer, pathparse, QString, "{creator}/{title}/{version}" )
SETTINGS_D( importer, skipFilesize, bool, false )
SETTINGS_D( importer, searchGameInfo, bool, true )
SETTINGS_D( importer, downloadBanner, bool, false )
SETTINGS_D( importer, downloadVNDB, bool, false )
SETTINGS_D( importer, moveImported, bool, true )

SETTINGS_D( db, first_start, bool, true )
SETTINGS_D( logging, level, int, 2 )

SETTINGS( geometry, main_window, QByteArray )
SETTINGS( geometry, batch_import_dialog, QByteArray )

SETTINGS_D( ui, use_system_theme, bool, true )

enum SCALE_TYPE : int
{
	IGNORE_ASPECT_RATIO = 0,
	KEEP_ASPECT_RATIO = 1,
	KEEP_ASPECT_RATIO_BY_EXPANDING = 2,
	FIT_BLUR_EXPANDING = 3,
	FIT_BLUR_STRETCH = 4
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

SETTINGS_DE( grid_ui, imageLayout, SCALE_TYPE, KEEP_ASPECT_RATIO ) // Default is keep aspect ratio
SETTINGS_DE( grid_ui, blurType, BLUR_TYPE, FEATHER_IMAGE )
SETTINGS_D( grid_ui, blurRadius, int, 30 )
SETTINGS_D( grid_ui, featherRadius, int, 30 )
SETTINGS_D( grid_ui, bannerSizeX, int, 537 )
SETTINGS_D( grid_ui, bannerSizeY, int, 251 )
SETTINGS_D( grid_ui, bannerSpacing, int, 5 )
SETTINGS_D( grid_ui, selectedColor, QString, "0,0,255" )
SETTINGS_D( grid_ui, selectedOpacity, int, 50 )
SETTINGS_D( grid_ui, enableCapsuleBorder, bool, true )
SETTINGS_D( grid_ui, borderColor, QString, "180,180,180" )

SETTINGS_D( grid_ui, enableTopOverlay, bool, true )
SETTINGS_D( grid_ui, enableBottomOverlay, bool, true )
SETTINGS_D( grid_ui, overlayHeight, int, 26 )
SETTINGS_D( grid_ui, overlayOpacity, int, 200 )
SETTINGS_D( grid_ui, overlayColor, QString, "0,0,0" )
SETTINGS_D( grid_ui, font, QString, "" )
SETTINGS_D( grid_ui, fontSize, int, 12 )
SETTINGS_D( grid_ui, windowHeight, int, 780 )
SETTINGS_D( grid_ui, windowWidth, int, 1520 )
SETTINGS_D( grid_ui, itemViewWidth, int, 1266 )
SETTINGS_D( grid_ui, itemViewHeight, int, 694 )

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

SETTINGS_DE( grid_ui, titleLocation, LOCATION, BOTTOM_CENTER ) //0:none, 1:top, 2:bottom
SETTINGS_DE( grid_ui, engineLocation, LOCATION, BOTTOM_LEFT )
SETTINGS_DE( grid_ui, versionLocation, LOCATION, BOTTOM_RIGHT )
SETTINGS_DE( grid_ui, creatorLocation, LOCATION, TOP_LEFT )

SETTINGS_D( grid_ui, favIconLocation, int, 0 )
SETTINGS_D( grid_ui, gameIconLocation, int, 0 )
SETTINGS_D( grid_ui, ratingIconLocation, int, 0 )
SETTINGS_D( grid_ui, downloadIconLocation, int, 0 )
SETTINGS_D( grid_ui, engineIconLocation, int, 0 )

SETTINGS_D( grid_ui, showFavIcon, bool, false )
SETTINGS_D( grid_ui, showGameIcon, bool, false )
SETTINGS_D( grid_ui, showRatingIcon, bool, false )
SETTINGS_D( grid_ui, showDownloadIcon, bool, false )
SETTINGS_D( grid_ui, showEngineIcon, bool, false )
SETTINGS_D( grid_ui, centerWidgets, bool, false )

SETTINGS_D( ui, stretch_banner_images, bool, false )
SETTINGS_D( ui, use_simple_layout, bool, true )
SETTINGS_D( images, image_type, QString, "webp" )

SETTINGS_D( application, font, QString, "" )
SETTINGS_D( application, fontSize, int, 9 )

SETTINGS_D( remote, last_check, int, 0 )
SETTINGS_D(
	remote, check_rate, int, std::chrono::duration_cast< std::chrono::seconds >( std::chrono::hours( 24 ) ).count() )

#pragma GCC diagnostic pop

#endif //ATLAS_CONFIG_HPP