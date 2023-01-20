//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordViewWidget.h" resolved

#include "RecordViewWidget.hpp"
#include "ui_RecordViewWidget.h"


RecordViewWidget::RecordViewWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::RecordViewWidget )
{
	ui->setupUi( this );

	ui->listView->setItemDelegate(&delegate);
	ui->listView->setModel(&model);
}

RecordViewWidget::~RecordViewWidget()
{
	delete ui;
}

void RecordViewWidget::recordsUpdated( const std::vector< Record >& records )
{
	qDebug() << "Records updated";
	qDebug() << "Updated " << records.size() << " records";
	model.setRecords( records );
}