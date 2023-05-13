//
// Created by kj16609 on 3/28/23.
//

#include "ImportPreProcessor.hpp"

#include <QMimeDatabase>
#include <QRunnable>
#include <QtConcurrent>

#include "atlas/foldersize.hpp"
#include "atlas/logging.hpp"
#include "atlas/utils/engineDetection/engineDetection.hpp"
#include "atlas/utils/regex/regex.hpp"

ImportPreProcessor::ImportPreProcessor() : QObject( nullptr )
{}

std::optional< GameImportData >
	runner( const QString regex, const std::filesystem::path folder, const std::filesystem::path base )
{
	FileScanner scanner { folder };
	std::vector< std::filesystem::path > potential_executables { detectExecutables( scanner ) };

	if ( potential_executables.size() > 0 )
	{
		const auto [ title, creator, version, engine ] =
			extractGroups( regex, QString::fromStdString( folder.string() ) );

		return { GameImportData { std::filesystem::relative( folder, base ),
			                      title,
			                      creator,
			                      engine.isEmpty() ? engineName( determineEngine( scanner ) ) : engine,
			                      version.isEmpty() ? "0.0" : version,
			                      folderSize( scanner ),
			                      potential_executables,
			                      potential_executables.at( 0 ) } };
	}
	else
		spdlog::warn( "No executables found for path {}", folder );

	return { std::nullopt };
}

void ImportPreProcessor::processDirectory( const QString regex, const std::filesystem::path base )
{
	running = true;
	spdlog::debug( "Processing base directory {:ce} with regex {}", base, regex );

	std::vector< QFuture< std::optional< GameImportData > > > futures;

	//Can't use a normal for loop since we need `pop()` to lower the number of itterations this has to go through.
	std::vector< std::filesystem::path > paths;
	paths.emplace_back( base );
	for ( size_t i = 0; i < paths.size(); ++i )
	{
		std::vector< std::filesystem::path > found_paths;
		bool found_valid_dir { false };

		for ( const auto& file : std::filesystem::directory_iterator( paths.at( i ) ) )
		{
			if ( abort_task )
			{
				abort_task = false;
				running = false;
				return;
			}

			const std::filesystem::path& folder { file.path() };
			if ( std::filesystem::is_directory( folder ) )
			{
				if ( valid( regex, QString::fromStdString( folder.string() ) ) )
				{
					spdlog::debug( "Folder {} passed regex. Scanning for executables", folder );

					futures.emplace_back( QtConcurrent::run( runner, regex, folder, base ) );
					found_valid_dir = true;
				}
				else
					found_paths.emplace_back( folder );
			}
		}

		if ( !found_valid_dir ) paths.insert( paths.end(), found_paths.begin(), found_paths.end() );
	}

	for ( auto& future : futures )
	{
		if ( abort_task )
		{
			abort_task = false;
			running = false;
			return;
		}

		future.then(
			[ this ]( const std::optional< GameImportData >& item )
			{
				if ( item.has_value() ) emit finishedDirectory( *item );
				return;
			} );
	}

	for ( auto& future : futures )
	{
		future.waitForFinished();
	}

	//if ( buffer.size() > 0 ) emit finishedDirectory( std::move( buffer ) );

	running = false;
	emit finishedProcessing();
}

void ImportPreProcessor::abort()
{
	spdlog::debug( "Aborting task in PreProcessor" );
	if ( running )
	{
		abort_task = true;
		if ( running ) running.wait( true );
	}
}
