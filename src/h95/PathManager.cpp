//
// Created by kj16609 on 1/31/23.
//


#include "PathManager.hpp"

PathManager::PathManager()
{
	key_replacer.registerKey(
		"{h95_games}",
		QString::fromStdString(
			std::filesystem::canonical( getSettings< QString >( "paths/games", "./data/games" ).toStdString() )
				.string() ) );

	key_replacer.registerKey(
		"{h95_data}",
		QString::fromStdString(
			std::filesystem::canonical( getSettings< QString >( "path/data", "./data" ).toStdString() ).string() ) );
}
