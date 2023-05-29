//
// Created by kj16609 on 3/29/23.
//
#include "DetailedRecordView.hpp"

#include "ui_DetailedRecordView.h"

DetailedRecordView::DetailedRecordView( QWidget* parent ) : QWidget( parent ), ui( new Ui::DetailedRecordView )
{
	ui->setupUi( this );
}

DetailedRecordView::~DetailedRecordView()
{
	delete ui;
}

void DetailedRecordView::setRecord( const Record record )
{
	ui->gameView->setRecord( record );
}
