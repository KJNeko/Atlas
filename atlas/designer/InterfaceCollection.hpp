//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include "OverlaySettingsWidgetInterface.hpp"
#include "primitives/toggle/SettingsToggleInterface.hpp"

class AtlasWidgetsCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" )
	Q_INTERFACES( QDesignerCustomWidgetCollectionInterface )

	QList< QDesignerCustomWidgetInterface* > widgets { new OverlaySettingsWidgetInterface(),
		                                               new SettingsToggleInterface() };

  public:

	explicit AtlasWidgetsCollection( QObject* parent = nullptr ) : QObject( parent ) {}

	QList< QDesignerCustomWidgetInterface* > customWidgets() const override { return widgets; }
};
