//
// Created by kj16609 on 9/13/23.
//

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 16 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_mappings
				(
					game_id INTEGER REFERENCES games(game_id),
					f95_id INTEGER REFERENCES f95_zone_data(f95_id)
				);
			)";
	}
} // namespace atlas::database::migrations
