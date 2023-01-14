//
// Created by kj16609 on 12/28/22.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <ui/tabs/FileTab.hpp>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionCreateEmptyRecord_triggered()
{
	QWidget* tab { nullptr };

	if ( ui->tabWidget->tabText( ui->tabWidget->currentIndex() ).startsWith( "File Viewer" ) )
		tab = ui->tabWidget->currentWidget();

	if ( tab == nullptr )
	{
		for ( auto i = 0; i < ui->tabWidget->count(); ++i )
		{
			if ( ui->tabWidget->tabText( i ).startsWith( "File Viewer" ) )
			{
				tab = ui->tabWidget->widget( i );
				ui->tabWidget->setCurrentIndex( i );
			}
		}
	}

	if(tab == nullptr)
	{
		tab = new FileTab();
		ui->tabWidget->addTab(tab, "File Viewer");
	}

	FileTab* const ptr {dynamic_cast<FileTab*>(tab)};
	ptr->addBlank();
}
