//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QObject>

#define ATLAS_PROPERTY_SETTINGS                                                                                        \
                                                                                                                       \
  private:                                                                                                             \
                                                                                                                       \
	QString m_settings_namespace {};                                                                                   \
	QString m_settings_key {};                                                                                         \
                                                                                                                       \
  public:                                                                                                              \
                                                                                                                       \
	Q_PROPERTY( QString m_settings_namespace READ settingsNamespace WRITE setSettingsNamespace )                       \
	Q_PROPERTY( QString m_settings_key READ settingsKey WRITE setSettingsKey )                                         \
	void setSettingsNamespace( const QString& settingsNamespace )                                                      \
	{                                                                                                                  \
		m_settings_namespace = settingsNamespace;                                                                      \
	}                                                                                                                  \
	QString settingsNamespace() const                                                                                  \
	{                                                                                                                  \
		return m_settings_namespace;                                                                                   \
	}                                                                                                                  \
	void setSettingsKey( const QString& settingsKey )                                                                  \
	{                                                                                                                  \
		m_settings_key = settingsKey;                                                                                  \
	}                                                                                                                  \
	QString settingsKey() const                                                                                        \
	{                                                                                                                  \
		return m_settings_key;                                                                                         \
	}                                                                                                                  \
	QString fullSettingsKey() const                                                                                    \
	{                                                                                                                  \
		return m_settings_namespace + "/" + m_settings_key;                                                            \
	}

#define ATLAS_SIMPLE_PROPERTY( type, i_name, name )                                                                    \
	void set##name( type name )                                                                                        \
	{                                                                                                                  \
		i_name = name;                                                                                                 \
	}                                                                                                                  \
	type get##name() const                                                                                             \
	{                                                                                                                  \
		return i_name;                                                                                                 \
	}
