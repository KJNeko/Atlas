//
// Created by kj16609 on 1/29/23.
//

#include <QCryptographicHash>
#include <QFile>

#include <h95/Importer.hpp>

#include <h95/logging.hpp>
#include <h95/database/Record.hpp>
#include <h95/config.hpp>


bool is_subpath( const std::filesystem::path& path, const std::filesystem::path& base )
{
	auto rel { std::filesystem::relative( path, base ) };
	return !rel.empty() && rel.native()[0] != '.';
}

void Importer::import_game(
	const QString& title,
	const QString& creator,
	const QString& version,
	const QString& engine,
	const bool delete_after )
{
	spdlog::debug( "Importer triggered to import game" );

	if ( m_root.empty() )
	{
		spdlog::error( "m_root was empty!" );
		return;
	}
	if ( m_source.empty() )
	{
		spdlog::error( "m_source was empty!" );
		return;
	}

	bool copied_game { false };

	try
	{
		if ( m_root != m_source )
		{
			spdlog::debug( "Source and destination not the same. Switching config to source -> dest copy" );

			std::filesystem::create_directories( m_root.parent_path() );
			spdlog::debug( "Copying {} -> {}", m_source.string(), m_root.string() );
			std::filesystem::copy( m_source, m_root, std::filesystem::copy_options::recursive );
			copied_game = true;


			if ( !m_banner.empty() )
			{
				//Delete banner from copied directory.
				if ( is_subpath( m_banner, m_root ) )
				{
					const auto banner_relative { std::filesystem::relative( m_banner, m_source ) };
					spdlog::debug(
						"Banner was detected inside source path. Removing from copy at {}",
						( m_root / banner_relative ).string() );
					std::filesystem::remove( m_root / banner_relative );
				}

				m_banner = importBanner( m_banner );
				spdlog::debug( "Banner path set to {}", m_banner.string() );
			}

			//Copy previews
			for ( auto& preview : m_previews )
			{
				//Delete preview from copied directory.
				if ( is_subpath( preview, m_root ) )
				{
					const auto preview_relative { std::filesystem::relative( preview, m_source ) };
					std::filesystem::remove( m_root / preview_relative );
				}

				preview = importPreview( preview, delete_after );
			}
		}

		m_executable = std::filesystem::relative( m_executable, m_source );
		spdlog::debug( "Setting executable to relative path: {}", m_executable.string() );

		//Import
		Record::create( title, creator, engine, { version, m_root, m_executable }, m_banner, m_previews );
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Import failed! Reason: {}", e.what() );
		if ( copied_game )
		{
			spdlog::info( "Deleting game from copied directory after failure" );
			std::filesystem::remove_all( m_root );
		}
	}
}

std::filesystem::path importBanner( const std::filesystem::path& path, const bool delete_after )
{
	const auto banner_path_str { getSettings< QString >( "paths/h95_banners", "./data/banners" ) };
	const std::filesystem::path banner_path { banner_path_str.toStdString() };
	std::filesystem::create_directories( banner_path );

	spdlog::debug( "Banner not empty. Attempting copy to h95 banner path: {}", banner_path.string() );

	auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

	QFile file { path };
	file.open( QFile::OpenModeFlag::ReadOnly );
	const auto file_data { file.readAll() };
	hasher.addData( file_data );

	const auto dest_path { banner_path / ( hasher.result().toHex( 0 ).toStdString() + path.extension().string() ) };

	file.copy( dest_path );

	if ( delete_after )
	{
		std::filesystem::remove( path );
		spdlog::debug( "Deleting banner from {} as requested after copy.", path.string() );
	}

	return dest_path;
}

std::filesystem::path importPreview( const std::filesystem::path& path, const bool delete_after )
{
	const auto preview_path_str { getSettings< QString >( "paths/h95_previews", "./data/previews" ) };
	const std::filesystem::path preview_path { preview_path_str.toStdString() };
	std::filesystem::create_directories( preview_path );

	spdlog::debug( "Handling preview {}", path.string() );
	auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

	QFile file { path };
	file.open( QFile::OpenModeFlag::ReadOnly );
	const auto file_data { file.readAll() };
	hasher.addData( file_data );

	const auto dest_path { preview_path / ( hasher.result().toHex( 0 ).toStdString() + path.extension().string() ) };

	spdlog::debug( "Copying preview from {} -> {}", path.string(), dest_path.string() );

	file.copy( dest_path );

	if ( delete_after ) std::filesystem::remove( path );

	return dest_path;
}
