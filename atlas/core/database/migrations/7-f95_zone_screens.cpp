//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 7 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_screens
				(
					f95_id     INTEGER REFERENCES f95_zone_data (f95_id),
					screen_url TEXT NOT NULL
				);
			)";
	}
} // namespace atlas::database::migrations