//
// Created by kj16609 on 3/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameTab.h" resolved

#include "GameTab.hpp"
#include "ui_GameTab.h"


GameTab::GameTab( QWidget* parent ) : QWidget( parent ), ui( new Ui::GameTab )
{
	ui->setupUi( this );

	connect( ui->searchWidget, &SearchWidget::searchComplete, ui->gameView, &GameView::setRecords );
}

GameTab::~GameTab()
{
	delete ui;
}

void GameTab::update()
{
	//Trigger search to happen
	ui->searchWidget->search();

	QWidget::update();
}
