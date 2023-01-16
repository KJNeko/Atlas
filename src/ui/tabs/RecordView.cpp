//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordView.h" resolved

#include "RecordView.hpp"
#include "ui_RecordView.h"


RecordView::RecordView( QWidget* parent ) : QWidget( parent ), ui( new Ui::RecordView )
{
	ui->setupUi( this );
}

RecordView::~RecordView()
{
	delete ui;
}
