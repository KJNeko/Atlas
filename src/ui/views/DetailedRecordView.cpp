//
// Created by kj16609 on 3/29/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DetailedRecordView.h" resolved

#include "DetailedRecordView.hpp"

#include <QDateTime>

#include <tracy/Tracy.hpp>

#include "ui_DetailedRecordView.h"

DetailedRecordView::DetailedRecordView( QWidget* parent ) : QWidget( parent ), ui( new Ui::DetailedRecordView )
{
	ui->setupUi( this );
}

DetailedRecordView::~DetailedRecordView()
{
	delete ui;
}

void DetailedRecordView::setRecord( const Record record_in )
{
	ZoneScoped;
	m_record = record_in;

	const auto& record { m_record.value() };
	ui->lbGameName->setText( record->getTitle() );

	if ( record->getLastPlayed() == 0 )
	{
		ui->lbLastPlayed->setText( "Last Played:\nNever" );
	}
	else
	{
		//Convert UNIX timestamp to QDateTime
		const QDateTime date {
			QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( record->getLastPlayed() ), Qt::LocalTime )
		};
		ui->lbLastPlayed->setText( QString( "Last Played:\n%1" ).arg( date.toString() ) );
	}
}

void DetailedRecordView::clearRecord()
{
	m_record = std::nullopt;
}
