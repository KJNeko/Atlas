//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "h95/database/Record.hpp"

#include <ui/dialog/GameImportDialog.hpp>
#include <ui/dialog/SettingsDialog.hpp>
#include <h95/config.hpp>
#include <h95/database/Database.hpp>
#include <h95/logging.hpp>

#include <QFileDialog>
#include <QDirIterator>
#include <QMimeData>
#include <QDropEvent>
#include <QPixmapCache>

#include <tracy/Tracy.hpp>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	//Check if we are launching for the first time. Do extra config here.
	if ( getSettings< bool >( "first_launch", true ) )
	{
		//Do extra config for first launch here (Extra prompts, ect)
		setSettings( "first_launch", false );
	}

	// 32MB, setCacheLimit takes in KB measurements.
	QPixmapCache::setCacheLimit( 1024 * 32 );

	spdlog::info( "Cache limit set to {} KB", QPixmapCache::cacheLimit() );

	this->restoreGeometry( getSettings< QByteArray >( "main_window/geometry" ) );

	ui->listView->update();
}

MainWindow::~MainWindow()
{
	setSettings( "main_window/geometry", saveGeometry() );

	delete ui;
}

void MainWindow::on_actionCreateRecord_triggered() try
{
	ZoneScoped;

	//TODO: Ask user for 'empty record' or 'add new game'

	Transaction transaction;
	Record::create("Empty Record", "", "", {}, ":/banner/placeholder.jpg", {}, transaction);
	transaction.commit();

	update();
}
catch(RecordAlreadyExists& e)
{
	QMessageBox::information(this, "Record already exists", "An empty record already exists.");
}

void MainWindow::on_actionSettings_triggered()
{
	SettingsDialog dialog;
	dialog.exec();
}
