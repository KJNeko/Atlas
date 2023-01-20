//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DATABASE_HPP
#define HYDRUS95_DATABASE_HPP

#include <sqlite_modern_cpp.h>
namespace database
{
	sqlite::database& db_ref();
	void initalize();
}  // namespace database

#endif	//HYDRUS95_DATABASE_HPP
