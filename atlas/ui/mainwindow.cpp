#include "mainwindow.h"

#include <moc_mainwindow.cpp>

#include "core/config/config.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/images/import.hpp"
#include "core/import/ImportNotifier.hpp"
#include "core/notifications/notifications.hpp"
#include "core/remote/AtlasRemote.hpp"
#include "core/utils/mainThread/mainThread.hpp"
#include "core/version.hpp"
#include "ui/dialog/AboutAtlas.hpp"
#include "ui/dialog/SettingsDialog.hpp"
#include "ui/dialog/StatsDialog.hpp"
#include "ui/dialog/aboutqtdialog.h"
#include "ui/importer/batchImporter/BatchImportDialog.hpp"
#include "ui/importer/extractionImporter/ExtractionImportDialog.hpp"
#include "ui/importer/simpleImporter/SimpleImporter.hpp"
#include "ui/importer/singleImporter/SingleImporter.hpp"
#include "ui/views/gamelist/GameListDelegate.hpp"
#include "ui_mainwindow.h"
#include "widgets/FilterWidget.hpp"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );
	readSettings();

	utils::setMainThread( this->thread() );

	//Check db first, if nothing is there add default
	//default
	connect( ui->SearchBox, &QLineEdit::textChanged, this, &MainWindow::searchTextChanged );
	connect( this, &MainWindow::triggerSearch, &record_search, &Search::searchTextChanged );
	connect( &record_search, &Search::searchCompleted, ui->recordView, &RecordListView::setRecords );

	connect( ui->recordView, &RecordListView::openDetailedView, this, &MainWindow::switchToDetailed );
	//connect( ui->homeButton, &QToolButton::clicked, this, &MainWindow::on_homeButton_pressed );
	//connect( ui->btnAddGame, &QPushButton::clicked, this, &MainWindow::on_btnAddGame_pressed );
	//connect( ui->btnFilter, &QToolButton::clicked, this, &MainWindow::on_btnFilter_pressed );

	//if ( config::geometry::main_window::hasValue() ) restoreGeometry( config::geometry::main_window::get() );
	MainWindow::resize( QSize( config::grid_ui::windowWidth::get(), config::grid_ui::windowHeight::get() ) );

	record_search.moveToThread( &search_thread );
	search_thread.start();

	//hide search icon
	//ui->searchIconSmall->hide();
	ui->NavTop->hide();

	//Set Font
	QFont font {};
	font.setPointSize( config::application::fontSize::get() );
	font.setFamily(
		config::application::font::get() == "" ? QApplication::font().defaultFamily() :
												 config::application::font::get() );

	QApplication::setFont( font );

	//IMPORTANT set the font for banner images here if it was never set
	config::grid_ui::font::
		set( config::grid_ui::font::get() == "" ? QApplication::font().defaultFamily() : config::grid_ui::font::get() );

	//Notification Stuff
	config::notify();

	atlas::notifications::initNotifications( this );
	connect( &atlas::notifications::handle(), &NotificationManagerUI::requestMove, this, &MainWindow::movePopup );
	ui->btnLog->setText( "Hide Log" );

	//Share the recordView's model to gameList
	//NEED TO OVERIDE THIS TO SET HEADER DATA
	ui->recordView->model()->setHeaderData( 0, Qt::Horizontal, "Games", 2 );

	ui->gamesTree->setModel( ui->recordView->model() );
	ui->gamesTree->setItemDelegate( new GameListDelegate() );
	ui->gamesTree->setHeaderHidden( false );

	emit triggerSearch( "" ); //, SortOrder::Name, true );

	//Init remote system
	atlas::initRemoteHandler();

	atlas::notifications::createUserMessage( QString( "Welcome to atlas! Version: %1" )
	                                             .arg( utils::version_string_qt() ) );

	//Make sure mouse tracking is enabled for view
	ui->recordView->setMouseTracking( true );
	//ui->gamesTree->model()->setHeaderData( 1, Qt::Horizontal, QString( "Games" ) );

	//DISABLE FEATURES NOT USED IN .5BETA
	ui->actionArrangeBy->setVisible( false );
	ui->actionCoverView->setVisible( false );
	ui->actionListView->setVisible( false );
	ui->actionManage->setVisible( false );
	ui->actionSimpleImporter->setVisible( false );
	ui->actionSingleImporter->setVisible( false );
	ui->actionGameListImporter->setVisible( false );

	//Fix scrolling
	ui->recordView->verticalScrollBar()->setSingleStep( 25 );

	connect(
		&atlas::import::internal::getNotifier(),
		&atlas::import::ImportNotifier::notification,
		this,
		&MainWindow::setBottomGameCounter );
	connect(
		&atlas::import::internal::getNotifier(),
		&atlas::import::ImportNotifier::notification,
		this,
		&MainWindow::refreshSearch );

	setBottomGameCounter();
	refreshSearch();

	const QString windowTitle = QString::fromStdString( "ATLAS " ) + utils::version_string_qt();
	MainWindow::setWindowTitle( windowTitle );

	console->setModal( true );
}

