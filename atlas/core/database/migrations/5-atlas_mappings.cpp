//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 5 >( Transaction& transaction )
	{
		transaction <<
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
