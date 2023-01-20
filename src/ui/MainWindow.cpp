//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "h95/config.hpp"
#include "ui/dialog/GameImportDialog.hpp"
#include "h95/database/database.hpp"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	if ( getSettings< bool >( "first_launch", true ) )
	{
		qDebug() << "First launch";
		setSettings( "first_launch", false );
	}

	database::initalize();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionImportGame_triggered()
{
	GameImportDialog dialog;
	connect( &dialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
	dialog.exec();
}
