//
// Created by kj16609 on 6/14/23.
//

#ifndef ATLASGAMEMANAGER_EXTRACT_HPP
#define ATLASGAMEMANAGER_EXTRACT_HPP

#include <filesystem>

namespace atlas
{
	std::vector< char > extract( const std::filesystem::path path );
}

#endif //ATLASGAMEMANAGER_EXTRACT_HPP
