//
// Created by kj16609 on 1/31/23.
//


#include "PathManager.hpp"

PathManager::PathManager()
{
	key_replacer.registerKey(
		"{h95_data}",
		QString::fromStdString(
			std::filesystem::canonical( getSettings< QString >( "paths/data", "./data/games" ).toStdString() )
				.string() )
	);




}
