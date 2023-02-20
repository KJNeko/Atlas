//
// Created by kj16609 on 1/29/23.
//

#include <spdlog/spdlog.h>
#include <QCryptographicHash>
#include <QFile>
#include "Importer.hpp"
#include "h95/database/Record.hpp"
#include "config.hpp"


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
	spdlog::debug("Importer triggered to import game");

	if(m_root.empty()) spdlog::error("m_root was empty!");
	if(m_source.empty()) spdlog::error("m_source was empty!");

	bool copied_game {false};

	try
	{

		if ( m_root != m_source )
		{
			spdlog::debug( "Source and destination not the same. Switching config to source -> dest copy" );

			std::filesystem::create_directories( m_root );
			spdlog::debug( "Copying {} -> {}", m_source.string(), m_root.string() );
			std::filesystem::copy( m_source, m_root, std::filesystem::copy_options::recursive );
			copied_game = true;


			if ( !m_banner.empty() )
			{
				const auto banner_path_str { getSettings< QString >( "paths/h95_banners", "./data/banners" ) };
				const std::filesystem::path banner_path { banner_path_str.toStdString() };
				std::filesystem::create_directories( banner_path );

				spdlog::debug( "Banner not empty. Attempting copy to h95 banner path: {}", banner_path.string() );

				auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

				QFile file { m_banner };
				file.open( QFile::OpenModeFlag::ReadOnly );
				const auto file_data { file.readAll() };
				hasher.addData( file_data );

				const auto dest_path {
					banner_path / ( hasher.result().toHex( 0 ).toStdString() + m_banner.extension().string() ) };

				file.copy( dest_path );

				if ( delete_after )
				{
					std::filesystem::remove( m_banner );
					spdlog::debug( "Deleting banner from {} as requested after copy.", m_banner.string() );
				}

				//Delete banner from copied directory.
				if ( is_subpath( m_banner, m_root ) )
				{
					const auto banner_relative { std::filesystem::relative( m_banner, m_source ) };
					spdlog::debug(
						"Banner was detected inside source path. Removing from copy at {}",
						( m_root / banner_relative ).string() );
					std::filesystem::remove( m_root / banner_relative );
				}

				m_banner = dest_path;
				spdlog::debug( "Banner path set to {}", m_banner.string() );
			}

			const auto preview_path_str { getSettings< QString >( "paths/h95_previews", "./data/previews" ) };
			const std::filesystem::path preview_path { preview_path_str.toStdString() };
			std::filesystem::create_directories( preview_path );

			//Copy previews
			for ( auto& preview : m_previews )
			{
				spdlog::debug( "Handling preview {}", preview.string() );
				auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

				QFile file { preview };
				file.open( QFile::OpenModeFlag::ReadOnly );
				const auto file_data { file.readAll() };
				hasher.addData( file_data );

				const auto dest_path {
					preview_path / ( hasher.result().toHex( 0 ).toStdString() + preview.extension().string() ) };

				spdlog::debug( "Copying preview from {} -> {}", preview.string(), dest_path.string() );

				file.copy( dest_path );

				if ( delete_after ) std::filesystem::remove( preview );

				//Delete preview from copied directory.
				if ( is_subpath( preview, m_root ) )
				{
					const auto preview_relative { std::filesystem::relative( preview, m_source ) };
					std::filesystem::remove( m_root / preview_relative );
				}

				preview = dest_path;
			}
		}

		m_executable = std::filesystem::relative( m_executable, m_source );
		spdlog::debug( "Setting executable to relative path: {}", m_executable.string() );

		//Import
		Record::create( title, creator, engine, { version, m_root, m_executable }, m_banner, m_previews );
	}
	catch(std::exception& e)
	{
		spdlog::error("Import failed! Reason: {}", e.what());
		if(copied_game)
		{
			spdlog::info("Deleting game from copied directory after failure");
			std::filesystem::remove_all(m_root);
		}
	}
}