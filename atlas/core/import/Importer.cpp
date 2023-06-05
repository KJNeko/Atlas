//
// Created by kj16609 on 6/2/23.
//

#include "Importer.hpp"

#include <QFuture>
#include <QtConcurrent>

#include "GameImportData.hpp"
#include "core/database/Database.hpp"
#include "core/database/record/Record.hpp"
#include "core/database/record/RecordBanner.hpp"
#include "core/database/record/RecordPreviews.hpp"
#include "core/foldersize.hpp"
#include "core/imageManager.hpp"
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
		auto signaler { createNotification< ProgressMessage >( QString( "Importing game %1" ).arg( title ), true ) };

		promise.start();

		//Verify that everything is valid
		if ( !std::filesystem::exists( root ) )
			throw std::runtime_error( fmt::format( "Root path {:ce} does not exist", root ) );
		if ( !std::filesystem::exists( root / relative_executable ) )
			throw std::runtime_error( fmt::format( "Executable {:ce} does not exist", root / relative_executable ) );
		if ( title.isEmpty() ) throw std::runtime_error( "Title is empty" );
		if ( creator.isEmpty() ) throw std::runtime_error( "Creator is empty" );
		if ( version.isEmpty() ) throw std::runtime_error( "Version is empty" );

		Transaction t { NoAutocommit };

		signaler->setProgress( Progress::ImportRecordData );
		signaler->setMessage( "Importing record data" );
		auto record { importRecord( title, creator, engine, t ) };

		signaler->setProgress( Progress::CollectingFileInformation );
		signaler->setMessage( "Collecting file information" );
		std::vector< std::filesystem::path > relative_paths;
		for ( const auto& entry : std::filesystem::recursive_directory_iterator( root ) )
		{
			if ( entry.is_regular_file() )
			{
				relative_paths.emplace_back( std::filesystem::relative( entry.path(), root ) );
				signaler->setProgress( Progress::CollectingFileInformation );
				signaler->setMessage( QString( "Processed %1 files" ).arg( relative_paths.size() ) );
			}
		}

		std::size_t folder_size { 0 };

		if ( owning )
		{
			signaler->setMax( static_cast< int >( relative_paths.size() ) );

			for ( std::size_t i = 0; i < relative_paths.size(); i++ )
			{
				const QString path_name { QString::fromStdString( relative_paths[ i ].string() ) };
				signaler->setProgress( static_cast< int >( i ) );
				signaler->setMessage( QString( "Copying file %1 %2/%3" )
				                          .arg( path_name )
				                          .arg( i )
				                          .arg( relative_paths.size() ) );

				const auto source { root / relative_paths[ i ] };
				const std::filesystem::path dest_root { config::paths::games::getPath() / creator.toStdString()
					                                    / title.toStdString() / version.toStdString() };
				const auto dest { dest_root / relative_paths[ i ] };
				std::filesystem::create_directories( dest.parent_path() );

				if ( !std::filesystem::copy_file( source, dest, std::filesystem::copy_options::overwrite_existing ) )
				{
					spdlog::error( "importGame: Failed to copy file {} to {}", source.string(), dest.string() );
					throw std::runtime_error( "Failed to copy file" );
				}

				folder_size += std::filesystem::file_size( dest );
			}
		}
		else
		{
			for ( const auto& path : relative_paths ) folder_size += std::filesystem::file_size( root / path );
		}

		signaler->setProgress( Progress::VersionData );
		signaler->setMessage( "Importing version data" );
		signaler->setMax( Progress::Complete );

		record->addVersion( version, root, relative_executable, folder_size, owning, t );

		signaler->setProgress( Progress::Banners );
		signaler->setMessage( "Importing banners" );
		for ( int i = 0; i < BannerType::SENTINEL; i++ )
		{
			const auto path { banners[ static_cast< std::size_t >( i ) ] };
			if ( !path.isEmpty() )
			{
				const auto banner_path { imageManager::importImage( path.toStdString() ) };
				record->banners().setBanner( banner_path, static_cast< BannerType >( i ), t );
			}
		}

		signaler->setMessage( "Importing previews" );
		signaler->setProgress( Progress::Previews );
		for ( const auto& path : previews )
		{
			const auto preview_path { imageManager::importImage( path.toStdString() ) };
			signaler->setMessage( QString( "Importing preview %1" ).arg( preview_path.string().c_str() ) );
			record->previews().addPreview( preview_path, t );
		}

		signaler->setProgress( Progress::Complete );
		signaler->setMessage( "Complete" );

		t.commit();
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
	bool owning )
{
	return QtConcurrent::
		run( QThreadPool::globalInstance(),
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
		owning );
}
