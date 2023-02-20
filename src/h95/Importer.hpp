//
// Created by kj16609 on 1/29/23.
//

#ifndef HYDRUS95_IMPORTER_HPP
#define HYDRUS95_IMPORTER_HPP

#include <filesystem>
#include <vector>
#include <QString>

class Importer
{
	std::filesystem::path m_root {};
	std::filesystem::path m_source {};
	std::filesystem::path m_executable {};
	std::filesystem::path m_banner {};
	std::vector< std::filesystem::path > m_previews {};

	public:
	Importer() = default;

	void setDestination( const std::filesystem::path& path )
	{
		m_root = path;
		spdlog::debug( "Destination set to {}", path.string() );
	}

	void setBanner( const std::filesystem::path& path )
	{
		m_banner = path;
		spdlog::debug( "Banner path set to {}", path.string() );
	}

	void setPreviews( const std::vector< std::filesystem::path >& previews )
	{
		m_previews = previews;
		spdlog::debug( "Set {} previews", previews.size() );
	}

	void setExecutable( const std::filesystem::path& path )
	{
		m_executable = path;
		spdlog::debug( "Executable path set to {}", path.string() );
	}

	void setSource( const std::filesystem::path& path )
	{
		m_source = path;
		spdlog::debug( "Source set to {}", path.string() );
	}

	//! Imports the game to the database and moves it to it's destination if m_root != m_source
	void import_game(
		const QString& title,
		const QString& creator,
		const QString& version,
		const QString& engine,
		bool delete_after );
};

#endif	//HYDRUS95_IMPORTER_HPP
