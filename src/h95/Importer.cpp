//
// Created by kj16609 on 1/29/23.
//

#include <spdlog/spdlog.h>
#include <QCryptographicHash>
#include <QFile>
#include "Importer.hpp"
#include "h95/database/Record.hpp"
#include "config.hpp"


void Importer::import_game(
	const QString& title,
	const QString& creator,
	const QString& version,
	const QString& engine,
	const bool delete_after )
{
	if ( m_root != m_source )
	{
		std::filesystem::create_directories( m_root );
		spdlog::debug("Copying {} -> {}", m_source.string(), m_root.string());
		std::filesystem::copy( m_source, m_root );

		if ( !m_banner.empty() )
		{
			const auto banner_path_str {getSettings<QString>("paths/h95_banners", "./data/banners")};
			const std::filesystem::path banner_path {banner_path_str.toStdString()};
			std::filesystem::create_directories(banner_path);

			auto hasher {QCryptographicHash(QCryptographicHash::Algorithm::Sha256)};

			QFile file {m_banner};
			file.open(QFile::OpenModeFlag::ReadOnly);
			const auto file_data {file.readAll()};
			hasher.addData(file_data);

			const auto dest_path {banner_path / (hasher.result().toHex(0).toStdString() + m_banner.extension().string())};

			file.copy(dest_path);

			if ( delete_after ) std::filesystem::remove( m_banner );

			m_banner = dest_path;
		}

		const auto preview_path_str {getSettings<QString>("paths/h95_previews", "./data/previews")};
		const std::filesystem::path preview_path {preview_path_str.toStdString()};
		std::filesystem::create_directories(preview_path);

		//Copy previews
		for ( auto& preview : m_previews )
		{
			auto hasher { QCryptographicHash( QCryptographicHash::Algorithm::Sha256 ) };

			QFile file { preview };
			file.open(QFile::OpenModeFlag::ReadOnly);
			const auto file_data { file.readAll() };
			hasher.addData( file_data );

			const auto dest_path {
				preview_path / ( hasher.result().toHex( 0 ).toStdString() + preview.extension().string() ) };

			file.copy( dest_path );

			if ( delete_after ) std::filesystem::remove( preview );

			preview = dest_path;
		}
	}

	m_executable = std::filesystem::relative( m_executable, m_source );

	//Import
	Record::create( title, creator, engine, { version, m_root, m_executable }, m_banner, m_previews );
}