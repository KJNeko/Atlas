//
// Created by kj16609 on 6/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GLImporter.h" resolved

#include "GLImporter.hpp"

#include "ui_GLImporter.h"

GLImporter::GLImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::GLImporter )
{
	ui->setupUi( this );
}

GLImporter::~GLImporter()
{
	delete ui;
}
