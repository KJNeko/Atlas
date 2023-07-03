//
// Created by kj16609 on 6/5/23.
//

#include "GameScanner.hpp"

#include <moc_GameScanner.cpp>
#include <queue>

#include <QFuture>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>

#include "core/foldersize.hpp"
#include "core/utils/FileScanner.hpp"
#include "core/utils/engineDetection/engineDetection.hpp"
#include "core/utils/regex/regex.hpp"

void runner(
	QPromise< GameImportData >& promise,
	const QString regex,
	const std::filesystem::path folder,
	const std::filesystem::path base )
{
	ZoneScoped;
	try
	{
		//promise.start();
		spdlog::debug( "Runner active for game {}", folder );
		if ( promise.isCanceled() ) return;
		FileScanner scanner { folder };
		std::vector< std::filesystem::path > potential_executables { detectExecutables( scanner ) };

		if ( promise.isCanceled() ) return;
		if ( potential_executables.size() > 0 )
		{
			const auto [ title, creator, version, engine ] =
				regex::extractGroups( regex, QString::fromStdString( folder.string() ) );

			spdlog::info( "Found game at path {}", folder );

			//Search for banners
			std::array< QString, BannerType::SENTINEL > banners {};

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

			std::vector< QString > previews;

			if ( std::filesystem::exists( folder / "previews" ) )
			{
				for ( const auto& file : std::filesystem::directory_iterator( folder / "previews" ) )
				{
					if ( promise.isCanceled() ) return;
					if ( file.is_regular_file() )
						previews.emplace_back( QString::fromStdString( file.path().string() ) );
				}
			}

			if ( promise.isCanceled() ) return;
			spdlog::info( "Adding result" );
			GameImportData data { std::filesystem::relative( folder, base ),
				                  std::move( title ),
				                  std::move( creator ),
				                  engine.isEmpty() ? engineName( determineEngine( scanner ) ) : std::move( engine ),
				                  version.isEmpty() ? "0.0" : std::move( version ),
				                  folderSize( scanner ),
				                  potential_executables,
				                  potential_executables.at( 0 ),
				                  std::move( banners ),
				                  std::move( previews ) };

			promise.addResult( std::move( data ) );
		}
		else
		{
			spdlog::warn( "No executables found for path {}", folder );
			throw std::runtime_error( fmt::format( "Failed to find executables for path {}", folder ) );
		}

		return;
	}
	catch ( const std::exception& e )
	{
		spdlog::error( "GameScanner::runner: {}", e.what() );
		promise.setException( std::current_exception() );
	}
	catch ( ... )
	{
		promise.setException( std::current_exception() );
	}
}

void GameScanner::mainRunner( QPromise< void >& promise, const std::filesystem::path base, const QString regex )
try
{
	ZoneScoped;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for( 10ms );

	std::vector< QFuture< void > > futures;

	//List of futures for all of the 'valid' directories we've found so far.
	std::queue< std::filesystem::path > to_scan {};
	to_scan.push( base );
	while ( to_scan.size() > 0 )
	{
		promise.suspendIfRequested();
		if ( promise.isCanceled() ) return;

		//List of all paths that we found nested. Will only be added to `to_scan` if the current path is NOT a valid match for the regex.
		const auto current { to_scan.front() };
		to_scan.pop();

		for ( const auto& file : std::filesystem::directory_iterator( current ) )
		{
			promise.suspendIfRequested();
			if ( promise.isCanceled() ) return;
			const std::filesystem::path& path { file.path() };
			if ( std::filesystem::is_directory( path ) )
			{
				if ( regex::valid( regex, QString::fromStdString( path.string() ) ) )
				{
					//The regex was a match. We can now process this directory further
					futures.emplace_back( QtConcurrent::run( &m_thread_pool, runner, regex, path, base )
					                          .then( [ this ]( const GameImportData data )
					                                 { emit foundGame( data ); } ) );
					if ( promise.isCanceled() ) break;
					//Directory should NOT be added to found_paths to be processed later since we shouldn't continue needing to look deeper.
				}
				else //Directory wasn't a match. But we can try searching deeper.
					to_scan.push( path );
			}
		}
	}

	spdlog::info( "Waiting for futures to finish" );

	for ( auto& future : futures )
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
	spdlog::error( "Ate error before entering Qt space! {}", e.what() );
}
catch ( ... )
{
	spdlog::error( "Ate error before entering Qt space!" );
}

void GameScanner::start( const std::filesystem::path path, const QString regex )
{
	ZoneScoped;
	m_thread_pool.setMaxThreadCount( 2 );

	m_runner_future = QtConcurrent::run( &m_thread_pool, &GameScanner::mainRunner, this, path, regex );
	if ( m_runner_future.isFinished() )
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
	if ( m_runner_future.isRunning() ) m_runner_future.cancel();

	m_thread_pool.waitForDone();
}
