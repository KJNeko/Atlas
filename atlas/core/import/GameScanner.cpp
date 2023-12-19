//
// Created by kj16609 on 6/5/23.
//

#include "GameScanner.hpp"

#include <moc_GameScanner.cpp>

#include <QFuture>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include <queue>
#include <ranges>

#include "core/database/RapidTransaction.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/database/remote/F95Data.hpp"
#include "core/gamelist/utils.hpp"
#include "core/utils/FileScanner.hpp"
#include "core/utils/engineDetection/engineDetection.hpp"
#include "core/utils/foldersize.hpp"
#include "core/utils/regex/regex.hpp"
#include "core/utils/threading/pools.hpp"

std::optional< gl::GameListInfos > findGLInfo( std::filesystem::path folder )
{
	ZoneScopedN( "Scan for GL data" );
	//Check if we have a GL_Infos.ini file
	if ( gl::dirHasGLInfo( folder ) )
	{
		atlas::logging::debug( "Found GL info for {}", folder );
		//We have one.
		return gl::parse( folder / GL_INFO_FILENAME );
	}
	else
		return {};
}

void runner(
	QPromise< GameImportData >& promise,
	const QString regex,
	const std::filesystem::path folder,
	const std::filesystem::path base,
	const bool size_files )
{
	ZoneScoped;
	if ( promise.isCanceled() ) return;
	atlas::utils::FileScanner scanner { folder };
	const std::vector< std::filesystem::path > potential_executables { detectExecutables( scanner ) };

	atlas::logging::debug( "Importing folder {} with base {} using regex {}", folder, base, regex );

	if ( promise.isCanceled() ) return;
	if ( potential_executables.size() <= 0 ) throw NoExecutablesFound( folder );

	const auto gl_info { findGLInfo( folder ) };
	const auto regex_output { regex::extractGroups( regex, QString::fromStdString( folder.string() ) ) };

	const bool gl_has_f95_thread { gl_info.has_value() && gl_info->f95_thread_id != INVALID_F95_ID };

	std::optional< atlas::remote::F95RemoteData > f95_data { std::nullopt };
	std::optional< atlas::remote::AtlasRemoteData > atlas_data { std::nullopt };

	if ( gl_has_f95_thread )
	{
		ZoneScopedN( "Check for remote data - GLInfos.ini" );
		const F95ID f95_id { gl_info->f95_thread_id };

		f95_data = atlas::remote::findF95Data( f95_id );

		if ( f95_data.has_value() ) atlas_data = atlas::remote::findAtlasData( f95_data.value()->atlas_id );
	}

	//Did GL have a f95 thread id?
	if ( !atlas_data.has_value() )
	{
		ZoneScopedN( "Check for remote data" );
		//Nope. Try the alternative search method.
		const auto& [ title, creator, version, engine ] = regex_output;
		atlas_data = atlas::remote::findAtlasData( title, creator );
	}

	regex::GroupsOutput final_output { regex_output };

	auto& [ title, creator, version, engine ] = final_output;

	if ( atlas_data.has_value() )
	{
		auto& a_data { atlas_data.value() };
		title = a_data->title;
		creator = a_data->creator;
		version = a_data->version;
		engine = a_data->engine;
	}

	//Search for banners
	std::array< QString, BannerType::SENTINEL > banners {};

	{
		ZoneScopedN( "Scan for banners" );
		for ( const auto& file : scanner )
		{
			if ( promise.isCanceled() ) return;
			if ( file.depth > 1 ) break;

			const auto& path { file.path };
			const auto& stem { path.filename().stem() };

			if ( stem == "banner" )
			{
				banners[ Normal ] = QString::fromStdString( path.string() );
			}
			else if ( stem == "banner_w" )
			{
				banners[ Wide ] = QString::fromStdString( path.string() );
			}
			else if ( stem == "logo" )
			{
				banners[ Logo ] = QString::fromStdString( path.string() );
			}
			else if ( stem == "cover" )
			{
				banners[ Cover ] = QString::fromStdString( path.string() );
			}
			else
				continue;
		}

		//Check if images are available locally, if not, get the url
		//Download image so we can store it later

		if ( atlas_data.has_value() )
		{
			banners[ Normal ] = f95_data.value()->banner_url;
		}
	}

	std::vector< QString > previews;

	{
		ZoneScopedN( "Scan for previews" );
		if ( std::filesystem::exists( folder / "previews" ) )
		{
			for ( const auto& file : std::filesystem::directory_iterator( folder / "previews" ) )
			{
				if ( promise.isCanceled() ) return;
				if ( file.is_regular_file() ) previews.emplace_back( QString::fromStdString( file.path().string() ) );
			}
		}
	}

	std::uint64_t file_size { 0 };
	std::uint64_t file_count { 0 };

	if ( size_files )
	{
		//Size up files
		for ( const auto& file : scanner )
		{
			++file_count;
			file_size += file.size;
		}
	}

	// Fetch the game_id from the DB. Will return INVALID_RECORD_ID if not found
	const auto game_id { atlas::records::fetchRecord( title, creator, engine ) };
	//If the gl_info has a f95_id then we can use that.
	auto atlas_id { INVALID_ATLAS_ID };

	if ( engine.isEmpty() )
	{
		//Set engine if it's not set already via the regex
		engine = engineName( determineEngine( scanner ) );
	}

	if ( promise.isCanceled() ) return;
	GameImportData data {
		std::move( folder ),
		std::move( title ),
		std::move( creator ),
		std::move( engine ),
		version.isEmpty() ? "0.0" : std::move( version ),
		file_size,
		file_count,
		potential_executables,
		potential_executables.at( 0 ),
		std::move( banners ),
		std::move( previews ),
		gl_info.has_value() ? std::move( gl_info.value() ) : gl::GameListInfos(),
		game_id,
		atlas_id,
	};

	promise.addResult( std::move( data ) );
}

