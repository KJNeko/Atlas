//
// Created by kj16609 on 3/28/23.
//

#include "ImportProcessor.hpp"

#include <QMimeDatabase>

#include <tracy/Tracy.hpp>

#include "h95/regex.hpp"
#include "h95/utils.hpp"

ImportProcessor::ImportProcessor()
{
	thread.start();
	this->moveToThread( &thread );
}

ImportProcessor::~ImportProcessor() noexcept
{
	this->moveToThread( this->parent()->thread() );
	if ( thread.isRunning() ) thread.exit();
}

void ImportProcessor::
	processDirectory( const QString regex, const std::filesystem::path base, const bool move_imported )
{
	for ( auto itter = std::filesystem::
	          recursive_directory_iterator( base, std::filesystem::directory_options::skip_permission_denied );
	      itter != std::filesystem::recursive_directory_iterator();
	      ++itter )
	{
		const std::filesystem::path& folder { *itter };
		if ( std::filesystem::is_directory( folder ) && valid( regex, QString::fromStdString( folder.string() ) ) )
		{
			ZoneScopedN( "test folder" );
			std::vector< std::filesystem::path > potential_executables;

			//Check for a valid game in the folder
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
						potential_executables.emplace_back( std::filesystem::relative( file, folder ) );
				}
			}

			if ( potential_executables.size() > 0 )
			{
				ZoneScopedN( "Add to list" );
				const auto [ title, version, creator ] =
					extractGroups( regex, QString::fromStdString( folder.string() ) );

				const GameImportData game_data { std::filesystem::relative( folder, base ),
					                             title,
					                             version,
					                             creator,
					                             folderSize( folder ),
					                             potential_executables,
					                             potential_executables.at( 0 ),
					                             move_imported };
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