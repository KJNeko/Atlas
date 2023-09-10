//
// Created by kj16609 on 6/4/23.
//

#ifndef ATLASGAMEMANAGER_UTILS_HPP
#define ATLASGAMEMANAGER_UTILS_HPP

#include <QString>

#include <filesystem>
#include <vector>

namespace gl
{
	//! Searches the given directory for games with the GL_Infos.ini file
	[[nodiscard]] std::vector< std::filesystem::path > searchGames( const std::filesystem::path );

	struct GameListInfos
	{
		QString version {};
		// I can only assume that this ID is actually some internal thing used by GL. Sadly we can't really access it without tid's assistance.
		// So it's just a dummy for this time.
		std::uint64_t gl_id { 0 };
		QString name {};
		QString thread_url {};
		std::uint64_t f95_thread_id { INVALID_F95_ID };
	};

#define GL_INFO_FILENAME "GL_Infos.ini"

	[[nodiscard]] GameListInfos parse( const std::filesystem::path& path );

	[[nodiscard]] inline bool dirHasGLInfo( const std::filesystem::path& path )
	{
		return std::filesystem::exists( path / GL_INFO_FILENAME );
	}

} // namespace gl

#endif //ATLASGAMEMANAGER_UTILS_HPP
