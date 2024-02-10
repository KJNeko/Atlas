//
// Created by kj16609 on 2/10/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

#include "OverlaySettingsWidget.hpp"

class QDESIGNER_WIDGET_EXPORT OverlaySettingsWidgetInterface : public QWidget, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" )
	Q_INTERFACES( QDesignerCustomWidgetInterface )

	bool initalized { false };

  public:

	explicit OverlaySettingsWidgetInterface( QWidget* parent = nullptr ) : QWidget( parent ) {}

	QString name() const override { return "OverlaySettingsWidget"; }

	QString group() const override { return "Atlas Widgets"; }

	QString toolTip() const override { return "Widget for setting overlay settings"; }

	QString whatsThis() const override { return "Widget for setting overlay settings"; }

	QString includeFile() const override { return "designer/OverlaySettingsWidget.hpp"; }

	QIcon icon() const override { return QIcon(); }

	bool isContainer() const override { return false; }

	QWidget* createWidget( QWidget* parent ) override { return new OverlaySettingsWidget( parent ); }

	void initialize( [[maybe_unused]] QDesignerFormEditorInterface* core ) override
	{
		if ( initalized )
		{
			return;
		}

		initalized = true;
	}

	bool isInitialized() const override { return initalized; }

	//xml
	QString domXml() const override
	{
		return R"(
			<ui language="c++">
				<widget class="OverlaySettingsWidget">
					<property name="geometry">
						<rect>
							<x>0</x>
							<y>0</y>
							<width>100</width>
							<height>100</height>
						</rect>
					</property>
				</widget>
				<customwidgets>
					<customwidget>
						<class>OverlaySettingsWidget</class>
						<extends>QWidget</extends>
						<header>designer/OverlaySettingsWidget.hpp</header>
					</customwidget>
				</customwidgets>
			</ui>
		)";
	}
};
