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

	QString m_settings_namespace {};
	QString m_settings_key {};

	QString m_uservisible_name { "Name" };

	bool m_enabled { false };
	QSize m_pos { 0, 0 };
	int m_allignment { 0 };
	bool m_default_color { false };
	QString m_fg_color {};
	QString m_bg_color {};
	int m_link { 0 };

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

	void setUserVisibleName( const QString& uservisibleName ) { m_uservisible_name = uservisibleName; }

	QString userVisibleName() const { return m_uservisible_name; }

	void setSettingsEnabled( bool enabled ) { m_enabled = enabled; }

	bool settingsEnabled() const { return m_enabled; }

	void setXAxis( int xaxis ) { m_pos.setWidth( xaxis ); }

	int xAxis() const { return m_pos.width(); }

	void setYAxis( int yaxis ) { m_pos.setHeight( yaxis ); }

	int yAxis() const { return m_pos.height(); }

	void setAllignment( int allignment ) { m_allignment = allignment; }

	int allignment() const { return m_allignment; }

	void setUsesDefaultColor( bool defaultColor ) { m_default_color = defaultColor; }

	bool isDefaultColor() const { return m_default_color; }

	void setForegroundColor( const QString& fgColor ) { m_fg_color = fgColor; }

	QString foregroundColor() const { return m_fg_color; }

	void setBackgroundColor( const QString& bgColor ) { m_bg_color = bgColor; }

	QString backgroundColor() const { return m_bg_color; }

	void setLink( int link ) { m_link = link; }

	int link() const { return m_link; }

  private:

	Ui::OverlaySettingsWidget* ui;
};
