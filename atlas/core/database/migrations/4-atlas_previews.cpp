//
// Created by kj16609 on 9/4/23.
//

#include <string_view>

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 4 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS atlas_previews
				(
					atlas_id    INTEGER REFERENCES atlas_data (atlas_id),
					preview_url STRING NOT NULL,
					UNIQUE (atlas_id, preview_url)
				);
			)";
	}
} // namespace atlas::database::migrations
