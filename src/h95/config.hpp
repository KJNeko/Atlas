//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_CONFIG_HPP
#define HYDRUS95_CONFIG_HPP

#include <QSettings>
#include <QVariant>

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
 * | main_view	| banner_height 	| int 		| 300		|
 * | main_view	| padding			| int		| 8			|
 * |			| data_path			| string	| './data'	|
 * | import		| auto_fill_path 	| string	| '{path}/{title}' |
 * | import		| should_autofill 	| bool		| 'true'	|
 * | import		| dest_path			| string	| '{h95_games}/{title}' |
 * | import		| should_copy 		| bool		| 'true'	|
 * | import		| should_delete		| bool		| 'false' 	|
 *
 */

//TODO: Add cache
inline QSettings getSettingsObject()
{
	return { "./data/config.ini", QSettings::IniFormat };
}

template< typename T > inline T getSettings( const QString setting_name )
{
	QSettings settings { getSettingsObject() };
	const auto variant { settings.value( setting_name ) };
	if ( variant.template canConvert< T >() )
		return variant.template value< T >();
	else
		return {};
}

template< typename T > inline T getSettings( const QString setting_name, const T default_value )
{
	QSettings settings { getSettingsObject() };
	const auto variant { settings.value( setting_name, default_value ) };
	if ( variant.template canConvert< T >() )
		return variant.template value< T >();
	else
		return {};
}

template< typename T > inline void setSettings( const QString settings_name, const T value )
{
	QSettings settings { getSettingsObject() };
	settings.setValue( settings_name, value );
}


#endif	//HYDRUS95_CONFIG_HPP
