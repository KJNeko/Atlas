//
// Created by kj16609 on 9/4/23.
//
#include <string_view>

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 7 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_screens
				(
					f95_id     INTEGER REFERENCES f95_zone_data (f95_id),
					screen_url TEXT NOT NULL
				);
			)";
	}
} // namespace atlas::database::migrations
