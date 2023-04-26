//
// Created by kj16609 on 3/28/23.
//

#include "ImportPreProcessor.hpp"

#include <QMimeDatabase>
#include <QRunnable>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>

#include "atlas/foldersize.hpp"
#include "atlas/logging.hpp"
#include "atlas/utils/engineDetection/engineDetection.hpp"
#include "atlas/utils/regex/regex.hpp"

ImportPreProcessor::ImportPreProcessor() : QObject( nullptr )
{}

std::optional< GameImportData >
	runner( const QString regex, const std::filesystem::path folder, const std::filesystem::path base )
{
	ZoneScoped;
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
			                      version,
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
	ZoneScoped;
	running = true;
	spdlog::debug( "Processing base directory {:ce} with regex {}", base, regex );

	std::vector< QFuture< std::optional< GameImportData > > > futures;

	//Can't use a normal for loop since we need `pop()` to lower the number of itterations this has to go through.
	for ( auto itter = std::filesystem::
	          recursive_directory_iterator( base, std::filesystem::directory_options::skip_permission_denied );
	      itter != std::filesystem::recursive_directory_iterator();
	      ++itter )
	{
		ZoneScopedN( "Process Directory" );
		if ( abort_task )
		{
			abort_task = false;
			running = false;
			return;
		}

		const std::filesystem::path& folder { *itter };
		if ( std::filesystem::is_directory( folder ) && valid( regex, QString::fromStdString( folder.string() ) ) )
		{
			ZoneScopedN( "Test folder for executables" );
			spdlog::debug( "Folder {} passed regex. Scanning for executables", folder );

			futures.emplace_back( QtConcurrent::run( runner, regex, folder, base ) );

			itter.pop();
			if ( itter == std::filesystem::recursive_directory_iterator() ) break;
		}
	}

	for ( auto& future : futures )
	{
		if ( abort_task )
		{
			abort_task = false;
			running = false;
			return;
		}

		future
			.then(
				[ this ]( const std::optional< GameImportData >& item )
				{
					if ( item.has_value() ) emit finishedDirectory( *item );
					return;
				} )
			.waitForFinished();
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
