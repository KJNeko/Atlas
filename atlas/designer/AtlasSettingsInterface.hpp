//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QObject>
#include <QSettings>

class AtlasSettingsInterfaceC
{
  public:

	virtual QString settingsNamespace() const = 0;
	virtual QString settingsKey() const = 0;
	virtual QString fullSettingsKey() const = 0;

	virtual void saveSettings( QSettings& settings ) = 0;
	virtual void loadSettings( QSettings& settings ) = 0;
};

Q_DECLARE_METATYPE( AtlasSettingsInterfaceC );

// Designer setup
/*
 * Widget is created:
 * Set to Designer mode
 *
 * Atlas setup
 * Widget is created
 * CTOR returns
 * Qt applies properties
 * Try load settings
 */
