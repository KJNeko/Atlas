//
// Created by kj16609 on 5/26/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleImporter.h" resolved

#include "SingleImporter.hpp"

#include <moc_SingleImporter.cpp>

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "core/database/record/GameData.hpp"
#include "core/gamelist/utils.hpp"
#include "core/import/GameImportData.hpp"
#include "core/import/Importer.hpp"
#include "core/utils/regex/regex.hpp"
#include "ui_SingleImporter.h"

SingleImporter::SingleImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::SingleImporter )
{
	ui->setupUi( this );
	ui->previews->delegate()->use_thumbnils = false;
}

SingleImporter::~SingleImporter()
{
	delete ui;
}

enum Page
{
	DataEntry = 0,
	BannerPage,
	PreviewPage,
	Summary
};

void SingleImporter::on_btnNext_clicked()
{
	const auto current { static_cast< Page >( ui->stackedWidget->currentIndex() ) };

	switch ( current )
	{
		default:
			break;
		case DataEntry:
			[[fallthrough]];
		case BannerPage:
			[[fallthrough]];
		case PreviewPage:
			ui->stackedWidget->setCurrentIndex( current + 1 );
			ui->btnBack->setEnabled( true );
			break;
		case Summary:
			triggerImport();
			break;
	}

	ui->btnBack->setEnabled( true );
}

void SingleImporter::on_btnBack_clicked()
{
	const auto current { static_cast< Page >( ui->stackedWidget->currentIndex() ) };

	switch ( current )
	{
		default:
			break;
		case DataEntry:
			ui->btnBack->setEnabled( false );
			break;
		case BannerPage:
			[[fallthrough]];
		case PreviewPage:
			[[fallthrough]];
		case Summary:
			ui->stackedWidget->setCurrentIndex( current - 1 );
			break;
	}
}

void SingleImporter::on_btnCancel_clicked()
{
	if ( QMessageBox::question( this, "Are you sure?", "Are you sure you want to cancel?" ) == QMessageBox::Yes )
		return;
}

void SingleImporter::on_leTitle_textChanged( [[maybe_unused]] const QString& text )
{
	if ( ui->leCreator->text().isEmpty() || ui->leVersion->text().isEmpty() ) // Don't bother checking yet.
		return;
	verifyDataEntry();
}

void SingleImporter::on_leCreator_textChanged( [[maybe_unused]] const QString& text )
{
	if ( ui->leTitle->text().isEmpty() || ui->leVersion->text().isEmpty() ) // Don't bother checking yet.
		return;
	verifyDataEntry();
}

void SingleImporter::on_leEngine_textChanged( [[maybe_unused]] const QString& text )
{
	if ( ui->leCreator->text().isEmpty() || ui->leVersion->text().isEmpty()
	     || ui->leTitle->text().isEmpty() ) // Don't bother checking yet.
		return;
	verifyDataEntry();
}

void SingleImporter::on_leVersion_textChanged( [[maybe_unused]] const QString& text )
{
	if ( ui->leCreator->text().isEmpty() || ui->leTitle->text().isEmpty() ) // Don't bother checking yet.
		return;
	verifyDataEntry();
}

void SingleImporter::on_leRootPath_textChanged( [[maybe_unused]] const QString& text )
{
	const QDir rootDir { text };
	if ( rootDir.exists() )
	{
		ui->leExecutable->setEnabled( true );
		ui->btnSelectExec->setEnabled( true );
		ui->infoLabel->setText( "" );
		fillIn();
	}
	else
	{
		ui->leExecutable->setEnabled( false );
		ui->btnSelectExec->setEnabled( false );
		ui->infoLabel->setText( "Root directory does not exist" );
	}
}

void SingleImporter::on_leExecutable_textChanged( [[maybe_unused]] const QString& text )
{
	const QFile execFile { text };
	if ( execFile.exists() )
	{
		ui->metadataFrame->setEnabled( true );
		ui->infoLabel->setText( "" );
	}
	else
	{
		ui->btnNext->setEnabled( false );
		ui->infoLabel->setText( "Executable does not exist. Should NOT be relative" );
	}
}

