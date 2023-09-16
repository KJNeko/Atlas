#include "ExtractionImportDialog.hpp"

#include <moc_ExtractionImportDialog.cpp>

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QStringList>
#include <QTableWidgetItem>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>

#include "core/database/record/Game.hpp"
#include "core/logging.hpp"
#include "ui_ExtractionImportDialog.h"

ExtractionImportDialog::ExtractionImportDialog( QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::ExtractionImportDialog )
{
	ui->setupUi( this );
	ui->btnBack->setHidden( true );

	ui->exGames->setColumnCount( 5 );
	QStringList headers { "Title", "Version", "Creator", "File", "Path", "Found in DB" };
	ui->exGames->setHorizontalHeaderLabels( headers );
}

ExtractionImportDialog::~ExtractionImportDialog()
{
	delete ui;
}

void ExtractionImportDialog::on_btnSetRoot_pressed()
{
	ZoneScoped;
	auto directory { QFileDialog::getExistingDirectory( this, "Select folder to add" ) };

	if ( directory.isEmpty() || !QFile::exists( directory ) )
		return;
	else
		ui->rootPath->setText( std::move( directory ) );
}

void ExtractionImportDialog::on_btnSetExtPath_pressed()
{
	ZoneScoped;
	auto directory { QFileDialog::getExistingDirectory( this, "Select folder to add" ) };

	if ( directory.isEmpty() || !QFile::exists( directory ) )
		return;
	else
		ui->extractionPath->setText( std::move( directory ) );
}

void ExtractionImportDialog::on_btnNext_pressed()
{
	ZoneScoped;

	//if ( import_triggered ) return;

	spdlog::debug( "next pressed" );
	if ( ui->btnNext->text() == "Import" )
	{
		//import_triggered = true;
		//importFiles();
	}
	else
	{
		//if ( search_started ) return;

		//search_started = true;
		//Verify that the path is set
		const auto& path { ui->rootPath->text() };
		if ( path.isEmpty() || !QFile::exists( path ) )
		{
			ui->statusLabel->setText( "Path not set or invalid. Please check" );
			return;
		}
		else
		{
			ui->statusLabel->setText( "" );
		}

		ui->exImportGames->setCurrentIndex( 1 );
		ui->btnBack->setEnabled( true );
		ui->btnNext->setDisabled( true );
		parseFiles( ui->rootPath->text() );
		ui->exGames->resizeColumnsToContents();

		//processFiles();
	}
}

void ExtractionImportDialog::parseFiles( const QString& path )
{
	std::array< std::string_view, 3 > exts { ".zip", ".rar", ".7z" };
	int row { 0 };
	ui->exGames->setRowCount( 50 );
	for ( const auto& p : std::filesystem::recursive_directory_iterator( path.toStdWString() ) )
	{
		const auto path_ext { p.path().extension().string() };
		for ( const auto& ext : exts )
		{
			if ( ext == path_ext )
			{
				const QString file_name { QString::fromStdString( p.path().filename().string() ) };
				const QString file_path { QString::fromStdString( p.path().string() ) };
				const QStringList qlist { parseFileName( QString::fromStdString( p.path().stem().string() ) ) };
				//"Title", "Version", "Creator", "File", "Path","Found in DB"
				QTableWidgetItem* const item0 { new QTableWidgetItem( qlist[ 0 ] ) };
				QTableWidgetItem* const item1 { new QTableWidgetItem( qlist[ 1 ] ) };
				QTableWidgetItem* const item3 { new QTableWidgetItem( file_name ) };
				QTableWidgetItem* const item4 { new QTableWidgetItem( file_path ) };

				ui->exGames->setItem( row, 0, item0 );
				ui->exGames->setItem( row, 1, item1 );
				ui->exGames->setItem( row, 3, item3 );
				ui->exGames->setItem( row, 4, item4 );
				row++;
			}
		}
	}
}

bool checkOsNames( QString s )
{
	std::array< std::string_view, 6 > arr { "pc", "win", "linux", "windows", "unc", "win64" };
	return std::find( arr.begin(), arr.end(), s.toLower().toStdString() ) != arr.end();
}

bool checkLanguages( QString s )
{
	std::array< std::string_view, 2 > arr { "japanese", "english" };
	return std::find( arr.begin(), arr.end(), s.toLower().toStdString() ) != arr.end();
}

QString addSpaces( QString s )
{
	QString tmp { "" };
	// Traverse the string
	for ( int i = 0; i < s.length(); i++ )
	{
		const QChar character { s[ i ] };
		if ( character.isUpper() )
		{
			if ( i != 0 ) tmp += " ";
		}

		tmp += s[ i ];
	}

	//Remove Underscore
	tmp.replace( '_', "" );
	return tmp;
}

QStringList findVersionType( QString s )
{
	QString title { "" };
	QString version { "" };
	const QStringList delimiters { "Final", "Episode", "Chapter", "Version", "Season" };
	bool versionFound { false };
	for ( const QString& delimiter : delimiters )
	{
		if ( s.toLower().contains( delimiter.toLower() ) )
		{
			const QStringList slist { s.split( delimiter.toLower(), Qt::KeepEmptyParts, Qt::CaseInsensitive ) };

			if ( slist.length() > 1 )
			{
				spdlog::info( "{}, {}", slist[ 0 ], slist[ 1 ] );
				version += delimiter + slist[ 1 ];
				title += slist[ 0 ];
			}
			else
			{
				spdlog::info( "{}", slist[ 0 ] );
				version += delimiter;
				title += slist[ 0 ];
			}
			versionFound = true;
		}
	}

	if ( !versionFound )
	{
		title += s;
	}

	return { title, version };
}

bool isDigit( const QString& s )
{
	bool isNum { false };
	for ( const QChar c : s )
	{
		if ( c.isDigit() )
		{
			isNum = true;
			break;
		}
	}
	return isNum;
}

QStringList parseStringByDelimiter( const QString& s, const QString& delimiter )
{
	QString title { "" };
	QString version { "" };
	const QStringList slist { s.split( delimiter ) };
	bool isVersion { false };
	//Itterate through list
	for ( int i = 0; i < slist.length(); i++ )
	{
		const auto& item { slist[ i ] };

		//Assume first item will always be a part of the title.
		//Check if it has a number or has an OS name
		if ( !checkOsNames( item ) && !checkLanguages( item ) )
		{
			if ( !( i > 0 && isDigit( item ) ) && isVersion == false )
			{
				//Check if string contaings a version type

				auto version_result { findVersionType( item ) };

				title += std::move( version_result[ 0 ] );
				version += std::move( version_result[ 1 ] );
				//Check for version chapter or season
			}
			else
			{
				isVersion = true;
				version += item;
			}
		}
	}
	//Add Spaces between Capital Letters before returning the title
	return { addSpaces( title ), version };
}

QStringList ExtractionImportDialog::parseFileName( const QString& s )
{
	//Set Defaults
	QStringList file_data { "", "" };
	if ( s.contains( '-' ) )
	{
		file_data = parseStringByDelimiter( s, "-" );
	}
	else if ( s.contains( '_' ) )
	{
		file_data = parseStringByDelimiter( s, "_" );
	}
	else if ( s.contains( " " ) )
	{
		file_data = parseStringByDelimiter( s, " " );
	}

	return { file_data[ 0 ], file_data[ 1 ] };
}
