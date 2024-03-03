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

void OverlaySettingsWidget::setUserVisibleName( const QString& uservisibleName )
{
	ui->lbName->setText( uservisibleName );
}

QString OverlaySettingsWidget::userVisibleName() const
{
	return ui->lbName->text();
}

void OverlaySettingsWidget::setSettingsEnabled( bool enabled )
{
	ui->cbEnabled->setChecked( enabled );
}

bool OverlaySettingsWidget::settingsEnabled() const
{
	return ui->cbEnabled->isChecked();
}

void OverlaySettingsWidget::setXAxis( int xaxis )
{
	ui->sbXAxis->setValue( xaxis );
}

int OverlaySettingsWidget::xAxis() const
{
	return ui->sbXAxis->value();
}

void OverlaySettingsWidget::setYAxis( int yaxis )
{
	ui->sbYAxis->setValue( yaxis );
}

int OverlaySettingsWidget::yAxis() const
{
	return ui->sbYAxis->value();
}

void OverlaySettingsWidget::setAllignment( int allignment )
{
	ui->cbAlignment->setCurrentIndex( allignment );
}

int OverlaySettingsWidget::allignment() const
{
	return ui->sbXAxis->value();
}

void OverlaySettingsWidget::setUsesDefaultColor( bool defaultColor )
{
	ui->cbColorDefault->setChecked( defaultColor );
}

bool OverlaySettingsWidget::isDefaultColor() const
{
	return ui->cbColorDefault->isChecked();
}

void OverlaySettingsWidget::setForegroundColor( const QColor& fgColor )
{
	m_fg_color = fgColor;
}

QColor OverlaySettingsWidget::foregroundColor() const
{
	return m_fg_color;
}

void OverlaySettingsWidget::setBackgroundColor( const QColor& bgColor )
{
	m_bg_color = bgColor;
}

QColor OverlaySettingsWidget::backgroundColor() const
{
	return m_bg_color;
}

void OverlaySettingsWidget::setLink( int link )
{
	ui->cbLink->setCurrentIndex( link );
}

int OverlaySettingsWidget::link() const
{
	return ui->cbLink->currentIndex();
}

void OverlaySettingsWidget::saveSettings( QSettings& settings )
{}

void OverlaySettingsWidget::loadSettings( QSettings& settings )
{}
