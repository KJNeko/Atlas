//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_CONFIG_HPP
#define HYDRUS95_CONFIG_HPP

#include <filesystem>

#include <QSettings>
#include <QVariant>

#include <h95/logging.hpp>

/**
 *
 * @page H95Settings Settings list
 *
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
 *
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
template< typename T > inline T getSettings( const QString setting_name )
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

/**
 * @tparam T type for setting
 * @param setting_name
 * @param default_value
 * @return default_value if setting_name is not set
 */
template< typename T > inline T getSettings( const QString setting_name, const T default_value )
{
	QSettings settings { getSettingsObject() };
	const auto variant { settings.value( setting_name, default_value ) };
	if ( variant.template canConvert< T >() )
		return variant.template value< T >();
	else
	{
		setSettings( setting_name, default_value );
		return default_value;
	}
}

template< typename T > inline void setSettings( const QString settings_name, const T value )
{
	QSettings settings { getSettingsObject() };
	settings.setValue( settings_name, value );
}

//! Returns the canonical path for `path/data` (Default `./data/`)
std::filesystem::path dataPath();

//! Returns the canonical path for `path/images` (Default `./data/images`)
std::filesystem::path imagePath();

//! Returns the canonical path for `path/games` (Default `./data/igames`)
std::filesystem::path gamePath();




#endif	//HYDRUS95_CONFIG_HPP
