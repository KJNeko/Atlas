//
// Created by kj16609 on 3/28/23.
//

#include "ImportProcessor.hpp"

#include <QApplication>

#include "h95/config.hpp"
#include "h95/database/Record.hpp"

ImportProcessor::ImportProcessor() : QObject( nullptr )
{
	thread.start();
	this->moveToThread( &thread );
}

ImportProcessor::~ImportProcessor() noexcept
{
	thread.quit();
	thread.wait( QDeadlineTimer::Forever );
}

void ImportProcessor::importGames( const std::vector< GameImportData > data, const std::filesystem::path source )
{
	emit startProgressBar();
	emit updateMax( data.size() );

	int counter { 0 };

	//God I love decomposition
	for ( auto [ path, title, creator, version, size, executables, executable, move_after_import ] : data )
	{
		emit updateText( QString( "Importing %1" ).arg( title ) );

		const auto source_folder { source / path };

		try
		{
			if ( move_after_import )
			{
				//Gather all files to copy
				std::vector< std::filesystem::path > files;
				for ( auto file : std::filesystem::recursive_directory_iterator( source_folder ) )
					if ( file.is_regular_file() ) files.emplace_back( std::move( file ) );

				//Set paths for destination
				const auto dest_root { config::paths::games::getPath() };
				const auto dest_folder { dest_root / creator.toStdString() / title.toStdString()
					                     / version.toStdString() };

				emit updateSubMax( files.size() );

				//Scan through and copy every file.
				for ( int i = 0; i < files.size(); ++i )
				{
					const auto source_path { source_folder / files.at( i ) };
					const auto dest_path { dest_folder / std::filesystem::relative( files.at( i ), source_folder ) };

					if ( !std::filesystem::exists( dest_path.parent_path() ) )
						std::filesystem::create_directories( dest_path.parent_path() );

					std::filesystem::copy( source_path, dest_path, std::filesystem::copy_options::update_existing );
					emit updateSubText( QString( "Copying files for %1: %2" )
					                        .arg( title )
					                        .arg( QString::fromStdString( source_path.filename() ) ) );

					emit updateSubValue( i );
				}

				//std::filesystem::remove( source_folder );
				path = std::filesystem::relative( dest_folder, dest_root );
			}

			GameMetadata metadata {
				std::move( version ), std::move( path ), std::move( executable ), !move_after_import, 0, 0
			};

			auto record { RecordData(
				std::move( title ),
				std::move( creator ),
				std::move( version ),
				std::uint64_t( 0 ),
				std::uint64_t( 0 ),
				{ metadata },
				{},
				{} ) };

			completed_records.emplace_back( record.getID() );

			//No crash! Yay. Continue to import
			spdlog::info( "Import succeeded with id {}", record.getID() );
			emit updateValue( ++counter );
		}
		catch ( RecordException& e )
		{
			spdlog::warn( "Something went wrong in the import thread: {}", e.what() );
			emit updateValue( ++counter );
		}
		catch ( std::exception& e )
		{
			spdlog::warn( "Something went wrong in the import thread: {}", e.what() );
			emit updateValue( ++counter );
		}
		catch ( ... )
		{
			spdlog::critical( "Something went seriously wrong in the import thread!" );
			emit updateValue( ++counter );
		}
	}

	emit closeProgressBar();
	emit importComplete();
}
