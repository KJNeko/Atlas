#include "ExtractionImportDialog.hpp"

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
	QStringList headers { "Title", "Version", "Creator", "File", "Path","Found in DB" };
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
		parseFiles( ui->rootPath->text().toStdString() );
		ui->exGames->resizeColumnsToContents();

		//processFiles();
	}
}

void ExtractionImportDialog::parseFiles( std::string path )
{
	std::string ext[ 3 ] = { ".zip", ".rar", ".7z" };
	int row = 0;
	ui->exGames->setRowCount( 50 );
	for ( auto& p : std::filesystem::recursive_directory_iterator( path ) )
	{
		if ( std::find( std::begin( ext ), std::end( ext ), p.path().extension().string() ) != std::end( ext ) )
		{
			QString file_name = QString::fromStdString( p.path().filename().string() );
			QString file_path = QString::fromStdString( p.path().string() );
			QStringList qlist = parseFileName( QString::fromStdString( p.path().stem().string() ) );
			//"Title", "Version", "Creator", "File", "Path","Found in DB"
			QTableWidgetItem* item0 { new QTableWidgetItem( qlist[0] ) };
			QTableWidgetItem* item1 { new QTableWidgetItem( qlist[1] ) };
			QTableWidgetItem* item3 { new QTableWidgetItem( file_name ) };
			QTableWidgetItem* item4 { new QTableWidgetItem( file_path ) };

			ui->exGames->setItem( row, 0, item0 );
			ui->exGames->setItem( row, 1, item1 );
			ui->exGames->setItem( row, 3, item3 );
			ui->exGames->setItem( row, 4, item4 );
			row++;
		}
	}
}

QStringList ExtractionImportDialog::parseFileName( QString s )
{
	//Convert to lower case to make parsing easier
	QString tmp { s };
	//Reset title
	QString title {""};
	QString version { "" };
	//First check "-"
	if ( tmp.contains( '-' ) )
	{
		//Split strings by -
		QStringList slist = tmp.split( "-" );
		bool isVersion = false;
		//Itterate through list
		for ( int i = 0; i < slist.length(); i++ )
		{
			//Assume first item will always be a part of the title.
			//Check if it has a number or has an OS name
			if(!checkOsNames( slist[ i ] ))
			{
				if ( !( i > 0 && isDigit( slist[ i ] ) ) && isVersion == false)
				{
					//Check if string contaings a version type

					title += findVersionType(slist[ i ])[0];
					version += findVersionType(slist[ i ])[1];
					//Check for version chapter or season

				}
				else{
					isVersion = true;
					version += slist[ i ];
				}
			}
		}
		//Add Spaces between Capital Letters
		title = addSpaces( title );
	}

	return QStringList{title,version};
}

bool ExtractionImportDialog::isDigit( QString& s )
{
	bool isNum { false };
	foreach ( QChar c, s )
	{
		if ( c.isDigit() )
		{
			isNum = true;
			break;
		}
	}
	return isNum;
}

bool ExtractionImportDialog::checkOsNames( QString s )
{
	std::string arr[ 5 ] = { "pc", "win", "linux", "windows", "unc" };
	return std::find( std::begin( arr ), std::end( arr ), s.toLower().toStdString() ) != std::end( arr );
}



QString ExtractionImportDialog::addSpaces(QString s)
{
	QString tmp { "" };
    // Traverse the string
    for(int i=0; i < s.length(); i++)
    {
		
		if (s[i]>='A' && s[i]<='Z')
        {
			if ( i != 0 ) tmp += " ";
			tmp += s[ i ];
		} 
        else
		{
            tmp += s[ i ];
		}
    }

	//Remove Underscore
	tmp.replace( '_', "" );
	return tmp;
}

QStringList ExtractionImportDialog::findVersionType(QString s){
	QString title {""};
	QString version { "" };
	QStringList delimiters { "Final", "Episode", "Chapter", "Version", "Season" };
	bool versionFound { false };
	for(QString delimiter : delimiters)
	{
		if(s.toLower().contains(delimiter.toLower()))
		{	QStringList slist = s.toLower().split(delimiter.toLower());

			if(slist.length() > 1)
			{
							spdlog::info( "{}, {}", slist[0], slist[1] );
				version += delimiter + slist[ 1 ];
				title += slist[ 0 ];
			}
			else
			{
							spdlog::info( "{}", slist[0] );
				version += delimiter;
				title += slist[ 0 ];
			}
			versionFound = true;
		}		
	}

	if(!versionFound)
	{
		//title += s;
	}

	return QStringList{title,version};
}