//
// Created by kj16609 on 9/4/23.
//

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 8 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS updates
				(
					update_time    INTEGER PRIMARY KEY,
					processed_time INTEGER,
					md5            BLOB
				);
			)";
	}
} // namespace atlas::database::migrations
