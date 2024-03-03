//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QtUiPlugin/QDesignerExportWidget>

#include "OverlaySettingsWidgetInterface.hpp"
#include "designer/primitives/SettingsToggle.hpp"
#include "designer/primitives/SettingsValue.hpp"

class QDESIGNER_WIDGET_EXPORT AtlasWidgetsCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" )
	Q_INTERFACES( QDesignerCustomWidgetCollectionInterface )

	QList< QDesignerCustomWidgetInterface* > widgets { new OverlaySettingsWidgetInterface(),
		                                               new SettingsToggleInterface(),
		                                               new SettingsValueInterface() };

  public:

	explicit AtlasWidgetsCollection( QObject* parent = nullptr ) : QObject( parent ) {}

	QList< QDesignerCustomWidgetInterface* > customWidgets() const override { return widgets; }
};
