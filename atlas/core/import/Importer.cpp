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
#include "core/database/record/Record.hpp"
#include "core/database/record/RecordBanner.hpp"
#include "core/database/record/RecordPreviews.hpp"
#include "core/foldersize.hpp"
#include "core/imageManager.hpp"
#include "core/utils/FileScanner.hpp"
#include "ui/notifications/NotificationPopup.hpp"
#include "ui/notifications/ProgressMessage.hpp"

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
		const bool owning )
	try
	{
		ZoneScoped;
		auto signaler { createNotification< ProgressMessage >( QString( "Importing game %1" ).arg( title ), true ) };

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
		signaler->setProgress( Progress::CollectingFileInformation );
		signaler->setMessage( "Calculating folder size: 0B" );
		std::size_t folder_size { 0 };
		std::size_t file_count { 0 };
		QLocale locale;
		for ( const auto& file : scanner )
		{
			++file_count;
			folder_size += file.size;

			if ( folder_size % 1024 == 0 )
			{
				signaler->setMessage( QString( "Calculating folder size: %1" )
				                          .arg( locale.formattedDataSize( static_cast< qint64 >( folder_size ) ) ) );
			}
		}
		TracyCZoneEnd( tracy_FileScanner );

		signaler->setProgress( Progress::ImportRecordData );
		signaler->setMessage( "Importing record data" );
		auto record { importRecord( title, creator, engine ) };

		signaler->setProgress( Progress::VersionData );
		signaler->setMessage( "Importing version data" );

		record->addVersion( version, root, relative_executable, folder_size, owning );

		if ( owning )
		{
			ZoneScopedN( "Copying files" );
			signaler->setMax( static_cast< int >( file_count ) );

			std::size_t i { 0 };
			for ( const auto& file : scanner )
			{
				++i;
				const QString r_path_name { QString::fromStdString( file.relative.string() ) };
				signaler->setProgress( static_cast< int >( i ) );
				signaler
					->setMessage( QString( "Copying file %1 %2/%3" ).arg( r_path_name ).arg( i ).arg( file_count ) );

				const auto source { root / file.relative };
				const std::filesystem::path dest_root { config::paths::games::getPath() / creator.toStdString()
					                                    / title.toStdString() / version.toStdString() };
				const auto dest { dest_root / file.relative };
				std::filesystem::create_directories( dest.parent_path() );

				if ( !std::filesystem::copy_file( source, dest, std::filesystem::copy_options::overwrite_existing ) )
				{
					spdlog::error( "importGame: Failed to copy file {} to {}", source.string(), dest.string() );
					throw std::runtime_error( "Failed to copy file" );
				}
			}
		}

		signaler->setMax( Progress::Complete );
		signaler->setProgress( Progress::Banners );
		signaler->setMessage( "Importing banners" );
		for ( int i = 0; i < BannerType::SENTINEL; i++ )
		{
			const auto path { banners[ static_cast< std::size_t >( i ) ] };
			if ( !path.isEmpty() )
			{
				record->banners().setBanner( path.toStdString(), static_cast< BannerType >( i ) );
			}
		}

		signaler->setMessage( "Importing previews" );
		signaler->setProgress( Progress::Previews );
		for ( const auto& path : previews )
		{
			signaler->setMessage( QString( "Importing preview %1" ).arg( path ) );
			record->previews().addPreview( path.toStdString() );
		}

		signaler->setProgress( Progress::Complete );
		signaler->setMessage( "Complete" );

		promise.addResult( record->getID() );
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
	         owning );
}

QFuture< RecordID > importGame( GameImportData data, const std::filesystem::path root, const bool owning )
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
		*QThreadPool::globalInstance() );
}

QFuture< RecordID >
	importGame( GameImportData data, const std::filesystem::path root, const bool owning, QThreadPool& pool )
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
		pool );
}
