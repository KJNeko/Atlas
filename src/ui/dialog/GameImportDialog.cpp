//
// Created by kj16609 on 1/17/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameImportDialog.h" resolved

#include "GameImportDialog.hpp"
#include "ui_GameImportDialog.h"

#include <h95/config.hpp>

GameImportDialog::GameImportDialog( const QString& path, QWidget* parent ) :
  QDialog( parent ),
  m_source_path( path ),
  ui( new Ui::GameImportDialog )
{
	ui->setupUi( this );

	runAutoPopulate();
}


GameImportDialog::~GameImportDialog()
{
	delete ui;
}

void GameImportDialog::loadAutoPopulateSettings()
{
	ui->pathPattern->setText(getSettings<QString>("GameImportDialog/pathPattern", "{path}/{creator}/{title}/{version}"));
}

void GameImportDialog::runAutoPopulate()
{



}
