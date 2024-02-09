//
// Created by kj16609 on 2/9/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class OverlaySettingsWidget;
}

QT_END_NAMESPACE

class OverlaySettingsWidget : public QWidget, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" )
	Q_INTERFACES( QDesignerCustomWidgetInterface )

  public:

	explicit OverlaySettingsWidget( QWidget* parent = nullptr );
	~OverlaySettingsWidget() override;

	QString name() const override { return "OverlaySettingsWidget"; }

	QString group() const override { return "Atlas Widgets"; }

	QString toolTip() const override { return "Widget for setting overlay settings"; }

	QString whatsThis() const override { return "Widget for setting overlay settings"; }

	QString includeFile() const override { return "designer/OverlaySettingsWidget.hpp"; }

	QIcon icon() const override { return QIcon(); }

	bool isContainer() const override { return false; }

	QWidget* createWidget( QWidget* parent ) override { return new OverlaySettingsWidget( parent ); }

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
			</ui>
		)";
	}

  private:

	Ui::OverlaySettingsWidget* ui;
};
