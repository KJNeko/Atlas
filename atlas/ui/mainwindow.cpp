#include "mainwindow.h"

#include "./dialog/BatchImportDialog.hpp"
#include "./dialog/SettingsDialog.hpp"
#include "./dialog/aboutqtdialog.h"
#include "./ui_mainwindow.h"
#include "core/config.hpp"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	//Check db first, if nothing is there add default
	//default
	addTreeRoot( "Games", "0" );

	connect( ui->SearchBox, &QLineEdit::textChanged, &record_search, &Search::searchTextChanged );
	connect( this, &MainWindow::triggerEmptySearch, &record_search, &Search::triggerEmptySearch );
	connect( &record_search, &Search::searchCompleted, ui->recordView, &RecordView::setRecords );

	connect( ui->recordView, &RecordView::openDetailedView, this, &MainWindow::switchToDetailed );
	connect( ui->homeButton, &QToolButton::clicked, this, &MainWindow::on_homeButton_pressed );

	//if ( config::geometry::main_window::hasValue() ) restoreGeometry( config::geometry::main_window::get() );
	MainWindow::resize( QSize( config::grid_ui::windowWidth::get(), config::grid_ui::windowHeight::get() ) );

	record_search.moveToThread( &search_thread );
	search_thread.start();

	//hide search icon
	ui->searchIconSmall->hide();
	ui->NavTop->hide();

	config::notify();

	emit triggerEmptySearch();
}

MainWindow::~MainWindow()
{
	search_thread.exit();
	config::geometry::main_window::set( saveGeometry() );
	delete ui;
}

void MainWindow::on_actionImport_triggered()
{
	BatchImportDialog biDialog;
	//connect( &biDialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
	connect(
		&biDialog,
		&BatchImportDialog::importComplete,
		ui->recordView,
		&RecordView::addRecords,
		Qt::SingleShotConnection );
	biDialog.exec();
}

void MainWindow::addTreeRoot( QString name, QString record_id )
{
	// QTreeWidgetItem(QTreeWidget * parent, int type = Type)
	QTreeWidget* treeWidget = this->ui->gamesTree;
	QTreeWidgetItem* treeItem = new QTreeWidgetItem( treeWidget );

	treeItem->setText( 0, name );
	addTreeChild( treeItem, "Test Game", record_id );
}

void MainWindow::addTreeChild( QTreeWidgetItem* parent, QString name, QString description )
{
	// QTreeWidgetItem(QTreeWidget * parent, int type = Type)
	QTreeWidgetItem* treeItem = new QTreeWidgetItem();

	// QTreeWidgetItem::setText(int column, const QString & text)
	treeItem->setText( 0, name );
	treeItem->setText( 1, description );

	// QTreeWidgetItem::addChild(QTreeWidgetItem * child)
	parent->addChild( treeItem );
}

void MainWindow::on_actionOptions_triggered()
{
	SettingsDialog settingsDialog { this };
	settingsDialog.setModal( true );
	settingsDialog.exec();

	ui->recordView->reloadConfig();

	QWidget::repaint();
}

void MainWindow::switchToDetailed( const Record record )
{
	ui->detailedRecordView->setRecord( record );
	ui->stackedWidget->setCurrentIndex( 1 );
}

void MainWindow::on_homeButton_pressed()
{
	//ui->detailedRecordView->clearRecord();
	ui->stackedWidget->setCurrentIndex( 0 );
}

void MainWindow::resizeEvent( [[maybe_unused]] QResizeEvent* event )
{
	//Store window height and width
	spdlog::debug( "Window width:{} Window Height:{}", MainWindow::width(), MainWindow::height() );
	config::grid_ui::windowHeight::
		set( config::grid_ui::windowHeight::get() != MainWindow::height() ? MainWindow::height() :
	                                                                        config::grid_ui::windowHeight::get() );
	config::grid_ui::windowWidth::
		set( config::grid_ui::windowWidth::get() != MainWindow::width() ? MainWindow::width() :
	                                                                      config::grid_ui::windowWidth::get() );

	config::grid_ui::itemViewWidth::set( ui->recordView->viewport()->width() );
	config::grid_ui::itemViewHeight::set( ui->recordView->viewport()->height() );

	ui->recordView->reloadConfig();
}

void MainWindow::showEvent( [[maybe_unused]] QShowEvent* event )
{
	//Store Banner View Dims
	config::grid_ui::itemViewWidth::set( ui->recordView->viewport()->width() );
	config::grid_ui::itemViewHeight::set( ui->recordView->viewport()->height() );
	ui->recordView->reloadConfig();
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