void SingleImporter::on_btnSelectRoot_pressed()
{
	const QString path { QFileDialog::getExistingDirectory( this, "Select root directory", QDir::homePath() ) };

	if ( path.isEmpty() ) return;

	ui->leRootPath->setText( path );
}

void SingleImporter::on_btnSelectExec_pressed()
{
	const QString path { QFileDialog::getOpenFileName( this, "Select executable", ui->leRootPath->text() ) };

	if ( path.isEmpty() ) return;

	ui->leExecutable->setText( path );
}

void SingleImporter::on_cbUseRegex_stateChanged( int state )
{
	ui->leRegex->setEnabled( state == Qt::Checked );

	//This releases the edits from being disabled by the regex when it's unticked. They should always be locked (Except for the 'version' field)
	if ( state == Qt::Unchecked )
	{
		ui->leTitle->setEnabled( true );
		ui->leCreator->setEnabled( true );
		ui->leEngine->setEnabled( true );
		ui->leVersion->setEnabled( true );
	}
}

void SingleImporter::on_leRegex_textChanged( const QString& text )
{
	ui->leTitle->setEnabled( !text.contains( "{title}" ) );
	ui->leCreator->setEnabled( !text.contains( "{creator}" ) );
	ui->leEngine->setEnabled( !text.contains( "{engine}" ) );
	ui->leVersion->setEnabled( !text.contains( "{version}" ) );

	const auto [ title, creator, version, engine ] { regex::extractGroups( text, ui->leRootPath->text() ) };

	if ( !ui->leTitle->isEnabled() ) ui->leTitle->setText( title );
	if ( !ui->leCreator->isEnabled() ) ui->leCreator->setText( creator );
	if ( !ui->leVersion->isEnabled() ) ui->leVersion->setText( version );
	if ( !ui->leEngine->isEnabled() ) ui->leEngine->setText( engine );
}

void SingleImporter::triggerImport()
{
	std::filesystem::path root_path { ui->leRootPath->text().toStdString() };
	std::filesystem::path exec_path { ui->leExecutable->text().toStdString() };
	const auto title { ui->leTitle->text() };
	const auto creator { ui->leCreator->text() };
	const auto engine { ui->leEngine->text() };
	const auto version { ui->leVersion->text() };

	std::array< QString, BannerType::SENTINEL > banners;
	banners[ BannerType::Normal ] = ui->leBannerNormal->text();
	banners[ BannerType::Wide ] = ui->leBannerWide->text();
	banners[ BannerType::Cover ] = ui->leBannerCover->text();
	banners[ BannerType::Logo ] = ui->leBannerLogo->text();

	GameImportData game_data { std::move( root_path ),
		                       title,
		                       creator,
		                       engine,
		                       version,
		                       0,
		                       0,
		                       {},
		                       std::move( std::filesystem::relative( exec_path, root_path ) ),
		                       banners,
		                       ui->previews->pathsQString(),
		                       gl::parse( root_path / "GL_Infos.ini" ),
		                       INVALID_RECORD_ID,
		                       INVALID_ATLAS_ID

	};

	(void)importGame( std::move( game_data ), "", ui->cbMoveDir->isChecked() );

	close();
}

void SingleImporter::verifyDataEntry()
{
	ui->btnNext->setEnabled( false );

	const auto& title { ui->leTitle->text() };
	const auto& creator { ui->leCreator->text() };
	const auto& engine { ui->leEngine->text() };
	const auto& version { ui->leVersion->text() };

	if ( title.isEmpty() || creator.isEmpty() || version.isEmpty() )
	{
		ui->infoLabel->setText( "One or more required fields are empty" );
		return;
	}

	if ( atlas::records::recordExists( title, creator, engine ) )
	{
		ui->infoLabel->setText( "Record already exists with the given title, creator, and engine" );
		return;
	}

	ui->btnNext->setEnabled( true );
	ui->infoLabel->setText( "" );
}

