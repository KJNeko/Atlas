//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 12 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS f95_zone_tags
				(
					f95_id INTEGER REFERENCES f95_zone_data (f95_id),
					tag_id INTEGER REFERENCES tags (tag_id)
				);
			)";
	}
} // namespace atlas::database::migrations
