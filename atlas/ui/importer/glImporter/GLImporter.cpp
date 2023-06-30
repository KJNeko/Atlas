//
// Created by kj16609 on 6/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GLImporter.h" resolved

#include "GLImporter.hpp"

#include <QtConcurrent>

#include <core/gamelist/utils.hpp>

#include "core/database/Transaction.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/import/Importer.hpp"
#include "core/utils/engineDetection/engineDetection.hpp"
#include "ui_GLImporter.h"

void GLImporterRunner::processGame( const std::filesystem::path path )
{
	spdlog::info( "Processing game directory {}", path );
	const auto info_path { path / "GL_Infos.ini" };
	if ( !std::filesystem::exists( info_path ) ) //Huh?
		return;

	const auto info { gl::parse( info_path ) };

	//Import game using f95 info
	//Fetch info from f95 data set
	AtlasID internal_id { 0 };
	RapidTransaction() << "SELECT atlas_id FROM f95_zone_data NATURAL JOIN atlas_data WHERE f95_id = ?" << info.f95_id
		>> internal_id;

	if ( internal_id == 0 )
	{
		spdlog::warn( "Missing data for thread {}", info.f95_id );
		emit message( QString::fromStdString( fmt::format( "Missing remote data for thread {}", info.f95_id ) ) );
		return;
	}

	//We found a link! Time to use it
	AtlasData data { internal_id };

	FileScanner scanner { path };
	const auto executables { detectExecutables( scanner ) };
	//Try to find an executable at the path

	if ( executables.size() == 0 )
	{
		spdlog::error( "Failed to import {:c}. No executables found?", path );
		emit message( QString::fromStdString( fmt::format( "Failed to import {:c}. No executables found?", path ) ) );
	}

	//Use this data to import the game
	auto future { importGame(
		path,
		scoreExecutables( executables ).front(),
		data.get< "title" >(),
		data.get< "creator" >(),
		data.get< "engine" >(),
		info.version,
		{},
		{} ) };

	emit message( QString::fromStdString( fmt::format( "Successfully imported game {}", path ) ) );

	future.waitForFinished();
}

void GLImporterRunner::importGLGames( const std::filesystem::path path )
{
	//Start scanning all files for GL info items
	auto itter = std::filesystem::recursive_directory_iterator( path );

	while ( itter != std::filesystem::recursive_directory_iterator() )
	{
		if ( itter->is_regular_file() && itter->path().filename() == "GL_Infos.ini" )
		{
			emit processGLFolder( itter->path() );
			itter.pop();
		}
		else
			++itter;
	}

	spdlog::info( "GLImporter: Waiting on futures" );
}

GLImporterRunner::GLImporterRunner()
{
	connect( this, &GLImporterRunner::processGLFolder, this, &GLImporterRunner::processGame );
}

GLImporter::GLImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::GLImporter )
{
	ui->setupUi( this );

	connect( this, &GLImporter::startImport, &runner, &GLImporterRunner::importGLGames );
	connect( &runner, &GLImporterRunner::message, this, &GLImporter::addMessage );
	runner.moveToThread( &m_thread );
}

GLImporter::~GLImporter()
{
	delete ui;
}

void GLImporter::setImportDir( const std::filesystem::path path )
{
	emit startImport( path );
}

void GLImporter::addMessage( const QString msg )
{
	ui->textBrowser->setText( ui->textBrowser->toPlainText() + "\n" + msg );
}
