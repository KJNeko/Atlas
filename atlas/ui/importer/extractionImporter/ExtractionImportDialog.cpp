#include "ExtractionImportDialog.hpp"

#include <moc_ExtractionImportDialog.cpp>

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QStringList>
#include <QComboBox>
#include <QMenu>
#include <QTableWidgetItem>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>

#include "core/database/record/game/Game.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/database/remote/F95Data.hpp"
#include "core/logging/logging.hpp"
#include "ui_ExtractionImportDialog.h"

#include <bit7z/bitarchivereader.hpp>

ExtractionImportDialog::ExtractionImportDialog( QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::ExtractionImportDialog )
{
	ui->setupUi( this );
	ui->btnBack->setHidden( true );

	ui->exGames->setColumnCount( 7 );
	QStringList headers { "id", "f95_id", "File", "Title", "Local Ver", "Remote Ver", "Creator" };
	ui->exGames->setHorizontalHeaderLabels( headers );
	ui->exGames->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->exGames, &QTableWidget::customContextMenuRequested, this, &ExtractionImportDialog::contextMenuRequested);
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
	constexpr std::array< std::string_view, 3 > exts { { ".zip", ".rar", ".7z" } };
	std::vector<std::filesystem::directory_entry> archives = {};//Empty Array

	for ( const auto& p : std::filesystem::recursive_directory_iterator( path.toStdWString() ) )
	{
		const auto path_ext { p.path().extension().string() };
		for ( const auto& ext : exts )
		{
			if ( ext == path_ext )
			{
				//store list of archives in a vector
				archives.emplace_back(p);
			}
		}
	}
	updateTable(archives);
}

void ExtractionImportDialog::updateTable (std::vector<std::filesystem::directory_entry> archives)
{	
	int row { 0 };
	for( auto p: archives)
	{
		const QString file_name { QString::fromStdString( p.path().filename().string() ) };
		const QString file_path { QString::fromStdString( p.path().string() ) };
		const QStringList qlist { parseFileName( QString::fromStdString( p.path().stem().string() ) ) };
		const QStringList executableList {findExecutables(p.path().string() )};
		//"Title", "Version", "Creator", "File", "Path","Found in DB"
		QString game_title {qlist[0]};
		QString game_version_local {qlist[1]};
		QString game_version_remote {""};
		QString game_creator {""};
		QString game_id = { "" };//Atlas ID
		QString game_f95id = {""};
		QString game_system = {""};
		QComboBox *title_list {new QComboBox};//init combo box

		//Check if item is in the database
		std::vector< atlas::remote::AtlasRemoteData > atlas_vector = atlas::remote::findAllMatchingAtlasData(game_title, "");
		//Check if vector is not empty
		if( atlas_vector.size() > 0)
		{
			if(atlas_vector.size() > 1){
				for(auto data : atlas_vector) 
				{
					std::optional <atlas::remote::AtlasRemoteData> atlas_data = data;
					//add id to title. Makes it easier to parse
					title_list->addItem(atlas_data.value()->title);
				}
				qInfo() << "Found more than 1 match";
			}
			else{
				std::optional <atlas::remote::AtlasRemoteData> atlas_data = atlas_vector[0];
				if ( atlas_data.has_value() )
				{
					std::optional <atlas::remote::F95RemoteData> f95_data = atlas::remote::findF95Data(QString::number(atlas_data.value()->atlas_id));
					game_title = atlas_data.value()->title;
					game_creator = atlas_data.value()->creator;
					game_f95id = QString::number(f95_data.value()->f95_id);
					game_id = QString::number(atlas_data.value()->atlas_id);
					game_version_remote = atlas_data.value()->version;
					qInfo() << atlas_data.value()->title;
				}
			}

		}

		QTableWidgetItem* const id_item { new QTableWidgetItem( game_id) };
		QTableWidgetItem* const f95id_item { new QTableWidgetItem( game_f95id) };
		QTableWidgetItem* const title_item { new QTableWidgetItem( game_title) };
		QTableWidgetItem* const loc_version_item { new QTableWidgetItem(game_version_local ) };
		QTableWidgetItem* const rmt_version_item { new QTableWidgetItem(game_version_remote ) };
		QTableWidgetItem* const creator_item {new QTableWidgetItem(game_creator)};
		QTableWidgetItem* const file_name_item { new QTableWidgetItem( file_name ) };
		QTableWidgetItem* const file_path_item { new QTableWidgetItem( file_path ) };

		ui->exGames->insertRow( row );
		ui->exGames->setItem( row, 0, id_item );
		ui->exGames->setItem( row, 1, f95id_item );
	    atlas_vector.size() > 1 ? ui->exGames->setCellWidget(row, 2, title_list) :
		ui->exGames->setItem( row, 2, title_item );
		ui->exGames->setItem( row, 3, file_name_item );
		ui->exGames->setItem( row, 4, loc_version_item);
		ui->exGames->setItem( row, 5, rmt_version_item );
		ui->exGames->setItem( row, 6, creator_item );
		row++;
	}
}

