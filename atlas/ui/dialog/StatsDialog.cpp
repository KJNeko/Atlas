//
// Created by kj16609 on 5/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StatsDialog.h" resolved

#include "StatsDialog.hpp"

#include <moc_StatsDialog.cpp>

#include <QChart>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QTime>
#include <QValueAxis>

#include "core/database/Database.hpp"
#include "core/database/GameMetadata.hpp"
#include "core/database/record/Record.hpp"
#include "ui_StatsDialog.h"

StatsDialog::StatsDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::StatsDialog )
{
	ui->setupUi( this );

	Transaction transaction {};

	QChart* chart { new QChart };
	QLineSeries* size_series { new QLineSeries };

	std::int64_t counter { 0 };

	transaction << "SELECT timestamp, delta FROM data_change ORDER BY timestamp ASC" >>
		[ & ]( const std::uint64_t timestamp, const std::int64_t delta )
	{
		counter += delta;
		size_series
			->append( static_cast< qreal >( timestamp ) * 1000.0, static_cast< qreal >( counter / 1000 / 1000 ) );
	};

	chart->addSeries( size_series );
	chart->setTitle( "Game size over time" );

	QDateTimeAxis* axisX { new QDateTimeAxis };
	axisX->setMin( QDateTime( QDate( 2022, 1, 1 ), QTime( 00, 00 ) ) );
	axisX->setFormat( "dd-MM-yyyy h:mm ap" );
	axisX->setTitleText( "Date" );
	chart->addAxis( axisX, Qt::AlignBottom );
	size_series->attachAxis( axisX );

	QValueAxis* axisY { new QValueAxis };
	axisY->setLabelFormat( "%i" );
	axisY->setTitleText( "Size (MB)" );
	chart->addAxis( axisY, Qt::AlignLeft );
	size_series->attachAxis( axisY );

	ui->graphicsView->setChart( chart );
}

StatsDialog::~StatsDialog()
{
	delete ui;
}