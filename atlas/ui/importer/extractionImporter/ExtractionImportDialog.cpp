#include "ExtractionImportDialog.hpp"
#include "ui_ExtractionImportDialog.h"

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrent>
#include <QStringList>
#include <QTableWidgetItem>

#include <tracy/Tracy.hpp>

#include "core/database/record/Game.hpp"
#include "core/logging.hpp"

ExtractionImportDialog::ExtractionImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractionImportDialog)
{
    ui->setupUi(this);
    ui->btnBack->setHidden( true );

	ui->exGames->setColumnCount( 5 );
	QStringList headers { "Title", "Version", "Found in DB", "File", "Path" };
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
        else{
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

void ExtractionImportDialog::parseFiles(std::string path)
{
	std::string ext[ 3 ] = { ".zip", ".rar", ".7z" };
    int row = 0;
    ui->exGames->setRowCount( 50 );
	for ( auto& p : std::filesystem::recursive_directory_iterator(path))
    {
        if(std::find(std::begin(ext), std::end(ext), p.path().extension().string()) != std::end(ext))
        {
			QString file_name = QString::fromStdString(p.path().filename().string());
            QString file_path = QString::fromStdString(p.path().string());
			QString title = parseTitle( QString::fromStdString( p.path().stem().string() ) );
			QTableWidgetItem* item1 {new QTableWidgetItem( title )};
			QTableWidgetItem* item3 {new QTableWidgetItem( file_name )};
            QTableWidgetItem* item4 {new QTableWidgetItem( file_path )};

			ui->exGames->setItem( row, 1, item1 );
			ui->exGames->setItem( row, 3, item3 );
            ui->exGames->setItem( row, 4, item4 );
            row++;
        }

	} 

}

QString ExtractionImportDialog::parseTitle(QString title)
{
	//Convert to lower case to make parsing easier
	QString tmp { title };
	//reset title
	title = "";
	if(tmp.contains('-'))
	{
		QStringList slist = tmp.split( "-" );

		for ( int i = 0; i < slist.length();  i++)
		{
			if(!isDigit(slist[i]) && !checkOsNames(slist[i]))
			{
				title += slist[i];
			}
		}
	}

	return title;
}

QString ExtractionImportDialog::parseVersion(QString version)
{
	return version;
}

bool ExtractionImportDialog::isDigit(QString &s)
{
	bool isNum {false};
	foreach(QChar c, s) {
        if (c.isDigit()) {
			isNum = true;
			break;
		}
    }
	return isNum;
}

bool ExtractionImportDialog::checkOsNames(QString s){
	std::string arr[ 4 ] = { "pc", "win", "linux", "windows" };
	return std::find(std::begin(arr), std::end(arr), s.toLower().toStdString()) != std::end(arr);
}