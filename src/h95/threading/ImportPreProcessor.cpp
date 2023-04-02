//
// Created by kj16609 on 3/28/23.
//

#include "ImportPreProcessor.hpp"

#include <QMimeDatabase>

#include <tracy/Tracy.hpp>

#include "h95/logging.hpp"
#include "h95/regex.hpp"
#include "h95/utils.hpp"
#include "h95/utils/engineDetection/engineDetection.hpp"

ImportPreProcessor::ImportPreProcessor() : QObject( nullptr )
{}

void ImportPreProcessor::processDirectory(
	const QString regex, const std::filesystem::path base, const bool move_imported, const bool skip_filesize )
{
	spdlog::debug( "Processing base directory {:ce} with regex {}", base, regex );
	//Can't use a normal for loop since we need `pop()` to lower the number of itterations this has to go through.
	for ( auto itter = std::filesystem::
	          recursive_directory_iterator( base, std::filesystem::directory_options::skip_permission_denied );
	      itter != std::filesystem::recursive_directory_iterator();
	      ++itter )
	{
		const std::filesystem::path& folder { *itter };
		if ( std::filesystem::is_directory( folder ) && valid( regex, QString::fromStdString( folder.string() ) ) )
		{
			spdlog::debug( "Folder {} passed regex. Scanning for executables", folder );
			ZoneScopedN( "Test folder for executables" );
			std::vector< std::filesystem::path > potential_executables;

			//Check for a valid game executable in the folder
			for ( const auto& file : std::filesystem::directory_iterator( folder ) )
			{
				ZoneScopedN( "Scan files" );
				if ( file.is_regular_file() )
				{
					QMimeDatabase mime_db;
					const auto type { mime_db.mimeTypeForFile(
						QString::fromStdString( file.path().string() ), QMimeDatabase::MatchContent ) };

					if ( ( type.inherits( "text/html" ) && file.path().filename() == "index.html" )
					     || ( type.inherits( "application/x-ms-dos-executable" )
					          && file.path().extension() == ".exe" ) )
					{
						spdlog::debug( "Executable {} passed check", file.path() );
						potential_executables.emplace_back( std::filesystem::relative( file, folder ) );
					}
					else
						spdlog::debug( "File {} failed check", file.path() );
				}
			}

			if ( potential_executables.size() > 0 )
			{
				ZoneScopedN( "Add to list" );
				const auto [ title, creator, version, engine ] =
					extractGroups( regex, QString::fromStdString( folder.string() ) );

				const GameImportData game_data {
					std::filesystem::relative( folder, base ),
					title,
					creator,
					engine.isEmpty() ? engineName( determineEngine( folder, potential_executables.at( 0 ) ) ) : engine,
					version,
					skip_filesize ? 0 : folderSize( folder ),
					potential_executables,
					potential_executables.at( 0 ),
					move_imported
				};
				emit finishedDirectory( game_data );
			}
			else
			{
				TracyMessageL( "No executables found" );
				spdlog::warn( "No executables found for path {}", folder );
			}

			itter.pop();
			if ( itter == std::filesystem::recursive_directory_iterator() ) break;
		}
	}
	emit finishedProcessing();
}