void SingleImporter::on_stackedWidget_currentChanged( int index )
{
	switch ( static_cast< Page >( index ) )
	{
		case BannerPage:
			[[fallthrough]];
		case PreviewPage:
			[[fallthrough]];
		default:
			return;
		case DataEntry:
			verifyDataEntry();
			break;
		case Summary:
			{
				ui->summary
					->setText( QString( "You are about to do the following:\n"
				                        "Import 1 game with the following details:\n"
				                        "Title: %1\n"
				                        "Creator: %2\n"
				                        "Engine: %3\n"
				                        "Version: %4\n"
				                        "From the following directory:\n"
				                        "%5\n"
				                        "With the following executable:\n"
				                        "%6\n"
				                        "%7"
				                        "\nAre you ABSOLUTELY SURE? (Pressing next will start the import process)" )
				                   .arg( ui->leTitle->text() )
				                   .arg( ui->leCreator->text() )
				                   .arg( ui->leEngine->text() )
				                   .arg( ui->leVersion->text() )
				                   .arg( ui->leRootPath->text() )
				                   .arg( ui->leExecutable->text() )
				                   .arg(
									   ui->cbMoveDir->isChecked() ?
										   "The directory will be moved to Atlas' game directory" :
										   "" ) );
			}
	}
}

void SingleImporter::on_btnSelectNormal_clicked()
{
	const QString path { QFileDialog::getOpenFileName( this, "Select normal banner", "" ) };

	if ( !path.isEmpty() ) ui->leBannerNormal->setText( path );
}

void SingleImporter::on_btnSelectWide_clicked()
{
	const QString path { QFileDialog::getOpenFileName( this, "Select wide banner", "" ) };

	if ( !path.isEmpty() ) ui->leBannerWide->setText( path );
}

void SingleImporter::on_btnSelectCover_clicked()
{
	const QString path { QFileDialog::getOpenFileName( this, "Select cover banner", "" ) };

	if ( !path.isEmpty() ) ui->leBannerCover->setText( path );
}

void SingleImporter::on_btnSelectLogo_clicked()
{
	const QString path { QFileDialog::getOpenFileName( this, "Select logo banner", "" ) };

	if ( !path.isEmpty() ) ui->leBannerLogo->setText( path );
}

void SingleImporter::on_leBannerNormal_textChanged( const QString& text )
{
	if ( !text.isEmpty() )
	{
		const QPixmap pixmap { text };
		if ( pixmap.isNull() )
		{
			ui->bannerNormal->setText( "Loaded image was null!" );
			ui->leBannerNormal->setStyleSheet( "QLineEdit { background-color: red; }" );
		}
		else
		{
			ui->leBannerNormal->setStyleSheet( "" );
			ui->bannerNormal
				->setPixmap( pixmap.scaled( ui->bannerTabWidget->size() - QSize( 50, 70 ), Qt::KeepAspectRatio ) );
		}
	}
}

void SingleImporter::on_leBannerWide_textChanged( const QString& text )
{
	if ( !text.isEmpty() )
	{
		const QPixmap pixmap { text };
		if ( pixmap.isNull() )
		{
			ui->bannerWide->setText( "Loaded image was null!" );
			ui->leBannerWide->setStyleSheet( "QLineEdit { background-color: red; }" );
		}
		else
		{
			ui->leBannerWide->setStyleSheet( "" );
			ui->bannerWide
				->setPixmap( pixmap.scaled( ui->bannerTabWidget->size() - QSize( 50, 70 ), Qt::KeepAspectRatio ) );
		}
	}
}

void SingleImporter::on_leBannerCover_textChanged( const QString& text )
{
	if ( !text.isEmpty() )
	{
		const QPixmap pixmap { text };
		if ( pixmap.isNull() )
		{
			ui->bannerCover->setText( "Loaded image was null!" );
			ui->leBannerCover->setStyleSheet( "QLineEdit { background-color: red; }" );
		}
		else
		{
			ui->leBannerCover->setStyleSheet( "" );
			ui->bannerCover
				->setPixmap( pixmap.scaled( ui->bannerTabWidget->size() - QSize( 50, 70 ), Qt::KeepAspectRatio ) );
		}
	}
}

