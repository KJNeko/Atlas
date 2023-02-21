//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DATABASE_HPP
#define HYDRUS95_DATABASE_HPP

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wconversion"

#include <sqlite_modern_cpp.h>

#pragma GCC diagnostic pop

namespace sqlite
{
	class database;
}

namespace database
{

	//! Returns the global ref to the database.
	sqlite::database& db_ref();

	//! Initalizes the database. Creating all tables and other stuff.
	void initalize();
}  // namespace database

#endif	//HYDRUS95_DATABASE_HPP
