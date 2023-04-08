//
// Created by kj16609 on 3/29/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DetailedRecordView.h" resolved

#include "DetailedRecordView.hpp"

#include <QDateTime>
#include <QGraphicsBlurEffect>
#include <QPaintEvent>
#include <QPainter>

#include <tracy/Tracy.hpp>

#include "h95/foldersize.hpp"
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

	reloadRecord();
}

void DetailedRecordView::reloadRecord()
{
	if ( m_record == nullptr ) return;
	const auto& record { *m_record };
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

	//Sum up all the file sizes in the game's folder across multiple versions
	std::size_t total_size { 0 };
	for ( const auto& version : record->getVersions() )
	{
		total_size += folderSize( version.getPath() );
	}

	const auto latest_size { record->getVersions().size() > 0 ? folderSize( record->getLatestVersion().getPath() ) :
		                                                        0 };

	const auto& locale { this->locale() };

	if ( latest_size == total_size )
		ui->lbStorageUsed->setText( QString( "Storage Used:\n%1" )
		                                .arg( locale.formattedDataSize( static_cast< qint64 >( total_size ) ) ) );
	else
		ui->lbStorageUsed->setText( QString( "Storage Used:\n%1 (%2)" )
		                                .arg(
											locale.formattedDataSize( static_cast< qint64 >( total_size ) ),
											locale.formattedDataSize( static_cast< qint64 >( latest_size ) ) ) );

	//If the record has a date/time that is larger then any of the versions then use that
	using Index = std::uint64_t;
	std::vector< std::pair< std::uint64_t, Index > > playtimes;
	playtimes.reserve( record->getVersions().size() );
	const auto& versions { record->getVersions() };
	for ( Index i = 0; i < versions.size(); ++i )
		if ( versions[ i ].getLastPlayed() > 0 ) playtimes.emplace_back( versions[ i ].getLastPlayed(), i );

	std::sort(
		playtimes.begin(), playtimes.end(), []( const auto& lhs, const auto& rhs ) { return lhs.first > rhs.first; } );

	const auto latest_playtime { playtimes.empty() ? record->getLastPlayed() : playtimes[ 0 ].first };

	if ( latest_playtime == 0 )
	{
		ui->lbLastPlayed->setText( "Last Played:\nNever" );
	}
	else
	{
		//Convert UNIX timestamp to QDateTime
		const QDateTime date {
			QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( latest_playtime ), Qt::LocalTime )
		};
		ui->lbLastPlayed->setText( QString( "Last Played:\n%1" ).arg( date.toString() ) );
	}

	ui->lbTotalPlaytime->setText(
		QString( "Total Playtime:\n%1" )
			.arg( QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( record->getTotalPlaytime() ), Qt::LocalTime )
	                  .toUTC()
	                  .toString( "hh:mm:ss" ) ) );
}

void DetailedRecordView::clearRecord()
{
	m_record = std::nullopt;
}

void DetailedRecordView::paintEvent( [[maybe_unused]] QPaintEvent* event )
{
	if ( m_record != nullptr )
	{
		QPainter painter { this };

		painter.save();

		const Record& record { *m_record };
		//Paint the banner
		const QSize size { ui->bannerFrame->size() };
		const QPixmap banner { record->getBanner( size.width(), size.height() ) };
		const QRect pixmap_rect {
			ui->bannerFrame->frameRect().center() - QPoint( banner.width() / 2, banner.height() / 2 ), banner.size()
		};
		painter.drawPixmap( pixmap_rect, banner );

		//Draw blurred version
		/*
		const auto blur_original { record->getBanner( size.width(), size.height(), true ) };
		const QRect blur_rect { ui->bannerFrame->rect().center() - QPoint( blur.width() / 2, blur.height() / 2 ),
			                    blur.size() };
		*/

		painter.restore();
	}
	QWidget::paintEvent( event );
}

GameMetadata& DetailedRecordView::selectedVersion()
{
	if ( !m_record.has_value() ) throw std::runtime_error( "selectedVersion: Record invalid" );

	if ( m_record.value()->getVersions().size() == 0 ) throw std::runtime_error( "selectedVersion: No versions available" );

	return m_record.value()->getVersions()[selected_version_idx];
}

void DetailedRecordView::on_btnPlay_pressed()
{
	selectedVersion().playGame();
}

void DetailedRecordView::on_tbSelectVersion_pressed()
{}