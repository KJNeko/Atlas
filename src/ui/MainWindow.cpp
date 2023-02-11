//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "h95/config.hpp"
#include "ui/dialog/GameImportDialog.hpp"
#include "h95/database/database.hpp"
#include "ui/dialog/SettingsDialog.hpp"

#include <QFileDialog>
#include <QDirIterator>

#include <tracy/Tracy.hpp>
#include <QMimeData>
#include <QDropEvent>
#include <spdlog/spdlog.h>
#include <QPixmapCache>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	//Check if we are launching for the first time. Do extra config here.
	if ( getSettings< bool >( "first_launch", true ) )
	{
		//Do extra config for first launch here (Extra prompts, ect)

		qDebug() << "First launch";
		setSettings( "first_launch", false );
	}

	// 32MB, setCacheLimit takes in KB measurements.
	QPixmapCache::setCacheLimit( 1024 * 32 );

	spdlog::info( "Cache limit set to {} KB", QPixmapCache::cacheLimit() );

	this->restoreGeometry( getSettings< QByteArray >( "main_window/geometry" ) );
}

MainWindow::~MainWindow()
{
	setSettings( "main_window/geometry", saveGeometry() );

	delete ui;
}

void MainWindow::dragEnterEvent( QDragEnterEvent* event )
{
	// Sent hint to handle drag event
	if ( event->mimeData()->hasUrls() ) event->acceptProposedAction();
}

void MainWindow::dropEvent( QDropEvent* event )
{
	const QMimeData* mime_data { event->mimeData() };

	if ( mime_data->hasUrls() )
	{
		QStringList path_list;
		QList< QUrl > url_list { mime_data->urls() };
		for ( const auto& url : url_list )
		{
			// Handle each URL given for import dialog.
			//TODO: Replace with a multi import handler/window
			GameImportDialog dialog { url };
			connect( &dialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
			dialog.exec();
		}
	}
}

void MainWindow::on_actionImportGame_triggered()
{
	ZoneScoped;
	GameImportDialog dialog;
	connect( &dialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
	dialog.exec();
}

// Simply debug testing/stuff
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
				{ placeholder, placeholder.toStdString(), placeholder.toStdString() },
				paths.at( i ),
				{} );
		}

		ui->recordView->refresh();
	}
}

void MainWindow::on_actionSettings_triggered()
{
	SettingsDialog dialog;
	dialog.exec();

	this->ui->recordView->refresh();
}
