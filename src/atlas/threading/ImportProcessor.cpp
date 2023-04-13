//
// Created by kj16609 on 3/28/23.
//

#include "ImportProcessor.hpp"

#include <QApplication>

#include "atlas/config.hpp"
#include "atlas/database/Record.hpp"
#include "atlas/foldersize.hpp"

ImportProcessor::ImportProcessor() : QObject( nullptr )
{}

void ImportProcessor::importGames(
	const std::vector< GameImportData > data, const std::filesystem::path source, const bool move_after_import )
{
	ZoneScoped;
	emit startProgressBar();
	emit updateMax( static_cast< int >( data.size() ) );

	int counter { 0 };

	//God I love decomposition
	for ( auto [ path, title, creator, engine, version, size, executables, executable ] : data )
	{
		ZoneScopedN( "Import game" );

		if ( pause_task ) pause_task.wait( pause_task );
		if ( abort_task )
		{
			abort_task = false;
			return;
		}

		emit updateText( QString( "Processing %1" ).arg( title ) );

		const auto source_folder { source / path };

		try
		{
			const bool scan_size { size == 0 };

			if ( move_after_import )
			{
				ZoneScopedN( "Copy game" );
				//Gather all files to copy
				std::vector< std::filesystem::path > files;
				for ( auto file : std::filesystem::recursive_directory_iterator( source_folder ) )
					if ( file.is_regular_file() ) files.emplace_back( std::move( file ) );

				//Set paths for destination
				const auto dest_root { config::paths::games::getPath() };
				const auto dest_folder { dest_root / creator.toStdString() / title.toStdString()
					                     / version.toStdString() };

				emit updateSubMax( static_cast< int >( files.size() ) );

				//Scan through and copy every file.
				for ( std::size_t i = 0; i < files.size(); ++i )
				{
					const auto source_path { source_folder / files.at( i ) };
					const auto dest_path { dest_folder / std::filesystem::relative( files.at( i ), source_folder ) };

					if ( !std::filesystem::exists( dest_path.parent_path() ) )
						std::filesystem::create_directories( dest_path.parent_path() );

					std::filesystem::copy( source_path, dest_path, std::filesystem::copy_options::update_existing );
					emit updateSubText( QString( "Copying: %1" )
					                        .arg( QString::fromStdString( source_path.filename().string() ) ) );

					if ( scan_size ) size += std::filesystem::file_size( source_path );

					emit updateSubValue( static_cast< int >( i ) );
				}

				path = std::filesystem::relative( dest_folder, dest_root );
			}
			else
			{
				emit updateSubText( "Scanning folder size... This could take awhile" );
				path = source_folder;
				if ( scan_size ) size = folderSize( path );
			}

			Transaction transaction { Transaction::NoAutocommit };

			Record record {
				importRecord( std::move( title ), std::move( creator ), std::move( engine ), transaction )
			};

			record->addVersion(
				std::move( version ),
				std::move( path ),
				std::move( executable ),
				!move_after_import,
				size,
				transaction );

			transaction.commit();

			if ( std::filesystem::exists( source_folder / "banner.jpg" )
			     || std::filesystem::exists( source_folder / "banner.png" ) )
			{
				const auto banner_path { std::filesystem::exists( source_folder / "banner.jpg" ) ?
					                         source_folder / "banner.jpg" :
					                         source_folder / "banner.png" };
				record->setBanner( banner_path );
			}

			completed_records.emplace_back( record->getID() );

			if ( move_after_import ) std::filesystem::remove_all( source_folder );

			//No crash! Yay. Continue to import
			spdlog::debug( "Import succeeded with id {}", record->getID() );
			emit updateValue( ++counter );
		}
		catch ( RecordException& e )
		{
			pause_task = true;
			emit importFailure(
				"Something went wrong",
				QString( "Game name: %1\nError: RecordException: \"%2\"" ).arg( title ).arg( e.what() ) );
			spdlog::warn( "Something went wrong in the import thread: RecordException:{}", e.what() );
			emit updateValue( ++counter );
		}
		catch ( std::exception& e )
		{
			pause_task = true;
			emit importFailure(
				"Something went wrong", QString( "Game name: %1\nError: \"%2\"" ).arg( title ).arg( e.what() ) );
			spdlog::warn( "Something went wrong in the import thread: {}", e.what() );
			emit updateValue( ++counter );
		}
		catch ( ... )
		{
			pause_task = true;
			emit importFailure(
				"Something went SERIOUSLY wrong",
				QString( "Something went VERY wrong during the import!\nGame name: %1\nError: Fuck" ).arg( title ) );
			spdlog::error( "Something went seriously wrong in the import thread!" );
			emit updateValue( ++counter );
		}
	}

	emit closeProgressBar();
	emit importComplete();
}

void ImportProcessor::abort()
{
	abort_task = true;
	unpause();
	spdlog::debug( "Aborting task in Processor" );
}

void ImportProcessor::unpause()
{
	pause_task = false;
	pause_task.notify_all();
	spdlog::debug( "Unblocking ImportProcessor" );
}
