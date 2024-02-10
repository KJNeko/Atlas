//
// Created by kj16609 on 2/9/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QWidget>

#include <iostream>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class OverlaySettingsWidget;
}

QT_END_NAMESPACE

class OverlaySettingsWidget : public QWidget
{
	Q_OBJECT

	//Internal values for setting saving/unloading
	QString m_settings_namespace {};
	QString m_settings_key {};

	QString m_fg_color {};
	QString m_bg_color {};

  public:

	Q_PROPERTY( QString m_settings_namespace READ settingsNamespace WRITE setSettingsNamespace )
	Q_PROPERTY( QString m_settings_key READ settingsKey WRITE setSettingsKey )

	//Internal
	Q_PROPERTY( QString m_uservisible_name READ userVisibleName WRITE setUserVisibleName )

	Q_PROPERTY( bool m_enabled READ settingsEnabled WRITE setSettingsEnabled )
	Q_PROPERTY( int m_xaxis READ xAxis WRITE setXAxis )
	Q_PROPERTY( int m_yaxis READ yAxis WRITE setYAxis )
	Q_PROPERTY( int m_allignment READ allignment WRITE setAllignment )
	Q_PROPERTY( bool m_default_color READ isDefaultColor WRITE setUsesDefaultColor )
	Q_PROPERTY( QString m_fg_color READ foregroundColor WRITE setForegroundColor )
	Q_PROPERTY( QString m_bg_font READ backgroundColor WRITE setBackgroundColor )
	Q_PROPERTY( int m_link READ link WRITE setLink )

	explicit OverlaySettingsWidget( QWidget* parent = nullptr );
	~OverlaySettingsWidget() override;

	void setSettingsNamespace( const QString& settingsNamespace ) { m_settings_namespace = settingsNamespace; }

	QString settingsNamespace() const { return m_settings_namespace; }

	void setSettingsKey( const QString& settingsKey ) { m_settings_key = settingsKey; }

	QString settingsKey() const { return m_settings_key; }

	void setUserVisibleName( const QString& uservisibleName );

	QString userVisibleName() const;

	void setSettingsEnabled( bool enabled );

	bool settingsEnabled() const;

	void setXAxis( int xaxis );

	int xAxis() const;

	void setYAxis( int yaxis );

	int yAxis() const;

	void setAllignment( int allignment );

	int allignment() const;

	void setUsesDefaultColor( bool defaultColor );

	bool isDefaultColor() const;

	void setForegroundColor( const QString& fgColor );

	QString foregroundColor() const;

	void setBackgroundColor( const QString& bgColor );

	QString backgroundColor() const;

	void setLink( int link );

	int link() const;

  private:

	Ui::OverlaySettingsWidget* ui;
};
