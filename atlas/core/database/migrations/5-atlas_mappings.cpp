//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 5 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS atlas_mappings
				(
					record_id INTEGER REFERENCES games (record_id),
					atlas_id  INTEGER REFERENCES atlas_data (atlas_id),
					UNIQUE (record_id, atlas_id)
				);
			)";
	}
} // namespace atlas::database::migrations
