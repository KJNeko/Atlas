//
// Created by kj16609 on 6/2/23.
//

#include "Importer.hpp"

#include <QFuture>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include "GameImportData.hpp"
#include "ImportNotifier.hpp"
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"
#include "core/imageManager.hpp"
#include "core/notifications/notifications.hpp"
#include "core/utils/FileScanner.hpp"
#include "core/utils/operators.hpp"
#include "core/utils/threading/pools.hpp"

namespace internal
{
	void importGame(
		QPromise< RecordID >& promise, GameImportData data, const std::filesystem::path import_root, const bool owning )
	try
	{
		ZoneScoped;

		auto
			[ relative_path,
		      title,
		      creator,
		      engine,
		      version,
		      game_size,
		      file_count,
		      executables,
		      relative_executable,
		      banners,
		      previews,
		      gl_infos,
		      game_id,
		      atlas_id,
		      has_version_conflict ] = std::move( data );

		ProgressSignaler signaler { QString( "Importing game %1" ).arg( title ) };

		promise.start();

		const auto game_root { import_root / relative_path };
		const auto executable_path { game_root / relative_executable };

		TracyCZoneN( tracy_checkZone, "Check", true );
		//Verify that everything is valid
		if ( !std::filesystem::exists( game_root ) )
			throw std::runtime_error( format_ns::format( "Root path {:ce} does not exist", game_root ) );
		if ( !std::filesystem::exists( executable_path ) )
			throw std::runtime_error( format_ns::format( "Executable {:ce} does not exist", executable_path ) );
		if ( title.isEmpty() ) throw std::runtime_error( "Title is empty" );
		if ( creator.isEmpty() ) throw std::runtime_error( "Creator is empty" );
		if ( version.isEmpty() ) throw std::runtime_error( "Version is empty" );
		TracyCZoneEnd( tracy_checkZone );

		TracyCZoneN( tracy_FileScanner, "File scan", true );

		//Get the size of the folder
		signaler.setProgress( 1 );
		signaler.setMax( 4 );
		signaler.setSubMessage( "Calculating folder size: 0B" );
		QLocale locale;
		atlas::utils::FileScanner scanner { game_root };

		if ( game_size == 0 )
		{
			for ( const auto& file : scanner )
			{
				game_size += file.size;

				if ( game_size % 1024 == 0 )
				{
					signaler
						.setSubMessage( QString( "Calculating folder size: %1" )
					                        .arg( locale.formattedDataSize( static_cast< qint64 >( game_size ) ) ) );
				}
			}
		}
		TracyCZoneEnd( tracy_FileScanner );

		auto record { [ & ]() -> atlas::records::Game
			          {
						  if ( game_id == INVALID_RECORD_ID )
							  return atlas::records::importRecord( title, creator, engine );
						  else
							  return atlas::records::Game( game_id );
					  }() };

		// Used for when we move files to a directory we 'own'
		const std::filesystem::path dest_root { config::paths::games::getPath() / creator / title / version };

		if ( owning )
		{
			ZoneScopedN( "Copying files" );
			signaler.setMax( static_cast< int >( file_count ) );

			std::size_t i { 0 };
			for ( const auto& file : scanner )
			{
				++i;
				const QString r_path_name { QString::fromStdString( file.relative.string() ) };
				signaler.setProgress( static_cast< int >( i ) );
				signaler
					.setSubMessage( QString( "Copying file %1 %2/%3" ).arg( r_path_name ).arg( i ).arg( file_count ) );

				const auto source { game_root / file.relative };
				const auto dest { dest_root / file.relative };
				std::filesystem::create_directories( dest.parent_path() );

				if ( !std::filesystem::copy_file( source, dest, std::filesystem::copy_options::overwrite_existing ) )
				{
					atlas::logging::error( "Failed to copy file {} to {}", source.string(), dest.string() );
					throw std::runtime_error( "Failed to copy file" );
				}
			}

			record.addVersion( version, dest_root, relative_executable, game_size, owning );
		}
		else
			record.addVersion( version, game_root, relative_executable, game_size, owning );

		if ( atlas_id != INVALID_ATLAS_ID ) record.connectAtlasData( atlas_id );
		if ( gl_infos.f95_thread_id != INVALID_F95_ID ) record.connectF95Data( gl_infos.f95_thread_id );

		std::array< std::optional< QFuture< std::filesystem::path > >, BannerType::SENTINEL > banner_futures {};

		for ( std::size_t i = 0; i < BannerType::SENTINEL; i++ )
		{
			const auto path { banners[ i ] };
			if ( !path.isEmpty() )
			{
				const std::filesystem::path banner_path { path.toStdWString() };
				banner_futures[ i ] = imageManager::importImage( banner_path, record->m_game_id );
			}
			else
				banner_futures[ i ] = { std::nullopt };

			//If the game is going into our directory then we should clean up the banners
			if ( owning )
			{
				const auto r_path { dest_root / std::filesystem::relative( { path.toStdWString() }, game_root ) };
				game_size -= std ::filesystem::file_size( r_path );
				// Remove the image file from the moved files
				std::filesystem::remove( r_path );
			}
		}

		std::vector< QFuture< std::filesystem::path > > preview_futures;

		for ( const auto& path : previews )
		{
			signaler.setSubMessage( QString( "Importing preview %1" ).arg( path ) );
			preview_futures.emplace_back( imageManager::importImage( { path.toStdWString() }, record->m_game_id ) );
			//record.addPreview( { path.toStdString() } );

			if ( owning ) //If we own it then we should delete the path from our directory
			{
				const auto r_path { dest_root / std::filesystem::relative( { path.toStdWString() }, game_root ) };
				game_size -= std ::filesystem::file_size( r_path );
				// Remove the image file from the moved files
				std::filesystem::remove( r_path );
			}
		}

		signaler.setSubMessage( QString( "Importing banners: 0/%1" ).arg( BannerType::SENTINEL - 1 ) );

		signaler.setMax( static_cast< int >( preview_futures.size() + BannerType::SENTINEL - 1 ) );

		for ( int i = 0; i < BannerType::SENTINEL; i++ )
		{
			auto opt { banner_futures[ static_cast< std::size_t >( i ) ] };

			if ( opt.has_value() )
			{
				try
				{
					const std::filesystem::path path { opt.value().result() };
					record.setBanner( path, static_cast< BannerType >( i ) );

					signaler.setSubMessage( QString( "Importing banners: %1/%2" )
					                            .arg( i )
					                            .arg( BannerType::SENTINEL - 1 ) );
				}
				catch ( const QUnhandledException& qt_e )
				{
					try
					{
						if ( qt_e.exception() ) std::rethrow_exception( qt_e.exception() );
					}
					catch ( std::exception& e )
					{
						atlas::logging::warn( "Failed to add banner to record {}: {}", record->m_game_id, e.what() );
					}
				}
			}
		}

		signaler.setSubMessage( QString( "Importing previews: %1/%2" ).arg( 0 ).arg( preview_futures.size() ) );

		std::size_t preview_counter { 0 };

		for ( auto& future : preview_futures )
		{
			try
			{
				future.waitForFinished();
				record.addPreview( future.result() );
				signaler.setSubMessage( QString( "Importing previews: %1/%2" )
				                            .arg( preview_counter++ )
				                            .arg( preview_futures.size() ) );
				signaler.setProgress( BannerType::SENTINEL - 1 + preview_counter );
			}
			catch ( const QUnhandledException& e )
			{
				//Qt is fucking stupid.
				try
				{
					if ( e.exception() ) std::rethrow_exception( e.exception() );
				}
				catch ( std::exception& e_n )
				{
					atlas::logging::error( "Failed to add preview from future: {}", e_n.what() );
				}
			}
		}

		promise.addResult( record->m_game_id );
		promise.finish();
		atlas::import::notifyImportComplete();
	}
	catch ( std::exception& e )
	{
		atlas::logging::error( "{}", e.what() );
		promise.setException( std::current_exception() );
	}

} // namespace internal

QFuture< RecordID > importGame( GameImportData data, const std::filesystem::path root, const bool owning )
{
	ZoneScoped;

	return QtConcurrent::run( &globalPools().importers, internal::importGame, std::move( data ), root, owning );
}
