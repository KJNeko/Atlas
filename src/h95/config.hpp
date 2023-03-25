//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_CONFIG_HPP
#define HYDRUS95_CONFIG_HPP

#include <filesystem>

#include <QSettings>
#include <QVariant>

#include "h95/logging.hpp"

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

//TODO: Add cache
inline QSettings getSettingsObject()
{
	return { "./data/config.ini", QSettings::IniFormat };
}

/**
 * @throws std::runtime_error when setting T not default constructable and no setting given
 * @tparam T type for setting
 * @param setting_name
 * @return
 */
//! Returns T for the given setting_name
template < typename T >
inline T getSettings( const QString setting_name )
{
	QSettings settings { getSettingsObject() };
	const auto variant { settings.value( setting_name ) };
	if ( variant.template canConvert< T >() )
		return variant.template value< T >();
	else
	{
		spdlog::warn( "Setting for {} was not populated!", setting_name );

		if constexpr ( std::is_default_constructible_v< T > )
			return {};
		else
			throw std::runtime_error(
				"T was not default constructable! Throwing instead! For given setting:" + setting_name.toStdString() );
	}
}

#define SETTING_D( type, group, name, default_val )                                                                    \
  namespace group::name                                                                                                \
  {                                                                                                                    \
	namespace internal                                                                                                 \
	{                                                                                                                  \
	  const std::string key { std::string( #group ) + "/" + #name };                                                   \
	  const std::optional< type > default_value { default_val };                                                       \
	}                                                                                                                  \
                                                                                                                       \
	inline type get()                                                                                                  \
	{                                                                                                                  \
	  QSettings settings { getSettingsObject() };                                                                      \
	  const QVariant value { settings.value( internal::key ) };                                                        \
	  if ( value.template canConvert< type >() )                                                                       \
		return value.template value< type >();                                                                         \
	  else if ( internal::default_value.has_value() )                                                                  \
		return internal::default_value.value();                                                                        \
	  else                                                                                                             \
		throw std::                                                                                                    \
			runtime_error( fmt::format( "No setting with name {} found, No default value given.", internal::key ) );   \
	}                                                                                                                  \
                                                                                                                       \
	inline void set( const type& value )                                                                               \
	{                                                                                                                  \
	  QSettings settings { getSettingsObject() };                                                                      \
	  settings.setValue( internal::key, value );                                                                       \
	}                                                                                                                  \
  }

#define SETTING( type, group, name ) SETTING_D( type, group, name, std::nullopt_t );

template < typename T >
void setSettings( const QString name, const T& value )
{
	getSettingsObject().setValue( name, value );
}

namespace config
{
	namespace paths
	{
		namespace database
		{
			inline QString getQString()
			{
				return "./data/hydrus95.db";
				/*
				QSettings settings { getSettingsObject() };
				return settings.value( "paths/database", "./data/hydrus95.db" ).value< QString >();
				 */
			}

			inline std::filesystem::path get()
			{
				return { getQString().toStdString() };
			}

			inline std::filesystem::path getCanonical()
			{
				const auto path { get() };
				if ( std::filesystem::exists( path ) )
					return std::filesystem::canonical( get() );
				else
					return path;
			}

			/*
			inline void set( const QString path )
			{
				setSettings( "paths/database", path );
			}

			inline void set( std::filesystem::path path )
			{
				set( QString::fromStdString( path.string() ) );
			}*/

		} // namespace database

		namespace images
		{
			inline QString getQString()
			{
				QSettings settings { getSettingsObject() };
				return settings.value( "paths/images", "./data/images" ).value< QString >();
			}

			inline std::filesystem::path get()
			{
				return { getQString().toStdString() };
			}

			inline std::filesystem::path getCanonical()
			{
				const auto path { get() };
				if ( std::filesystem::exists( path ) )
					return std::filesystem::canonical( get() );
				else
					return path;
			}

			inline void set( const QString path )
			{
				setSettings( "paths/images", path );
			}

			inline void set( std::filesystem::path path )
			{
				set( QString::fromStdString( path.string() ) );
			}

		} // namespace images

		namespace games
		{
			inline QString getQString()
			{
				QSettings settings { getSettingsObject() };
				return settings.value( "paths/games", "./data/games" ).value< QString >();
			}

			inline std::filesystem::path get()
			{
				return { getQString().toStdString() };
			}

			inline std::filesystem::path getCanonical()
			{
				const auto path { get() };
				if ( std::filesystem::exists( path ) )
					return std::filesystem::canonical( get() );
				else
					return path;
			}

			inline void set( const QString path )
			{
				setSettings( "paths/games", path );
			}

			inline void set( std::filesystem::path path )
			{
				set( QString::fromStdString( path.string() ) );
			}
		} // namespace games

	} // namespace paths

	SETTING_D( bool, db, first_start, true )

	SETTING_D( int, logging, level, 2 )

} // namespace config

#endif //HYDRUS95_CONFIG_HPP