MainWindow::~MainWindow()
{
	search_thread.exit();
	config::geometry::main_window::set( saveGeometry() );
	delete ui;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	config::geometry::main_window::set( saveGeometry() );
	config::state::main_window::set( saveState() );
	QMainWindow::closeEvent( event );
}

void MainWindow::readSettings()
{
	restoreGeometry( config::geometry::main_window::get() );
	restoreState( config::state::main_window::get() );
}

void MainWindow::on_actionSimpleImporter_triggered()
{
	QMessageBox::information( this, "Importer", "Please select the game directory" );
	if ( const auto dir = QFileDialog::getExistingDirectory(
			 this, "Open directory", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	     !dir.isEmpty() )
	{
		SimpleImporter importer { this };
		importer.setRoot( dir );
		importer.exec();
		//Trigger search
	}
	else
		QMessageBox::information( this, "Error", "No directory provided." );
}

void MainWindow::on_actionSingleImporter_triggered()
{
	SingleImporter importer { this };
	importer.exec();
}

void MainWindow::on_actionExtractionImporter_triggered()
{
	ExtractionImportDialog importer { this };
	importer.exec();
}

void MainWindow::on_actionBulkImporter_triggered()
{
	BatchImportDialog importer { this };
	importer.exec();
}

void MainWindow::on_btnFilter_pressed()
{
	FilterWidget filterWIdget { this };
	filterWIdget.show();
}

void MainWindow::on_actionOptions_triggered()
{
	atlas::logging::debug( "Settings menu requested" );
	SettingsDialog settingsDialog { this };
	settingsDialog.setModal( true );
	settingsDialog.exec();

	ui->recordView->reloadConfig();

	QWidget::repaint();
}

void MainWindow::switchToDetailed( const atlas::records::Game record )
{
	atlas::logging::debug( "Switched to detailed view for game {}", record->m_title );
	ui->detailedRecordView->setRecord( record );
	ui->stackedWidget->setCurrentIndex( 1 );
}

void MainWindow::on_homeButton_pressed()
{
	ui->stackedWidget->setCurrentIndex( 0 );
}

void MainWindow::on_btnAddGame_pressed()
{
	SingleImporter importer { this };
	importer.exec();
}

void MainWindow::resizeEvent( QResizeEvent* event )
{
	QMainWindow::resizeEvent( event );

	//Store window height and width
	config::grid_ui::windowHeight::
		set( config::grid_ui::windowHeight::get() != MainWindow::height() ? MainWindow::height() :
	                                                                        config::grid_ui::windowHeight::get() );
	config::grid_ui::windowWidth::
		set( config::grid_ui::windowWidth::get() != MainWindow::width() ? MainWindow::width() :
	                                                                      config::grid_ui::windowWidth::get() );
	//config::grid_ui::itemViewWidth::set( ui->recordView->viewport()->width() );
	//config::grid_ui::itemViewHeight::set( ui->recordView->viewport()->height() );

	//ui->recordView->reloadConfig();

	movePopup();
}

void MainWindow::showEvent( [[maybe_unused]] QShowEvent* event )
{
	QWidget::showEvent( event );

	movePopup();
}

void MainWindow::on_actionExit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionAboutQt_triggered()
{
	AboutQtDialog aboutQtDialog { this };
	aboutQtDialog.setModal( true );
	aboutQtDialog.exec();
}

void MainWindow::on_actionAboutAtlas_triggered()
{
	AboutAtlas aboutAtlas { this };
	aboutAtlas.setModal( true );
	aboutAtlas.exec();
}

void MainWindow::on_actionViewFileHistory_triggered()
{
	StatsDialog dialog;
	dialog.exec();
}

void MainWindow::searchTextChanged( [[maybe_unused]] const QString str )
{
	atlas::logging::debug( "Search text changed to {}", str );
	/*const auto search_type = [ & ]()
	{
		switch ( ui->sortSelection->currentIndex() )
		{
			default:
				[[fallthrough]];
			case 0:
				return SortOrder::Name;
			case 1:
				return SortOrder::Creator;
			case 2:
				return SortOrder::Engine;
			case 3:
				return SortOrder::Time;
		
	}();}*/

	//emit triggerSearch( str, search_type, ui->sortOrderButton->text() == "ASC" );
	emit triggerSearch( std::move( str ) );
}

/*void MainWindow::on_sortOrderButton_clicked()
{
	if ( ui->sortOrderButton->text() == "ASC" )
		ui->sortOrderButton->setText( "DESC" );
	else
		ui->sortOrderButton->setText( "ASC" );

	searchTextChanged( ui->SearchBox->text() );8
}*/

/*void MainWindow::on_sortSelection_currentIndexChanged( [[maybe_unused]] int index )
{
	searchTextChanged( ui->SearchBox->text() );
}*/

void MainWindow::moveEvent( QMoveEvent* event )
{
	QMainWindow::moveEvent( event );
	movePopup();
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
	if ( ui->stackedWidget->currentIndex() == 1 ) ui->stackedWidget->setCurrentIndex( 0 );

	return QMainWindow::keyPressEvent( event );
}

void MainWindow::movePopup()
{
	auto& task_popup { atlas::notifications::handle() };
	const auto [ x, y ] = task_popup.size();

	const auto point { ui->recordView->mapToGlobal( ui->recordView->rect().bottomRight() ) - QPoint { x, y } };

	task_popup.move( point );

	//Check if the popup is hidden since we call this once when we turn visible <=> invisible.
	ui->btnLog->setText( task_popup.isVisible() ? "Hide Log" : "Show Log" );

	//spdlog::info( "Max height of popup{}", ui->recordView->height() );
	//task_popup.setMaximumHeight( ui->recordView->height() );
}

void MainWindow::taskPopupResized()
{
	movePopup();
}

void MainWindow::setBottomGameCounter()
{
	std::uint_fast32_t unique_games { 0 };
	std::uint_fast32_t total_versions { 0 };

	// We need to filter out our testing dummy record sowe just get rid of id 1
	RapidTransaction() << "SELECT COUNT(*) FROM games WHERE record_id != ?" << 1 >> unique_games;
	RapidTransaction() << "SELECT COUNT(*) FROM games NATURAL JOIN versions WHERE record_id != ?" << 1
		>> total_versions;

	ui->GamesInstalled
		->setText( QString( "%1 games installed, %2 total versions" ).arg( unique_games ).arg( total_versions ) );

	//repaint if new games are added
}

void MainWindow::refreshSearch()
{
	searchTextChanged( ui->SearchBox->text() );
}

void MainWindow::on_stackedWidget_currentChanged( const int idx )
{
	if ( idx == 0 ) ui->detailedRecordView->clearRecord();
}

void MainWindow::on_btnLog_pressed()
{
	auto& task_popup { atlas::notifications::handle() };
	task_popup.setHidden( !task_popup.isHidden() );
	ui->btnLog->setText( task_popup.isHidden() ? "Show Log" : "Hide Log" );
}

void MainWindow::on_actionUpdates_triggered()
{
#ifdef _WIN32
	atlas::initUpdateHandler( true );
#endif
}

void MainWindow::on_actionConsoleWindow_triggered()
{
	console->exec();
}

void MainWindow::on_actionUpdateMeta_triggered()
{
	bool download_all_images = false;

	QMessageBox msgBox( this );
	msgBox.setWindowFlags( Qt::FramelessWindowHint | Qt::Dialog ); //Dont show title or exit button
	msgBox.setWindowTitle( "Update Metadata/Images" );
	msgBox.setText( tr( "Please select an update option below" ) );
	QAbstractButton* pbutton1 = msgBox.addButton( tr( "All metadata" ), QMessageBox::YesRole );
	QAbstractButton* pbutton2 = msgBox.addButton( tr( "Missing metadata" ), QMessageBox::YesRole );
	pbutton1->setFixedSize( QSize( 150, 75 ) );
	pbutton1->setToolTip( "This will update every game matched in the ATLAS database. All images will be replaced" );
	pbutton2->setFixedSize( QSize( 150, 75 ) );

	msgBox.exec();

	if ( msgBox.clickedButton() == pbutton1 )
	{
		download_all_images = true;
	}

	//This will only update values that are already matched in the database.
	atlas::logging::info( "Updating Meta and Downloading Images" );
	//Get a list of every record id
	std::vector< RecordID > record_ids;
	RapidTransaction() << "SELECT record_id FROM games" >> [ &record_ids ]( RecordID record_id )
	{ record_ids.push_back( record_id ); };

	//Go through each record and runs checks based on user input
	for ( auto& record_id : record_ids )
	{
		atlas::records::Game game { record_id };
		std::optional< atlas::remote::AtlasRemoteData > atlas_data {
			atlas::remote::findAtlasData( game->m_title, game->m_creator )
		};
		if ( atlas_data.has_value() )
		{
			//const atlas::remote::AtlasRemoteData& atlas_data { game->atlas_data.value() };
			const AtlasID atlas_id { atlas_data.value()->atlas_id };

			std::optional< atlas::remote::F95RemoteData > f95_data {
				atlas::remote::findF95Data( QString::number( atlas_id ) )
			};

			if ( !f95_data.has_value() ) continue;

			const QUrl image_url( f95_data.value()->banner_url );
			const F95ID f95_id { f95_data.value()->f95_id };

			if ( image_url.isEmpty() ) continue; // No URL to import
			qInfo() << "Updating Images for " << atlas_data.value()->title;
			//Check if we should download all images or not. Check if this is a new item and update the image if not
			if ( download_all_images || !game->atlas_data.has_value() )
			{
				//Store data if this is a new entry
				if ( !game->atlas_data.has_value() )
				{
					game.connectAtlasData( atlas_id );
					game.connectF95Data( f95_id );
				}
				atlas::images::async::importImageFromURL( image_url.toString(), game.id() )
					.then(
						[ record_id ]( std::filesystem::path path )
						{
							atlas::records::Game game_r { record_id };
							if ( !path.empty() )
							{
								game_r.setBanner( path.string(), Normal );
							}
						}

					);
			}
		}
		else
		{
			//qInfo() << game->m_title;
		}
	}
}