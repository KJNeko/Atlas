//
// Created by kj16609 on 6/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GLImporter.h" resolved

#include "GLImporter.hpp"

#include <QtConcurrent>

#include <core/gamelist/utils.hpp>

#include "core/database/Transaction.hpp"
#include "core/database/remote/F95Data.hpp"
#include "ui_GLImporter.h"

void processGame( QPromise< void >& promise, const std::filesystem::path path, GLImporter* importer )
{
	const auto info_path { path / "GL_Infos.ini" };
	if ( !std::filesystem::exists( info_path ) ) //Huh?
		return;

	const auto info { gl::parse( info_path ) };

	//Import game using f95 info
	//Fetch info from f95 data set
	std::uint64_t internal_id { 0 };
	RapidTransaction() << "SELECT id FROM f95_data WHERE f95_id = ?" << info.f95_id >> internal_id;

	if ( internal_id == 0 ) spdlog::warn( "Missing data for thread {}", info.f95_id );

	F95Data f95_data { internal_id };
}

void importGLGames( QPromise< void >& promise, const std::filesystem::path path, GLImporter* importer )
{
	//Start scanning all files for GL info items
	auto itter = std::filesystem::recursive_directory_iterator( path );

	std::vector< QFuture< void > > futures;

	while ( itter != std::filesystem::recursive_directory_iterator() )
	{
		if ( itter->is_regular_file() && itter->path().filename() == "GL_Infos.ini" )
		{
			futures.emplace_back( QtConcurrent::run( processGame, itter->path(), importer ) );
		}
	}

	for ( QFuture< void >& future : futures )
	{
		//Spin loop to check if we are canceled or not. Cancel all the futures if we are canceled too.
		while ( !future.isFinished() )
		{
			if ( promise.isCanceled() ) future.cancel();
			std::this_thread::yield();
			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10ms );
		}
	}
}

GLImporter::GLImporter( QWidget* parent ) : QDialog( parent ), ui( new Ui::GLImporter )
{
	ui->setupUi( this );
}

GLImporter::~GLImporter()
{
	delete ui;
}

void GLImporter::setImportDir( const std::filesystem::path path )
{
	QtConcurrent::run( importGLGames, path, this );
}
