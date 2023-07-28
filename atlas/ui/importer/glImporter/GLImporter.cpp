//
// Created by kj16609 on 6/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GLImporter.h" resolved
#include "GLImporter.hpp"

#include <moc_GLImporter.cpp>

#include <QtConcurrent>

#include <core/gamelist/utils.hpp>

#include "core/database/Transaction.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/import/Importer.hpp"
#include "core/utils/engineDetection/engineDetection.hpp"
#include "ui_GLImporter.h"

void GLImporterRunner::processGame( const std::filesystem::path root, const std::filesystem::path path )
{
	ZoneScoped;
	const auto info_path { root / path / "GL_Infos.ini" };
	if ( !std::filesystem::exists( info_path ) ) //Huh?
	{
		spdlog::error( "Path {} does not exist", info_path );
		return;
	}

	const auto info { gl::parse( info_path ) };

	//Import game using f95 info
	//Fetch info from f95 data set
	AtlasID internal_id { 0 };

	if ( !info.name.isEmpty() )
	{
		const auto search_str { info.name.simplified().toUpper().replace( " ", "" ) };

		RapidTransaction() << "SELECT atlas_id FROM atlas_data WHERE id_name LIKE ?" << search_str >> internal_id;

		if ( internal_id == 0 )
		{
			emit message( QString::fromStdString(
				fmt::format( "Failed to determine thread from name. Aborting for {}", path ) ) );
			emit failed( MissingData );
			return;
		}
	}
	else
	{
		emit message( QString::fromStdString(
			fmt::format( "Failed to determine thread. Not enough info. Aborting for {}", path ) ) );
		emit failed( MissingData );
		return;
	}

	//We found a link! Time to use it
	atlas::remote::AtlasData data { internal_id };

	atlas::utils::FileScanner scanner { root / path };
	const auto executables { detectExecutables( scanner ) };
	//Try to find an executable at the path

	if ( executables.size() == 0 )
	{
		spdlog::error( "Failed to import {}. No executables found?", path );
		emit message( QString::fromStdString( fmt::format( "Failed to import {:c}. No executables found?", path ) ) );
		emit failed( MissingExecutable );
	}

	auto [ title, creator, engine ] = data.get<
		atlas::remote::AtlasColumns::Title,
		atlas::remote::AtlasColumns::Creator,
		atlas::remote::AtlasColumns::Engine >();

	//Use this data to import the game
	try
	{
		auto future { importGame(
			path,
			scoreExecutables( executables ).front(),
			std::move( title ),
			std::move( creator ),
			std::move( engine ),
			info.version,
			{},
			{} ) };
		emit message( QString::fromStdString( fmt::format( "Successfully imported game {}", path ) ) );
		future.waitForFinished();
		emit success();
	}
	catch ( std::exception& e )
	{
		spdlog::warn( "GLImporter: {}: {}", path, e.what() );
		emit message( QString::fromStdString( fmt::format( "Failed to import {}: {}", path, e.what() ) ) );
		emit failed( Except );
	}
}

void GLImporterRunner::importGLGames( const std::filesystem::path root )
{
	ZoneScoped;
	spdlog::info( "Starting GL import at directory {}", root );
	//Start scanning all files for GL info items
	auto itter = std::filesystem::recursive_directory_iterator( root );

	while ( itter != std::filesystem::recursive_directory_iterator() )
	{
		if ( itter->is_regular_file() && itter->path().filename() == "GL_Infos.ini" )
		{
			emit processGLFolder( root, std::filesystem::relative( itter->path().parent_path(), root ) );
			itter.pop();
		}
		else
			++itter;
	}
}

GLImporterRunner::GLImporterRunner()
{
	connect( this, &GLImporterRunner::processGLFolder, this, &GLImporterRunner::processGame );
}

GLImporter::GLImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::GLImporter )
{
	ZoneScoped;
	ui->setupUi( this );

	connect( this, &GLImporter::startImport, &runner, &GLImporterRunner::importGLGames, Qt::QueuedConnection );
	connect( &runner, &GLImporterRunner::message, this, &GLImporter::addMessage, Qt::QueuedConnection );
	connect( &runner, &GLImporterRunner::success, this, &GLImporter::addSuccess, Qt::QueuedConnection );
	connect( &runner, &GLImporterRunner::failed, this, &GLImporter::addFailed, Qt::QueuedConnection );
	runner.moveToThread( &m_thread );
	m_thread.start();
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
	ZoneScoped;
	ui->textBrowser->append( msg );
}

void GLImporter::addSuccess()
{
	++success;
	updateText();
}

void GLImporter::addFailed( const FailType type )
{
	switch ( type )
	{
		default:
			[[fallthrough]];
		case Except:
			{
				++failed_exception;
				break;
			}
		case MissingData:
			{
				++failed_data;
				break;
			}
		case MissingExecutable:
			{
				++failed_exec;
				break;
			}
	}
	updateText();
}

void GLImporter::updateText()
{
	const QString success_str { QString( "%1 successful" ).arg( success ) };
	const QString failed_str { QString( "%1 failed (%2 data, %3 errors)" )
		                           .arg( failed_exception + failed_data + failed_exec )
		                           .arg( failed_data + failed_exec )
		                           .arg( failed_exception ) };

	ui->label->setText( success_str + "" + failed_str );
}
