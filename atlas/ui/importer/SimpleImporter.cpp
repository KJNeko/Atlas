//
// Created by kj16609 on 5/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SimpleImporter.h" resolved

#include "SimpleImporter.hpp"

#include "SIModel.hpp"
#include "ui_SimpleImporter.h"

SimpleImporter::SimpleImporter( QWidget* parent ) : QWidget( parent ), ui( new Ui::SimpleImporter )
{
	ui->setupUi( this );

	auto* model { new SIModel( this ) };
	model->setRoot( QDir::currentPath() );

	ui->treeView->setModel( model );
}

SimpleImporter::~SimpleImporter()
{
	delete ui;
}
