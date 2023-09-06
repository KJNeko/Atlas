//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 10 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS tag_mappings
				(
					record_id INTEGER REFERENCES games (record_id),
					tag_id REFERENCES tags (tag_id),
					UNIQUE (record_id, tag_id)
				);
			)";
	}
} // namespace atlas::database::migrations
