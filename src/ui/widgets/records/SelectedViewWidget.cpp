//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectedViewWidget.h" resolved

#include "SelectedViewWidget.hpp"
#include "ui_SelectedViewWidget.h"


SelectedViewWidget::SelectedViewWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::SelectedViewWidget )
{
	ui->setupUi( this );
}

SelectedViewWidget::~SelectedViewWidget()
{
	delete ui;
}
