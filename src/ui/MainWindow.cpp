//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <backend/config.hpp>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	if(getSettings<bool>("first_launch", true))
	{
		qDebug() << "First launch";
		setSettings("first_launch", false);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}
