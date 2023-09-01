//
// Created by kj16609 on 6/2/23.
//

#include "Importer.hpp"

#include <QFuture>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include "GameImportData.hpp"
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
		QPromise< RecordID >& promise,
		const std::filesystem::path& root,
		const std::filesystem::path& relative_executable,
		const QString& title,
		const QString& creator,
		const QString& engine,
		const QString& version,
		const std::array< QString, BannerType::SENTINEL >& banners,
		const std::vector< QString >& previews,
		std::uint64_t game_size,
		const std::uint64_t file_count,
		const bool owning,
		const AtlasID atlas_id )
	try
	{
		ZoneScoped;

		ProgressSignaler signaler { QString( "Importing game %1" ).arg( title ) };

		promise.start();

		TracyCZoneN( tracy_checkZone, "Check", true );
		//Verify that everything is valid
		if ( !std::filesystem::exists( root ) )
			throw std::runtime_error( fmt::format( "Root path {:ce} does not exist", root ) );
		if ( !std::filesystem::exists( root / relative_executable ) )
			throw std::runtime_error( fmt::format( "Executable {:ce} does not exist", root / relative_executable ) );
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
		atlas::utils::FileScanner scanner { root };

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

		auto record { atlas::records::importRecord( title, creator, engine ) };

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

				const auto source { root / file.relative };
				const auto dest { dest_root / file.relative };
				std::filesystem::create_directories( dest.parent_path() );

				if ( !std::filesystem::copy_file( source, dest, std::filesystem::copy_options::overwrite_existing ) )
				{
					spdlog::error( "importGame: Failed to copy file {} to {}", source.string(), dest.string() );
					throw std::runtime_error( "Failed to copy file" );
				}
			}

			record.addVersion( version, dest_root, relative_executable, game_size, owning );
		}
		else
			record.addVersion( version, root, relative_executable, game_size, owning );

		if ( atlas_id != INVALID_ATLAS_ID ) record.connectAtlasData( atlas_id );

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
				const auto r_path { dest_root / std::filesystem::relative( { path.toStdWString() }, root ) };
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
				const auto r_path { dest_root / std::filesystem::relative( { path.toStdWString() }, root ) };
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
						spdlog::warn( "Failed to add banner to record {}: {}", record->m_game_id, e.what() );
						atlas::notifications::createDevMessage(
							fmt::format( "Failed to add banner to record {}:{}: ", record->m_game_id, record->m_title ),
							e );
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
					atlas::notifications::createMessage( QString::fromStdString(
						fmt::format( "Failed to add preview to {} ({}): \n{}", title, version, e_n.what() ) ) );
					spdlog::warn( "Failed to add preview from future: {}", e_n.what() );
				}
			}
			catch ( std::runtime_error& e )
			{
				spdlog::warn( "Failed to add preview from future: {}", e.what() );
			}
			catch ( std::exception& e )
			{
				spdlog::warn( "Failed to add preview from future: {}", e.what() );
			}
		}

		promise.addResult( record->m_game_id );
		promise.finish();
	}
	catch ( std::exception& e )
	{
		spdlog::error( "importGame: {}", e.what() );
		promise.setException( std::current_exception() );
	}
	catch ( ... )
	{
		spdlog::error( "importGame: {}", "Fuck" );
		promise.setException( std::current_exception() );
	}

} // namespace internal

QFuture< RecordID > importGame(
	std::filesystem::path root,
	std::filesystem::path relative_executable,
	QString title,
	QString creator,
	QString engine,
	QString version,
	std::array< QString, BannerType::SENTINEL > banners,
	std::vector< QString > previews,
	const std::uint64_t folder_size,
	const std::uint64_t file_count,
	const bool owning,
	const AtlasID atlas_id )
{
	ZoneScoped;
	return QtConcurrent::
		run( &globalPools().importers,
	         internal::importGame,
	         std::move( root ),
	         std::move( relative_executable ),
	         std::move( title ),
	         std::move( creator ),
	         std::move( engine ),
	         std::move( version ),
	         std::move( banners ),
	         std::move( previews ),
	         folder_size,
	         file_count,
	         owning,
	         atlas_id );
}

QFuture< RecordID > importGame( GameImportData data, const std::filesystem::path root, const bool owning )
{
	ZoneScoped;
	auto
		[ path,
	      title,
	      creator,
	      engine,
	      version,
	      size,
	      file_count,
	      executables,
	      executable,
	      banners,
	      previews,
	      atlas_id ] = std::move( data );

	return importGame(
		root / path,
		root / path / executable,
		std::move( title ),
		std::move( creator ),
		std::move( engine ),
		std::move( version ),
		std::move( banners ),
		std::move( previews ),
		size,
		file_count,
		owning,
		atlas_id );
}
