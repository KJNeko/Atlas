//
// Created by kj16609 on 2/18/24.
//

#pragma once
#include <QCheckBox>
#include <QDesignerCustomWidgetCollectionInterface>
#include <QDesignerCustomWidgetInterface>
#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

#include "SettingsToggle.hpp"
#include "designer/defines.hpp"

class SettingsToggleInterface : public QWidget, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	//Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" )
	Q_INTERFACES( QDesignerCustomWidgetInterface )

	bool initalized { false };

  public:

	explicit SettingsToggleInterface( QWidget* parent = nullptr ) : QWidget( parent ) {}

	QString name() const override { return "SettingsToggle"; }

	QString group() const override { return "Atlas Widgets"; }

	QString toolTip() const override { return "Widget for setting toggle settings"; }

	QString whatsThis() const override { return "Widget for setting toggle settings"; }

	QString includeFile() const override { return "designer/primitives/toggle/SettingsToggle.hpp"; }

	QIcon icon() const override { return QIcon(); }

	bool isContainer() const override { return false; }

	QWidget* createWidget( QWidget* parent ) override { return new SettingsToggle( parent ); }

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
				<widget class="SettingsToggle">
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
						<class>SettingsToggle</class>
						<extends>QWidget</extends>
						<header>designer/primitives/SettingsToggle.hpp</header>
					</customwidget>
				</customwidgets>
			</ui>
		)";
	}
};