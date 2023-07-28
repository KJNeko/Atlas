//
// Created by kj16609 on 6/2/23.
//

#include "Importer.hpp"

#include <QFuture>
#include <QtConcurrent>

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include "GameImportData.hpp"
#include "core/database/Database.hpp"
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"
#include "core/foldersize.hpp"
#include "core/imageManager.hpp"
#include "core/notifications.hpp"
#include "core/utils/FileScanner.hpp"

namespace internal
{
	enum Progress
	{
		ImportRecordData,
		CollectingFileInformation,
		VersionData,
		Banners,
		Previews,
		Complete,
	};

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
		const bool owning,
		const bool scan_filesize )
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
		FileScanner scanner { root };

		//Get the size of the folder
		signaler.setProgress( Progress::CollectingFileInformation );
		signaler.setMessage( "Calculating folder size: 0B" );
		std::size_t folder_size { 0 };
		std::size_t file_count { 0 };
		QLocale locale;
		if ( scan_filesize )
		{
			for ( const auto& file : scanner )
			{
				++file_count;
				folder_size += file.size;

				if ( folder_size % 1024 == 0 )
				{
					signaler.setMessage( QString( "Calculating folder size: %1" )
					                         .arg( locale.formattedDataSize( static_cast< qint64 >( folder_size ) ) ) );
				}
			}
		}
		TracyCZoneEnd( tracy_FileScanner );

		signaler.setProgress( Progress::ImportRecordData );
		signaler.setMessage( "Importing record data" );
		auto record { importRecord( title, creator, engine ) };

		signaler.setProgress( Progress::VersionData );
		signaler.setMessage( "Importing version data" );

		// Used for when we move files to a directory we 'own'
		const std::filesystem::path dest_root { config::paths::games::getPath() / creator.toStdString()
			                                    / title.toStdString() / version.toStdString() };

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
				signaler.setMessage( QString( "Copying file %1 %2/%3" ).arg( r_path_name ).arg( i ).arg( file_count ) );

				const auto source { root / file.relative };
				const auto dest { dest_root / file.relative };
				std::filesystem::create_directories( dest.parent_path() );

				if ( !std::filesystem::copy_file( source, dest, std::filesystem::copy_options::overwrite_existing ) )
				{
					spdlog::error( "importGame: Failed to copy file {} to {}", source.string(), dest.string() );
					throw std::runtime_error( "Failed to copy file" );
				}
			}

			record.addVersion( version, dest_root, relative_executable );
		}
		else
			record.addVersion( version, root, relative_executable );

		signaler.setMax( Progress::Complete );
		signaler.setProgress( Progress::Banners );
		signaler.setMessage( "Importing banners" );

		std::array< std::optional< QFuture< std::filesystem::path > >, BannerType::SENTINEL > banner_futures {};

		for ( std::size_t i = 0; i < BannerType::SENTINEL; i++ )
		{
			const auto path { banners[ i ] };
			if ( !path.isEmpty() )
			{
				banner_futures[ i ] = imageManager::importImage( { path.toStdString() }, record->m_game_id );
			}
			else
				banner_futures[ i ] = { std::nullopt };

			//If the game is going into our directory then we should clean up the banners
			if ( owning )
			{
				// Remove the image file from the moved files.
				std::filesystem::remove(
					dest_root
					/ std::filesystem::relative( { banners[ static_cast< std::size_t >( i ) ].toStdString() }, root ) );
			}
		}

		signaler.setMessage( "Importing previews" );
		signaler.setProgress( Progress::Previews );

		std::vector< QFuture< std::filesystem::path > > preview_futures;

		for ( const auto& path : previews )
		{
			signaler.setMessage( QString( "Importing preview %1" ).arg( path ) );
			preview_futures.emplace_back( imageManager::importImage( { path.toStdString() }, record->m_game_id ) );
			//record.addPreview( { path.toStdString() } );

			if ( owning ) //If we own it then we should delete the path from our directory
			{
				// Remove the image file from the moved files
				std::filesystem::remove( dest_root / std::filesystem::relative( { path.toStdString() }, root ) );
			}
		}

		signaler.setMessage( "Waiting on image futures" );

		for ( int i = 0; i < BannerType::SENTINEL; i++ )
		{
			auto opt { banner_futures[ static_cast< std::size_t >( i ) ] };

			if ( opt.has_value() )
			{
				try
				{
					const std::filesystem::path path { opt.value().result() };
					record.setBanner( path, static_cast< BannerType >( i ) );
				}
				catch ( std::exception& e )
				{
					spdlog::warn( "Failed to add banner to record {}: {}", record->m_game_id, e.what() );
				}
			}
		}

		for ( auto& future : preview_futures )
		{
			try
			{
				future.waitForFinished();
				record.addPreview( future.result() );
			}
			catch ( std::exception& e )
			{
				spdlog::warn( "Failed to add preview from future: {}", e.what() );
			}
		}

		signaler.setProgress( Progress::Complete );
		signaler.setMessage( "Complete" );

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
	bool owning,
	bool scan_filesize,
	QThreadPool& pool )
{
	ZoneScoped;
	return QtConcurrent::
		run( &pool,
	         internal::importGame,
	         std::move( root ),
	         std::move( relative_executable ),
	         std::move( title ),
	         std::move( creator ),
	         std::move( engine ),
	         std::move( version ),
	         std::move( banners ),
	         std::move( previews ),
	         owning,
	         scan_filesize );
}

QFuture< RecordID >
	importGame( GameImportData data, const std::filesystem::path root, const bool owning, const bool scan_filesize )
{
	ZoneScoped;
	auto [ path, title, creator, engine, version, size, executables, executable, banners, previews ] =
		std::move( data );

	return importGame(
		root / path,
		root / path / executable,
		std::move( title ),
		std::move( creator ),
		"",
		std::move( version ),
		std::move( banners ),
		std::move( previews ),
		owning,
		scan_filesize,
		*QThreadPool::globalInstance() );
}

QFuture< RecordID > importGame(
	GameImportData data,
	const std::filesystem::path root,
	const bool owning,
	const bool scan_filesize,
	QThreadPool& pool )
{
	ZoneScoped;
	auto [ path, title, creator, engine, version, size, executables, executable, banners, previews ] =
		std::move( data );

	return importGame(
		root / path,
		root / path / executable,
		std::move( title ),
		std::move( creator ),
		"",
		std::move( version ),
		std::move( banners ),
		std::move( previews ),
		owning,
		scan_filesize,
		pool );
}
