//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordViewWidget.h" resolved

#include "RecordViewWidget.hpp"
#include "ui_RecordViewWidget.h"

#include <tracy/Tracy.hpp>

#include <h95/logging.hpp>

RecordViewWidget::RecordViewWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::RecordViewWidget )
{
	ui->setupUi( this );

	ui->listView->setItemDelegate( &delegate );
	ui->listView->setModel( &model );

	// Connect selection change to record view
	connect(
		ui->listView->selectionModel(),
		SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
		this,
		SLOT( selectionChanged( QItemSelection, QItemSelection ) ) );

	connect( &delegate, &RecordViewDelegate::requestRefreshRecords, this, &RecordViewWidget::requestUpdate );
}

RecordViewWidget::~RecordViewWidget()
{
	delete ui;
}

void RecordViewWidget::selectionChanged(
	const QItemSelection& selected,
	[[maybe_unused]] const QItemSelection& deseleted )
{
	if ( selected.size() == 0 )
	{
		spdlog::debug( "Selected size is zero" );
		emit changeSelection( QPersistentModelIndex() );  //Nothing selected. So deselect everything
	}
	else
	{
		const auto indexes { selected.indexes() };
		if ( indexes.size() == 0 ) { throw std::runtime_error( "Zero indexes?" ); }
		emit changeSelection( indexes.back() );
	}
}

void RecordViewWidget::recordsUpdated( const std::vector< Record >& records )
{
	ZoneScoped;
	model.setRecords( records );
}

void RecordViewWidget::selectionHiding()
{
	ui->listView->selectionModel()->clear();
	this->show();
}
