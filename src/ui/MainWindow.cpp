//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "h95/config.hpp"
#include "ui/dialog/GameImportDialog.hpp"
#include "h95/database/database.hpp"

#include <QFileDialog>
#include <QDirIterator>

#include <tracy/Tracy.hpp>

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
	ZoneScoped;
	GameImportDialog dialog;
	connect( &dialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
	dialog.exec();
}

void MainWindow::on_actionMassAddImages_triggered()
{
	ZoneScoped;
	QFileDialog dialog { this };
	dialog.setFileMode( QFileDialog::Directory );
	if ( !dialog.exec() )
		return;
	else
	{
		std::vector< std::filesystem::path > paths;
		const auto dirs { dialog.selectedFiles() };
		paths.reserve( static_cast< decltype( paths )::size_type >( dirs.size() ) );
		for ( const auto& dir : dirs )
		{
			QDirIterator iterator { dir, QDirIterator::Subdirectories };

			while ( iterator.hasNext() )
			{
				const auto file { iterator.nextFileInfo() };
				if ( file.isFile() ) paths.emplace_back( file.filePath().toStdString() );
			}
		}

		std::cout << "Importing records" << std::endl;

		for ( size_t i = 0; i < paths.size(); ++i )
		{
			std::cout << i << std::endl;

			const auto placeholder { QString::fromStdString( std::to_string( i ) ) };
			Record::create(
				placeholder,
				placeholder,
				placeholder,
				placeholder,
				{ placeholder.toStdString(), placeholder.toStdString() },
				paths.at( i ),
				{} );
		}

		ui->recordView->refresh();
	}
}
