//
// Created by kj16609 on 3/2/24.
//

#include "SettingsValue.hpp"

#include "moc_SettingsValue.cpp"

void SettingsValue::loadSettings( QSettings& settings )
{
	const QString settings_key { fullSettingsKey() };

	if ( settings.contains( settings_key ) ) this->setValue( settings.value( settings_key ).toInt() );
}

void SettingsValue::saveSettings( QSettings& settings )
{}