//
// Created by kj16609 on 1/20/23.
//

#ifndef HYDRUS95_PATHMANAGER_HPP
#define HYDRUS95_PATHMANAGER_HPP


/**

Valid placeholders:

{h95_data}
{h95_games}
{title}
{creator}
{engine}
{version}
*/

#include "h95/config.hpp"
#include "KeyReplacer.hpp"
#include <string>
#include <filesystem>
#include <iostream>
#include <tracy/Tracy.hpp>

struct PathManager
{
	KeyReplacer key_replacer {};

	PathManager();

	/**
	 * @example path = {h95_data}/{engine}/{version}
	 * @param path
	 * @param relativeFromRoot
	 * @return
	 */
	//! Uses key_replacer to populate `path` with keys.
	std::filesystem::path fillPath( const std::filesystem::path& path,[[maybe_unused]] bool relativeFromRoot = false ) const;

	//! Uses `key_path` to rip values out of `path` to populate `key_replacer`
	void populateValues( const std::filesystem::path& path, const std::filesystem::path key_path );
};

#endif	//HYDRUS95_PATHMANAGER_HPP
