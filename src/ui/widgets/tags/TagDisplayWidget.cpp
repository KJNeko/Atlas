//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TagSearchDisplayWidget.h" resolved

#include "TagDisplayWidget.hpp"
#include "ui_TagDisplayWidget.h"


TagDisplayWidget::TagDisplayWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::TagDisplayWidget )
{
	ui->setupUi( this );
}

TagDisplayWidget::~TagDisplayWidget()
{
	delete ui;
}

void TagDisplayWidget::keyPressEvent( QKeyEvent* event )
{
	QWidget::keyPressEvent( event );
}
