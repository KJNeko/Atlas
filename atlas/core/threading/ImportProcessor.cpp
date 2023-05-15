//
// Created by kj16609 on 3/28/23.
//

#include "ImportProcessor.hpp"

#include <QApplication>

#include "core/config.hpp"
#include "core/database/Record.hpp"
#include "core/database/RecordData.hpp"
#include "core/foldersize.hpp"

ImportProcessor::ImportProcessor() : QObject( nullptr )
{}

void ImportProcessor::importGames(
	const std::vector< GameImportData > data, const std::filesystem::path source, const bool move_after_import )
{
	spdlog::debug(
		"ImportProcessor::importGames() - Starting import, source: {:ce}, move_after_import: {}",
		source,
		move_after_import );

	emit startProgressBar();
	emit updateMax( static_cast< int >( data.size() ) );

	running = true;

	int counter { 0 };

	//God I love decomposition
	for ( auto [ path, title, creator, engine, version, size, executables, executable ] : data )
	{
		if ( pause_task )
		{
			spdlog::debug( "ImportProcessor::importGames() - Pausing task" );
			pause_task.wait( pause_task );
			spdlog::debug( "ImportProcessor::importGames() - Resuming task" );
		}
		if ( abort_task )
		{
			spdlog::debug( "ImportProcessor::importGames() - Aborting task" );
			abort_task = false;
			running = false;
			return;
		}

		emit updateText( QString( "Processing %1" ).arg( title ) );

		const auto source_folder { source / path };

		try
		{
			if ( move_after_import )
			{
				spdlog::debug( "ImportProcessor::importGames() - Copying game" );
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
					try
					{
						const auto source_path { source_folder / files.at( i ) };
						const auto dest_path { dest_folder
							                   / std::filesystem::relative( files.at( i ), source_folder ) };

						if ( !std::filesystem::exists( dest_path.parent_path() ) )
							std::filesystem::create_directories( dest_path.parent_path() );

						std::filesystem::copy( source_path, dest_path, std::filesystem::copy_options::update_existing );

						emit updateSubText( QString( "Copying: %1" )
						                        .arg( QString::fromStdString( source_path.filename().string() ) ) );

						emit updateSubValue( static_cast< int >( i ) );
					}
					catch ( std::filesystem::filesystem_error& e )
					{
						spdlog::error( "ImportProcessor::importGames() - Failed to copy file: {}", e.what() );
					}
				}

				path = std::filesystem::relative( dest_folder, dest_root );
			}
			else
			{
				path = source_folder;
			}

			Transaction transaction { Transaction::NoAutocommit };

			auto record = [ & ]() -> Record
			{
				if ( !recordExists( title, creator, engine, transaction ) )
					return importRecord( std::move( title ), std::move( creator ), std::move( engine ), transaction );
				else
					return { recordID( std::move( title ), std::move( creator ), std::move( engine ), transaction ),
						     transaction };
			}();

			record->addVersion(
				std::move( version ),
				std::move( path ),
				std::move( executable ),
				size,
				!move_after_import,
				transaction );

			//Get a list of all files in base dir and iterate through them to get images

			//Get a list of all files in base dir and iterate through them to get images
			const std::vector< std::string > image_ext { ".png", ".jpg", ".jpeg", ".webp", ".bmp", ".gif" };
			for ( const auto& file : std::filesystem::directory_iterator( source_folder ) )
			{
				const auto filename { file.path().stem().string() };
				const auto ext { file.path().extension().string() };
				const bool is_image { std::find( image_ext.begin(), image_ext.end(), ext ) != image_ext.end() };

				if ( filename == "banner" && is_image )
				{
					record->setBanner( file.path(), Normal, transaction );
				}
				else if ( filename == "banner_w" && is_image )
				{
					record->setBanner( file.path(), Wide, transaction );
				}
				else if ( filename == "cover" && is_image )
				{
					record->setBanner( file.path(), Cover, transaction );
				}
				else if ( filename == "logo" && is_image )
				{
					record->setBanner( file.path(), Logo, transaction );
				}
			}

			spdlog::debug( "Adding previews" );
			if ( std::filesystem::exists( source_folder / "previews" ) )
			{
				for ( const auto& file : std::filesystem::directory_iterator( source_folder / "previews" ) )
				{
					const auto filename { file.path().stem().string() };
					const auto ext { file.path().extension().string() };
					const bool is_image { std::find( image_ext.begin(), image_ext.end(), ext ) != image_ext.end() };
					if ( is_image )
					{
						record->addPreview( file, transaction );
					}
					//emit updateSubText( QString( "Adding preview: %1" )
					//                       .arg( QString::fromStdString( file.path().filename().string() ) ) );
					//if ( file.is_regular_file() ) record->addPreview( file, transaction );
				}
			}

			spdlog::debug( "Adding to completed records" );
			completed_records.emplace_back( record->getID() );

			transaction.commit();

			spdlog::debug( "Import succeeded with id {}", record->getID() );

			if ( move_after_import )
			{
				emit updateSubText( "Deleting source folder..." );
				std::filesystem::remove_all( source_folder );
			}

			//No crash! Yay. Continue to import
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
		catch ( sqlite::sqlite_exception& e )
		{
			pause_task = true;

			const auto error_str {
				QString(
					"Game name: %1\nError: sqlite::sqlite_exception: \"%2\"\nQuery:\"%3\"\nErrorStr: \"%4\"\nExtended code: \"%5\"" )
					.arg( title )
					.arg( e.what() )
					.arg( QString::fromStdString( e.get_sql() ) )
					.arg( e.errstr() )
					.arg( e.get_extended_code() )
			};

			emit importFailure( "Something went wrong", error_str );

			spdlog::warn( "Something went wrong in the import thread: {}", error_str );
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

	running = false;
	emit closeProgressBar();
	emit importComplete();
}

void ImportProcessor::abort()
{
	if ( running )
	{
		abort_task = true;
		unpause();
		spdlog::debug( "Aborting task in Processor" );
	}
}

void ImportProcessor::unpause()
{
	if ( running )
	{
		pause_task = false;
		pause_task.notify_all();
		spdlog::debug( "Unblocking ImportProcessor" );
	}
}