void GameScanner::
	mainRunner( QPromise< void >& promise, const std::filesystem::path base, QString pattern, const bool size_folder )
try
{
	ZoneScoped;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for( 10ms );

	std::vector< QFuture< void > > futures;

	if ( pattern.contains( '{' ) && pattern.contains( '}' ) ) pattern = regex::regexify( std::move( pattern ) );
	QRegularExpression regex { pattern };

	for ( auto itter = std::filesystem::recursive_directory_iterator( base );
	      itter != std::filesystem::recursive_directory_iterator();
	      ++itter )
	{
		ZoneScopedN( "Process directory" );

		promise.suspendIfRequested();
		if ( promise.isCanceled() )
		{
			break;
		}

		if ( itter->is_directory() )
		{
			TracyCZoneN( regex_Tracy, "Regex", true );
			const auto result { regex::valid( regex, QString::fromStdString( itter->path().string() ) ) };
			TracyCZoneEnd( regex_Tracy );

			//Is the directory we just found already in the database?

			if ( result )
			{
				std::optional< RecordID > path_id;
				RapidTransaction() << "SELECT record_id FROM versions WHERE game_path = ?" << itter->path() >> path_id;
				if ( path_id.has_value() ) continue;

				++directories_left;
				//The regex was a match. We can now process this directory further
				futures.emplace_back( QtConcurrent::
				                          run( &globalPools().pre_importers,
				                               runner,
				                               pattern,
				                               itter->path(),
				                               base,
				                               size_folder )
				                              .then(
												  [ this ]( const GameImportData data )
												  {
													  emit foundGame( data );
													  --directories_left;
												  } ) );

				if ( promise.isCanceled() ) break;
				itter.disable_recursion_pending();
				//Directory should NOT be added to found_paths to be processed later since we shouldn't continue needing to look deeper.
			}
		}
	}

	emit prescanWaiting();

	for ( auto& future : futures | std::views::reverse )
	{
		if ( promise.isCanceled() ) break;

		while ( true )
		{
			promise.suspendIfRequested();
			if ( promise.isCanceled() )
			{
				break;
			}

			if ( future.isCanceled() || future.isFinished() ) break;
			std::this_thread::yield();
			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10ms );
		}
	}

	if ( promise.isCanceled() )
	{
		//We need to cancel all the futures we have running
		for ( auto& future : futures | std::views::reverse )
		{
			future.cancel();
		}

		//Wait for them to finish
		for ( auto& future : futures )
		{
			future.waitForFinished();
		}
	}

	done = true;
}
catch ( QUnhandledException& e )
{
	if ( promise.isCanceled() ) //We don't care about the error if we're canceled
		return;

	try
	{
		std::rethrow_exception( e.exception() );
	}
	catch ( std::exception& e_2 )
	{
		atlas::logging::error( "Main runner ate error before entering Qt space! {}", e_2.what() );
	}
}
catch ( std::exception& e )
{
	atlas::logging::error( "Main runner ate error before entering Qt space! {}", e.what() );
}
catch ( ... )
{
	atlas::logging::error( "Main runner fucking died" );
}

void GameScanner::start( const std::filesystem::path path, const QString regex, const bool size_folders )
{
	ZoneScoped;

	m_runner_future =
		QtConcurrent::run( &globalPools().pre_importers, &GameScanner::mainRunner, this, path, regex, size_folders );
	if ( m_runner_future.isFinished() ) // Optimistic checking if we finished instantly.
		emitComplete();
	else
	{
		connect( &m_watcher, &QFutureWatcher< void >::finished, this, &GameScanner::emitComplete );
		m_watcher.setFuture( m_runner_future );
	}
}

void GameScanner::pause()
{
	m_runner_future.suspend();
}

void GameScanner::resume()
{
	m_runner_future.resume();
}

void GameScanner::abort()
{
	m_runner_future.cancel();
}

bool GameScanner::isRunning()
{
	return m_runner_future.isRunning() || !done;
}

bool GameScanner::isPaused()
{
	return m_runner_future.isSuspended();
}

GameScanner::~GameScanner()
{
	ZoneScoped;
	if ( m_runner_future.isRunning() ) m_runner_future.cancel();
}
