//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_CONFIG_HPP
#define HYDRUS95_CONFIG_HPP

#include <QSettings>
#include <QVariant>

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
