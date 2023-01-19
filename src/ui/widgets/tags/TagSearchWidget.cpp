//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TagSearchWidget.h" resolved

#include "TagSearchWidget.hpp"
#include "ui_TagSearchWidget.h"


TagSearchWidget::TagSearchWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::TagSearchWidget )
{
	ui->setupUi( this );
}

TagSearchWidget::~TagSearchWidget()
{
	delete ui;
}

void TagSearchWidget::similarTagFinished( std::vector< Tag >& tags )
{
	ui->tagDisplay->tagsChanged(tags);
}

TagDisplayWidget* TagSearchWidget::view() const
{
	return ui->tagDisplay;
}
