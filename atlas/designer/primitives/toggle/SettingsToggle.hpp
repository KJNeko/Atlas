//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QCheckBox>
#include <QDesignerCustomWidgetCollectionInterface>
#include <QDesignerCustomWidgetInterface>
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

#include "designer/defines.hpp"

class SettingsToggle : public QCheckBox
{
	Q_OBJECT

	bool m_default_enabled;

  public:

	SettingsToggle( QWidget* parent ) : QCheckBox( parent ) {}

	ATLAS_PROPERTY_SETTINGS

	Q_PROPERTY( bool m_default_enabled READ getEnabled WRITE setEnabled )
	ATLAS_SIMPLE_PROPERTY( bool, m_default_enabled, Enabled )
};
