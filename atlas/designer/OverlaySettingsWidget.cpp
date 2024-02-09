//
// Created by kj16609 on 2/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_OverlaySettingWidget.h" resolved

#include "designer/OverlaySettingsWidget.hpp"

#include "ui_OverlaySettingsWidget.h"

OverlaySettingsWidget::OverlaySettingsWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::OverlaySettingsWidget )
{
	ui->setupUi( this );
}

OverlaySettingsWidget::~OverlaySettingsWidget()
{
	delete ui;
}