bool checkOsNames( QString s )
{
	constexpr std::array< std::string_view, 8 > arr { { "pc", "win", "linux", "windows", "unc", "win64","pc-lin", "mac" } };
	return std::find( arr.begin(), arr.end(), s.toLower().toStdString() ) != arr.end();
}

bool checkLanguages( QString s )
{
	constexpr std::array< std::string_view, 2 > arr { { "japanese", "english" } };
	return std::find( arr.begin(), arr.end(), s.toLower().toStdString() ) != arr.end();
}

bool checkOther( QString s )
{
	constexpr std::array< std::string_view, 4 > arr { { "compressed", "patch", "save", "saves" } };
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
				atlas::logging::debug( "{}, {}", slist[ 0 ], slist[ 1 ] );
				version += delimiter + slist[ 1 ];
				title += slist[ 0 ];
			}
			else
			{
				atlas::logging::debug( "{}", slist[ 0 ] );
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

				QList< QString > version_result { findVersionType( item ) };

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

void ExtractionImportDialog::contextMenuRequested(const QPoint& pos)
{
	QTableWidgetItem *item = ui->exGames->itemAt(pos);
	const int row = item->row();
	//qInfo() << "current row:" <<row;
    if (item) {
		QMenu *menu = new QMenu(this);
		menu->addAction(new QAction("Delete Item", this));
		//menu->addAction(new QAction("Action 2", this));
		//menu->addAction(new QAction("Action 3", this));
		menu->popup(ui->exGames->mapToGlobal(pos));
		connect(menu, &QMenu::triggered, [this, row](){ExtractionImportDialog::deleteTableItem(row);});
		//qInfo() << item.
        // do what you want with the item.
    }
}

void ExtractionImportDialog::deleteTableItem(const int row){
	ui->exGames->removeRow(row);
}

//Find executables inside of archive
QStringList ExtractionImportDialog::findExecutables(const std::string file)
{
	qInfo() << "Looking for executables " << QString::fromStdString(file);
	QStringList executables {""};

	try { // bit7z classes can throw BitException objects
		using namespace bit7z;

		//load dll from folder root
		Bit7zLibrary lib{ "7z.dll" };
		BitArchiveReader arc{ lib, file, BitFormat::Auto };

		// Printing the metadata of the archived items
		std::cout << "Archived items";
		for ( const auto& item : arc ) {
			//Only get items in root dir, skip all folders.			
			if (!item.isDir() && QString::fromStdString(item.path()).count(QLatin1Char('\\')) == 1){
				std::cout << '\n';
				std::cout << "    Name: "        << item.name() << '\n';
			}
		}
		std::cout.flush();
	} catch ( const bit7z::BitException& ex ) { qInfo() << "ERROR " <<QString::fromStdString(ex.what()); }

	return executables;
}
