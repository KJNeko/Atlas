//
// Created by kj16609 on 5/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SimpleImporter.h" resolved

#include "SimpleImporter.hpp"

#include <QComboBox>
#include <QMenu>

#include "SIModel.hpp"
#include "ui_SimpleImporter.h"

SimpleImporter::SimpleImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::SimpleImporter )
{
	ui->setupUi( this );

	ui->dirView->setModel( new SIModel() );
	ui->dirView->setContextMenuPolicy( Qt::CustomContextMenu );

	//connect( ui->dirView, &QTreeView::customContextMenuRequested, this, &SimpleImporter::onCustomContextMenuRequested );
	connect(
		ui->dirView->selectionModel(),
		&QItemSelectionModel::selectionChanged,
		this,
		&SimpleImporter::dirView_itemSelectionChanged );
}

SimpleImporter::~SimpleImporter()
{
	delete ui;
}

void SimpleImporter::setRoot( const QString path )
{
	static_cast< SIModel* >( ui->dirView->model() )->setRoot( path );
}

void SimpleImporter::dirView_itemSelectionChanged(
	[[maybe_unused]] const QItemSelection& selected, [[maybe_unused]] const QItemSelection& deselected )
{}
