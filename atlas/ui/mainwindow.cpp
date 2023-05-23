#include "mainwindow.h"

#include <QtConcurrent>

#include "./dialog/BatchImportDialog.hpp"
#include "./dialog/SettingsDialog.hpp"
#include "./dialog/StatsDialog.hpp"
#include "./dialog/aboutqtdialog.h"
#include "./ui_mainwindow.h"
#include "core/config.hpp"
#include "ui/notifications/NotificationMessage.hpp"
#include "ui/notifications/NotificationPopup.hpp"
#include "ui/notifications/ProgressMessage.hpp"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	//Check db first, if nothing is there add default
	//default
	addTreeRoot( "Games", "0" );

	connect( ui->SearchBox, &QLineEdit::textChanged, this, &MainWindow::searchTextChanged );
	connect( this, &MainWindow::triggerSearch, &record_search, &Search::searchTextChanged );
	connect( &record_search, &Search::searchCompleted, ui->recordView, &RecordView::setRecords );

	connect( ui->recordView, &RecordView::openDetailedView, this, &MainWindow::switchToDetailed );
	connect( ui->homeButton, &QToolButton::clicked, this, &MainWindow::on_homeButton_pressed );

	//if ( config::geometry::main_window::hasValue() ) restoreGeometry( config::geometry::main_window::get() );
	MainWindow::resize( QSize( config::grid_ui::windowWidth::get(), config::grid_ui::windowHeight::get() ) );

	record_search.moveToThread( &search_thread );
	search_thread.start();

	//hide search icon
	ui->searchIconSmall->hide();

	config::notify();

	emit triggerSearch( "", SortOrder::Name, true );

	initNotificationPopup( this );
	getNotificationPopup()->hide();
	connect( getNotificationPopup(), &::NotificationPopup::popupResized, this, &MainWindow::movePopup );

	getNotificationPopup()->createNotification< NotificationMessage >( QString( "Welcome to Atlas!" ), true );

	(void)QtConcurrent::run(
		[]()
		{
			std::unique_ptr< ProgressMessageSignaler > signaler {
				getNotificationPopup()->createNotification< ProgressMessage >( QString( "Test!" ), true )
			};

			signaler->setMax( 100 );
			signaler->setProgress( 0 );
			signaler->setMessage( QString( "Test!" ) );
			for ( int i = 0; i < 100; ++i )
			{
				signaler->setProgress( i );
				std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
			}
		} );

	(void)getNotificationPopup()->createNotification< ProgressMessage >( QString( "Test 2!" ), true );

	for ( int i = 0; i < 10; ++i )
	{
		getNotificationPopup()->createNotification< NotificationMessage >( QString::number( i ) );
	}
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

	config::grid_ui::itemViewWidth::set( ui->recordView->viewport()->width() );
	config::grid_ui::itemViewHeight::set( ui->recordView->viewport()->height() );

	ui->recordView->reloadConfig();

	movePopup();
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

void MainWindow::on_actionViewFileHistory_triggered()
{
	StatsDialog dialog;
	dialog.exec();
}

void MainWindow::searchTextChanged( const QString str )
{
	const auto search_type = [ & ]()
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
		}
	}();

	emit triggerSearch( str, search_type, ui->sortOrderButton->text() == "ASC" );
}

void MainWindow::on_sortOrderButton_clicked()
{
	if ( ui->sortOrderButton->text() == "ASC" )
		ui->sortOrderButton->setText( "DESC" );
	else
		ui->sortOrderButton->setText( "ASC" );

	searchTextChanged( ui->SearchBox->text() );
}

void MainWindow::on_sortSelection_currentIndexChanged( [[maybe_unused]] int index )
{
	searchTextChanged( ui->SearchBox->text() );
}

void MainWindow::showMessagePopup()
{
	auto& task_popup { *getNotificationPopup() };

	if ( task_popup.isVisible() )
		task_popup.hide();
	else
	{
		task_popup.show();
		movePopup();
	}
}

void MainWindow::on_btnShowMessageLog_clicked()
{
	showMessagePopup();
}

void MainWindow::moveEvent( QMoveEvent* event )
{
	QMainWindow::moveEvent( event );
	movePopup();
}

void MainWindow::movePopup()
{
	auto& task_popup { *getNotificationPopup() };
	const auto [ x, y ] = task_popup.size();

	const auto point {
		ui->recordView->mapToGlobal( ui->recordView->rect().bottomRight() - QPoint { x, y } - QPoint( 5, 5 ) )
	};
	task_popup.move( point );
}

void MainWindow::taskPopupResized()
{
	movePopup();
}
