//
// Created by kj16609 on 9/4/23.
//
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 11 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS atlas_tags
				(
					tag_id   INTEGER REFERENCES tags (tag_id),
					atlas_id INTEGER REFERENCES atlas_data (atlas_id),
					UNIQUE (atlas_id, tag_id)
				);
			)";
	}
} // namespace atlas::database::migrations
