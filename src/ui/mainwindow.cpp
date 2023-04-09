#include "mainwindow.h"

#include "./dialog/BatchImportDialog.hpp"
#include "./dialog/SettingsDialog.hpp"
#include "./ui_mainwindow.h"
#include "atlas/config.hpp"

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );
	//connect(ui->actionImport, SIGNAL(triggered()),this,SLOT(openBatchImportDialog()));
	//ui->actionImport->triggered()

	//Check db first, if nothing is there add default

	//default
	addTreeRoot( "Games", "0" );

	connect( ui->SearchBox, &QLineEdit::textChanged, &record_search, &Search::searchTextChanged );
	connect( this, &MainWindow::triggerEmptySearch, &record_search, &Search::triggerEmptySearch );
	connect( &record_search, &Search::searchCompleted, ui->recordView, &RecordView::setRecords );

	connect( ui->recordView, &RecordView::openDetailedView, this, &MainWindow::switchToDetailed );
	connect( ui->homeButton, &QToolButton::clicked, this, &MainWindow::on_homeButton_pressed );

	if ( config::geometry::main_window::hasValue() ) restoreGeometry( config::geometry::main_window::get() );

	record_search.moveToThread( &search_thread );
	search_thread.start();

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
	QTreeWidget *treeWidget = this->ui->gamesTree;
	QTreeWidgetItem *treeItem = new QTreeWidgetItem( treeWidget );

	treeItem->setText( 0, name );
	addTreeChild( treeItem, "Test Game", record_id );
}

void MainWindow::addTreeChild( QTreeWidgetItem *parent, QString name, QString description )
{
	// QTreeWidgetItem(QTreeWidget * parent, int type = Type)
	QTreeWidgetItem *treeItem = new QTreeWidgetItem();

	// QTreeWidgetItem::setText(int column, const QString & text)
	treeItem->setText( 0, name );
	treeItem->setText( 1, description );

	// QTreeWidgetItem::addChild(QTreeWidgetItem * child)
	parent->addChild( treeItem );
}

void MainWindow::on_actionOptions_triggered()
{
	SettingsDialog *settingsDialog { new SettingsDialog( this ) };

	settingsDialog->exec();
	settingsDialog->deleteLater();

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
