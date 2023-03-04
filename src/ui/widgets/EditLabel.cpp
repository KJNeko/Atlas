//
// Created by kj16609 on 3/1/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EditLabel.h" resolved

#include "EditLabel.hpp"
#include "ui_EditLabel.h"


EditLabel::EditLabel( QWidget* parent ) : QWidget( parent ), ui( new Ui::EditLabel )
{
	ui->setupUi( this );
}

EditLabel::~EditLabel()
{
	delete ui;
}

void EditLabel::setText( QString text )
{
	ui->editBar->setText( std::move( text ) );
}

void EditLabel::setLabel(QString label)
{
	ui->label->setText(std::move(label));
}

QString EditLabel::text() const
{
	return ui->editBar->text();
}

