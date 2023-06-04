//
// Created by kj16609 on 6/4/23.
//

#ifndef ATLASGAMEMANAGER_UTILS_HPP
#define ATLASGAMEMANAGER_UTILS_HPP

#include <filesystem>
#include <vector>

#include <QString>

namespace gl
{
	//! Searches the given directory for games with the GL_Infos.ini file
	std::vector< std::filesystem::path > searchGames( const std::filesystem::path );

	struct GameListInfos
	{
		QString version {};
		std::uint64_t f95_id { 0 };
		QString name {};
		QString thread_url {};
	};

	GameListInfos parse( const std::filesystem::path& path );

} // namespace gl

#endif //ATLASGAMEMANAGER_UTILS_HPP
