//
// Created by kj16609 on 9/4/23.
//

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 9 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS tags
				(
					tag_id INTEGER PRIMARY KEY,
					tag    TEXT UNIQUE
				);
			)";
	}
} // namespace atlas::database::migrations