void SingleImporter::on_leBannerLogo_textChanged( const QString& text )
{
	if ( !text.isEmpty() )
	{
		const QPixmap pixmap { text };
		if ( pixmap.isNull() )
		{
			ui->bannerLogo->setText( "Loaded image was null!" );
			ui->leBannerLogo->setStyleSheet( "QLineEdit { background-color: red; }" );
		}
		else
		{
			ui->leBannerLogo->setStyleSheet( "" );
			ui->bannerLogo
				->setPixmap( pixmap.scaled( ui->bannerTabWidget->size() - QSize( 50, 70 ), Qt::KeepAspectRatio ) );
		}
	}
}

void SingleImporter::resizeEvent( QResizeEvent* event )
{
	QDialog::resizeEvent( event );

	//Reload banner for tab
	switch ( static_cast< BannerType >( ui->bannerTabWidget->currentIndex() ) )
	{
		case Normal:
			on_leBannerNormal_textChanged( ui->leBannerNormal->text() );
			break;
		case Wide:
			on_leBannerWide_textChanged( ui->leBannerWide->text() );
			break;
		case Cover:
			on_leBannerCover_textChanged( ui->leBannerCover->text() );
			break;
		case Logo:
			on_leBannerLogo_textChanged( ui->leBannerLogo->text() );
			break;
		default:
			[[fallthrough]];
		case SENTINEL:
			[[fallthrough]];
		case Error:
			break;
	}
}

void SingleImporter::on_bannerTabWidget_currentChanged( int index )
{
	//Reload banner for tab
	switch ( static_cast< BannerType >( index ) )
	{
		case Normal:
			on_leBannerNormal_textChanged( ui->leBannerNormal->text() );
			break;
		case Wide:
			on_leBannerWide_textChanged( ui->leBannerWide->text() );
			break;
		case Cover:
			on_leBannerCover_textChanged( ui->leBannerCover->text() );
			break;
		case Logo:
			on_leBannerLogo_textChanged( ui->leBannerLogo->text() );
			break;
		default:
			[[fallthrough]];
		case SENTINEL:
			[[fallthrough]];
		case Error:
			break;
	}
}

void SingleImporter::fillIn()
{
	const std::filesystem::path root { ui->leRootPath->text().toStdString() };

	//Check if the executable has GL_Infos.ini next to it
	if ( std::filesystem::exists( root / "GL_Infos.ini" ) )
	{
		//We have it. Parse and fill in the info
		const auto gl_data { gl::parse( root / "GL_Infos.ini" ) };

		ui->leTitle->setText( gl_data.name );
		ui->leVersion->setText( gl_data.version );

		//TODO: Use the f95 id we get here to request more information from the f95_data table
	}

	//Check if there are banners
	for ( const auto& file : std::filesystem::directory_iterator( root ) )
	{
		const auto path { file.path() };

		const auto stem { path.stem().string() };

		const auto qt_str { QString::fromStdString( path.string() ) };

		if ( stem == "banner" )
			ui->leBannerNormal->setText( qt_str );
		else if ( stem == "banner_w" )
			ui->leBannerWide->setText( qt_str );
		else if ( stem == "cover" )
			ui->leBannerCover->setText( qt_str );
		else if ( stem == "logo" )
			ui->leBannerLogo->setText( qt_str );
		else
			continue;
	}

	//Check if we have a previews folder
	std::vector< std::filesystem::path > previews;
	if ( std::filesystem::exists( root / "previews" ) )
	{
		for ( const auto& preview_path : std::filesystem::directory_iterator( root / "previews" ) )
			previews.emplace_back( preview_path );
	}

	ui->previews->setPaths( previews );
}
