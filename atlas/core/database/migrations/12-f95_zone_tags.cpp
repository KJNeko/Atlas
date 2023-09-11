//
// Created by kj16609 on 9/4/23.
//
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 12 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_tags
				(
					f95_id INTEGER REFERENCES f95_zone_data (f95_id),
					tag_id INTEGER REFERENCES tags (tag_id)
				);
			)";
	}
} // namespace atlas::database::migrations
