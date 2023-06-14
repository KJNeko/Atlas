//
// Created by kj16609 on 4/19/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VersionView.h" resolved

#include "VersionView.hpp"

#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_VersionView.h"

VersionView::VersionView( QWidget* parent ) : QWidget( parent ), ui( new Ui::VersionView )
{
	ZoneScoped;
	ui->setupUi( this );

	ui->btnChangeExec->setEnabled( false );
	ui->btnChangeVersion->setEnabled( false );
}

VersionView::~VersionView()
{
	delete ui;
}

void VersionView::setVersion( const std::optional< GameMetadata > metadata )
{
	ZoneScoped;
	m_metadata = metadata;

	ui->btnChangeExec->setEnabled( m_metadata.has_value() );
	ui->btnChangeVersion->setEnabled( m_metadata.has_value() );

	reloadData();
}

void VersionView::reloadData()
{
	ZoneScoped;
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

void VersionView::on_btnChangeVersion_pressed()
{
	ZoneScoped;
	if ( const auto output =
	         QInputDialog::getText( this, "Change Version", "New Version", QLineEdit::Normal, ui->versionEdit->text() );
	     !output.isEmpty() && output != ui->versionEdit->text() )
	{
		Transaction trans { Autocommit };
		std::size_t count { 0 };
		trans << "SELECT COUNT(*) FROM game_metadata WHERE version = ? AND record_id = ?" << output.toStdString()
			  << m_metadata->getParentID()
			>> count;

		if ( count > 0 )
		{
			QMessageBox::warning( this, "Duplicate version!", "A version with this name already exists!" );
			return;
		}

		this->m_metadata->setVersionName( output, trans );
	}

	reloadData();
}

void VersionView::on_btnChangeExec_pressed()
{
	ZoneScoped;
	if ( const auto file = QFileDialog::getOpenFileName(
			 this,
			 "Select executable",
			 QString::fromStdString( this->m_metadata->getPath().string() ),
			 "Executables (*.exe *.sh *.bat)" );
	     !file.isEmpty() )
	{
		const std::filesystem::path path { file.toStdString() };
		const auto rel_path { std::filesystem::relative( path, this->m_metadata->getPath() ) };

		this->m_metadata->setRelativeExecPath( rel_path );
	}
}
