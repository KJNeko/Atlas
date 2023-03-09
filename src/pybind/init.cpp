//
// Created by kj16609 on 3/9/23.
//

#include <h95/database/Database.hpp>
#include <h95/logging.hpp>

extern "C"
{
	void init()
	{
		spdlog::info( "Doing init!" );

		initLogging();

		Database::initalize("./data/hydrus95.db");
	}
}