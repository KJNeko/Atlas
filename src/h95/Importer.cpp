//
// Created by kj16609 on 1/29/23.
//

#include <spdlog/spdlog.h>
#include "Importer.hpp"
#include "h95/database/Record.hpp"

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
	const bool delete_after)
{
	if ( m_root != m_source )
	{
		std::filesystem::create_directories( m_root );
		std::filesystem::copy( m_source, m_root );

		std::filesystem::create_directories( m_root / "h95" );

		if(!m_banner.empty())
		{
			const auto rel1 { std::filesystem::relative( m_banner, m_source ) };

			spdlog::debug( "{} vs {} -> {}", m_banner.string(), m_source.string(), rel1.string() );

			if ( is_subpath(
					 m_banner,
					 m_source ) )  //Banner is inside of root. Copy it from where it is to the new destination
				std::filesystem::rename( m_root / rel1, m_root / "h95" / m_banner.filename() );
			else  //Banner is not inside of root
				std::filesystem::copy( m_banner, m_root / "h95" / m_banner.filename() );

			if ( delete_after ) std::filesystem::remove( m_banner );

			m_banner = std::filesystem::relative( m_root / "h95" / m_banner.filename(), m_root );
		}

		//Copy previews
		for ( auto& preview : m_previews )
		{
			const auto rel_preview { std::filesystem::relative( preview, m_root ) };

			std::filesystem::create_directories(m_root / "h95" / "previews");

			if ( is_subpath( preview, m_root ) )  //Preview is inside root
				std::filesystem::rename( preview / rel_preview, m_root / "h95" / "previews" / preview.filename() );
			else
				std::filesystem::copy( preview, m_root / "h95" / "previews" / preview.filename() );

			if(delete_after)
				std::filesystem::remove(preview);

			preview = std::filesystem::relative( m_root / "h95" / "previews" / preview.filename(), m_root );
		}

		if(delete_after)
			std::filesystem::remove(m_source);
	}

	m_executable = std::filesystem::relative(m_executable, m_source);

	//Import
	Record::create( title, creator, version, engine, { m_root, m_executable }, m_banner, m_previews );

	return;
}