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

void runner(
	QPromise< GameImportData >& promise,
	const QString regex,
	const std::filesystem::path folder,
	const std::filesystem::path base,
	const bool size_files )
{
	ZoneScoped;
	try
	{
		if ( promise.isCanceled() ) return;
		atlas::utils::FileScanner scanner { folder };
		std::vector< std::filesystem::path > potential_executables { detectExecutables( scanner ) };

		if ( promise.isCanceled() ) return;
		if ( potential_executables.size() > 0 )
		{
			const auto [ title, creator, version, engine ] = [ & ]() -> regex::GroupsOutput
			{
				if ( gl::dirHasGLInfo( folder ) )
				{
					const auto gl_info { gl::parse( folder / GL_INFO_FILENAME ) };
					// Grab information from remote.

					if ( gl_info.f95_thread_id == INVALID_F95_ID )
					{
						//Unable to do anything with this
						//TODO: Try the SHORT_ID from the atlas_id stuff to see if we can get a name match from the title.
						return regex::extractGroups( regex, QString::fromStdString( folder.string() ) );
					}

					//Try to find the thread info
					if ( !atlas::remote::hasF95DataFor( gl_info.f95_thread_id ) )
					{
						return regex::extractGroups( regex, QString::fromStdString( folder.string() ) );
					}
					else
					{
						try
						{
							atlas::remote::F95RemoteData f95_data { gl_info.f95_thread_id };
							atlas::remote::AtlasRemoteData atlas_data { f95_data->atlas_id };

							//Grab version info from gl_infos directly
							regex::GroupsOutput output {
								atlas_data->title, atlas_data->creator, gl_info.version, atlas_data->engine
							};

							return output;
						}
						catch ( std::exception& e )
						{
							atlas::logging::warn( "Failed to get remote data in scanner: {}", e.what() );
							return regex::extractGroups( regex, QString::fromStdString( folder.string() ) );
						}
					}
				}
				else
				{
					return regex::extractGroups( regex, QString::fromStdString( folder.string() ) );
				}
			}();

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
			}

			std::vector< QString > previews;

			{
				ZoneScopedN( "Scan for previews" );
				if ( std::filesystem::exists( folder / "previews" ) )
				{
					for ( const auto& file : std::filesystem::directory_iterator( folder / "previews" ) )
					{
						if ( promise.isCanceled() ) return;
						if ( file.is_regular_file() )
							previews.emplace_back( QString::fromStdString( file.path().string() ) );
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

			if ( promise.isCanceled() ) return;
			GameImportData data { base,
				                  std::filesystem::relative( folder, base ),
				                  std::move( title ),
				                  std::move( creator ),
				                  engine.isEmpty() ? engineName( determineEngine( scanner ) ) : std::move( engine ),
				                  version.isEmpty() ? "0.0" : std::move( version ),
				                  file_size,
				                  file_count,
				                  potential_executables,
				                  potential_executables.at( 0 ),
				                  std::move( banners ),
				                  std::move( previews ) };

			promise.addResult( std::move( data ) );
		}
		else
		{
			atlas::logging::warn( "No executables found for path {}", folder );
			throw std::runtime_error( format_ns::format( "Failed to find executables for path {}", folder ) );
		}

		return;
	}

	catch ( const std::exception& e )
	{
		atlas::logging::error( "{}", e.what() );
		promise.setException( std::current_exception() );
	}

	catch ( ... )
	{
		promise.setException( std::current_exception() );
	}
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
		if ( promise.isCanceled() ) return;

		if ( itter->is_directory() )
		{
			TracyCZoneN( regex_Tracy, "Regex", true );
			const auto result { regex::valid( regex, QString::fromStdString( itter->path().string() ) ) };
			TracyCZoneEnd( regex_Tracy );

			//Is the directory we just found already in the database?
			RecordID path_id { INVALID_RECORD_ID };
			RapidTransaction() << "SELECT record_id FROM versions WHERE game_path = ?" << itter->path() >> path_id;
			if ( path_id != INVALID_RECORD_ID ) continue;

			if ( result )
			{
				atlas::logging::info( "Found game not already installed at: {}", itter->path() );

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

	atlas::logging::info( "Waiting for futures to finish" );

	emit prescanWaiting();

	for ( auto& future : futures | std::views::reverse )
	{
		while ( true )
		{
			promise.suspendIfRequested();
			if ( promise.isCanceled() )
			{
				future.cancel();
				future.waitForFinished();
				break;
			}

			if ( future.isCanceled() || future.isFinished() ) break;
			std::this_thread::yield();
			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10ms );
		}
	}
}
catch ( std::exception& e )
{
	atlas::logging::error( "Main runner ate error before entering Qt space! {}", e.what() );
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
	return m_runner_future.isRunning();
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
