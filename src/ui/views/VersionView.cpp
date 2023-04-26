//
// Created by kj16609 on 4/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VersionView.h" resolved

#include "VersionView.hpp"

#include <QDateTime>

#include "ui_VersionView.h"

VersionView::VersionView( QWidget* parent ) : QWidget( parent ), ui( new Ui::VersionView )
{
	ui->setupUi( this );
}

VersionView::~VersionView()
{
	delete ui;
}

void VersionView::setVersion( const std::optional< GameMetadata > metadata )
{
	m_metadata = metadata;

	reloadData();
}

void VersionView::reloadData()
{
	if ( !m_metadata.has_value() ) return;

	const auto& mdata { m_metadata.value() };

	ui->versionEdit->setText( mdata.getVersionName() );
	ui->executableEdit->setText( QString::fromStdString( mdata.getRelativeExecPath().string() ) );

	ui->canonicalPathLabel
		->setText( QString( "Canonical path: %1" ).arg( QString::fromStdString( mdata.getPath().string() ) ) );

	const auto date_time { QDateTime::fromSecsSinceEpoch( mdata.getPlaytime() ).toString( "hh:mm:ss" ) };

	ui->playtimeLabel->setText( QString( "Playtime: %1" ).arg( date_time ) );

	if ( const auto last_playtime = mdata.getLastPlayed(); last_playtime > 0 )
	{
		const auto str { QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( last_playtime ) ).toString() };
		ui->lastPlayedLabel->setText( QString( "Last Played: %1" ).arg( str ) );
	}
	else
		ui->lastPlayedLabel->setText( "Last Played: Never" );

	ui->folderSizeLabel
		->setText( QString( "Folder Size: %1" )
	                   .arg( this->locale().formattedDataSize( static_cast< qint64 >( mdata.getFolderSize() ) ) ) );
}
