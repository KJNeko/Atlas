//
// Created by kj16609 on 1/15/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecordView.h" resolved

#include "RecordView.hpp"
#include "ui_RecordView.h"


RecordView::RecordView( QWidget* parent ) : QWidget( parent ), ui( new Ui::RecordView )
{
	ui->setupUi( this );

	// Connect backend to view
	connect(
		&backend,
		SIGNAL( recordsUpdated( std::vector< Record >& ) ),
		ui->recordView,
		SLOT( recordsUpdated( std::vector< Record >& ) ) );

	//Connect backend to tag display
	connect(
		&backend,
		SIGNAL( tagsChanged( std::vector< Tag >& ) ),
		ui->tagView,
		SLOT( tagsChanged( std::vector< Tag >& ) ) );
	connect( ui->tagView, SIGNAL( selected( Tag& ) ), &backend, SLOT( removeTag( Tag& ) ) );

	//Connect to search
	connect(
		&backend,
		SIGNAL( similarTagFinished( std::vector< Tag >& ) ),
		ui->tagSearch,
		SLOT( similarTagFinished( std::vector< Tag >& ) ) );
	connect(
		ui->tagSearch,
		SIGNAL( searchSimilar( QString& ) ),
		&backend,
		SLOT( searchSimilar( QString& ) ) );
	connect( ui->tagSearch->view(), SIGNAL( selected( Tag& ) ), &backend, SLOT( addTag( Tag& ) ) );

	//Connect intermediate pieces
	connect(
		ui->recordView,
		SIGNAL( recordSelected( Record& ) ),
		ui->selectedView,
		SLOT( recordSelected( Record& ) ) );
}

RecordView::~RecordView()
{
	delete ui;
}
