//
// Created by kj16609 on 1/13/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameTab.h" resolved

#include "FileTab.hpp"
#include "ui_FileTab.h"
#include <backend/records/RecordBase.hpp>
#include <backend/database.hpp>

FileTab::FileTab( QWidget* parent ) : QWidget( parent ), ui( new Ui::FileTab )
{
	ui->setupUi( this );
}

FileTab::~FileTab()
{
	delete ui;
}


void FileTab::addBlank()
{
	ui->gameList->addRecord(database::createEmptyRecord());
}
