//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DATABASE_HPP
#define HYDRUS95_DATABASE_HPP

#include <QSqlDatabase>
#include "backend/records/Record.hpp"

namespace database
{
	void initalize();

	/**
	 * @param title
	 * @param creator
	 * @param version
	 * @throws std::runtime_error when parameters are invalid
	 * @throws DuplicateNameRecord when the `title` parameter already exists
	 * @return
	 */
	Record createEmptyRecord();


	GameRecord* transformToGameRecord(Record* ptr);
}


#endif	//HYDRUS95_DATABASE_HPP
