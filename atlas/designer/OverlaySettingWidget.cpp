//
// Created by kj16609 on 2/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_OverlaySettingWidget.h" resolved

#include "OverlaySettingWidget.hpp"

#include "ui_OverlaySettingWidget.h"

OverlaySettingWidget::OverlaySettingWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::OverlaySettingWidget )
{
	ui->setupUi( this );
}

OverlaySettingWidget::~OverlaySettingWidget()
{
	delete ui;
}
