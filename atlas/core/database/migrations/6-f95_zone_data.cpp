//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 6 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_data
				(
					f95_id              INTEGER UNIQUE PRIMARY KEY,
					atlas_id            INTEGER REFERENCES atlas_data (atlas_id) UNIQUE,
					banner_url          STRING,
					site_url            STRING,
					last_thread_comment STRING,
					thread_publish_date STRING,
					last_record_update  STRING,
					views               STRING,
					likes               STRING,
					tags                STRING,
					rating              STRING,
					screens             STRING,
					replies             STRING
				);
			)";
	}
} // namespace atlas::database::migrations
