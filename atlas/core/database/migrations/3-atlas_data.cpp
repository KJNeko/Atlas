//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 3 >()
	{
		return
			R"(
			CREATE TABLE IF NOT EXISTS atlas_data
			(
				atlas_id       INTEGER PRIMARY KEY,
				id_name        STRING UNIQUE,
				short_name     STRING,
				title          STRING,
				original_name  STRING,
				category       STRING,
				engine         STRING,
				status         STRING,
				version        STRING,
				developer      STRING,
				creator        STRING,
				overview       STRING,
				censored       STRING,
				language       STRING,
				translations   STRING,
				genre          STRING,
				tags           STRING,
				voice          STRING,
				os             STRING,
				release_date   DATE,
				length         STRING,
				banner         STRING,
				banner_wide    STRING,
				cover          STRING,
				logo           STRING,
				wallpaper      STRING,
				previews       STRING,
				last_db_update STRING
			);
			)";
	}
} // namespace atlas::database::migrations
