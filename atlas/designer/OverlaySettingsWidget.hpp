//
// Created by kj16609 on 2/9/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QWidget>

#include <iostream>

#include "AtlasSettingsInterface.hpp"
#include "designer/defines.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class OverlaySettingsWidget;
}

QT_END_NAMESPACE

class OverlaySettingsWidget : public QWidget, public AtlasSettingsInterfaceC
{
	Q_OBJECT

	QColor m_fg_color {};
	QColor m_bg_color {};

  public:

	ATLAS_PROPERTY_SETTINGS

	//Internal
	Q_PROPERTY( QString m_uservisible_name READ userVisibleName WRITE setUserVisibleName )

	Q_PROPERTY( bool m_enabled READ settingsEnabled WRITE setSettingsEnabled )
	Q_PROPERTY( int m_xaxis READ xAxis WRITE setXAxis )
	Q_PROPERTY( int m_yaxis READ yAxis WRITE setYAxis )
	Q_PROPERTY( int m_allignment READ allignment WRITE setAllignment )
	Q_PROPERTY( bool m_default_color READ isDefaultColor WRITE setUsesDefaultColor )
	Q_PROPERTY( QColor m_fg_color READ foregroundColor WRITE setForegroundColor )
	Q_PROPERTY( QColor m_bg_font READ backgroundColor WRITE setBackgroundColor )
	Q_PROPERTY( int m_link READ link WRITE setLink )

	explicit OverlaySettingsWidget( QWidget* parent = nullptr );
	~OverlaySettingsWidget() override;

	void saveSettings( QSettings& settings ) override;
	void loadSettings( QSettings& settings ) override;

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

	void setForegroundColor( const QColor& fgColor );

	QColor foregroundColor() const;

	void setBackgroundColor( const QColor& bgColor );

	QColor backgroundColor() const;

	void setLink( int link );

	int link() const;

  private:

	Ui::OverlaySettingsWidget* ui;
};
