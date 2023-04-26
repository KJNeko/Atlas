//
// Created by kj16609 on 4/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordEditor.h" resolved

#include "RecordEditor.hpp"

#include "ui_RecordEditor.h"

RecordEditor::RecordEditor( QWidget* parent ) : QDialog( parent ), ui( new Ui::RecordEditor )
{
	ui->setupUi( this );
}

RecordEditor::~RecordEditor()
{
	delete ui;
}
