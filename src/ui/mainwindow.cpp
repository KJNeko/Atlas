#include "mainwindow.h"

#include "./dialog/batchImportDialog.h"
#include "./ui_mainwindow.h"
#include "./dialog/SettingsDialog.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->actionImport, SIGNAL(triggered()),this,SLOT(openBatchImportDialog()));
    //ui->actionImport->triggered()

    //Check db first, if nothing is there add default

    //default
    addTreeRoot("Games", "0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_triggered()
{
    batchImportDialog biDialog;
    //connect( &biDialog, SIGNAL( importComplete() ), ui->recordView, SLOT( refresh() ), Qt::SingleShotConnection );
    biDialog.exec();
}

void MainWindow::addTreeRoot(QString name, QString record_id)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidget *treeWidget = this->ui->gamesTree;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeWidget);

    treeItem->setText(0, name);
    addTreeChild(treeItem, "Test Game", record_id);
}

void MainWindow::addTreeChild(QTreeWidgetItem *parent,
                          QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}

void MainWindow::on_actionOptions_triggered()
{
	SettingsDialog *settingsDialog { new SettingsDialog( this ) };

	settingsDialog->exec();
	settingsDialog->deleteLater();
}
