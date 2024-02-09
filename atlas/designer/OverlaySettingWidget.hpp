//
// Created by kj16609 on 2/9/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class OverlaySettingWidget;
}

QT_END_NAMESPACE

class OverlaySettingWidget : public QWidget, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" )
	Q_INTERFACES( QDesignerCustomWidgetInterface )

  public:

	explicit OverlaySettingWidget( QWidget* parent = nullptr );
	~OverlaySettingWidget() override;

	QString name() const override { return "Overlay Settings Widget"; }

	QString group() const override { return "Atlas Widgets"; }

	QString toolTip() const override { return "Widget for setting overlay settings"; }

	QString whatsThis() const override { return "Widget for setting overlay settings"; }

	QString includeFile() const override { return "designer/OverlaySettingWidget.hpp"; }

	QIcon icon() const override { return QIcon(); }

	bool isContainer() const override { return false; }

	QWidget* createWidget( QWidget* parent ) override { return new OverlaySettingWidget( parent ); }

	//xml
	QString domXml() const override
	{
		return R"(
			<ui language="c++">
				<widget class="OverlaySettingWidget" name="Overlay Settings Widget">
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

	Ui::OverlaySettingWidget* ui;
};
