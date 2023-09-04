//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 9 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS tags
				(
					tag_id INTEGER PRIMARY KEY,
					tag    TEXT UNIQUE
				);
			)";
	}
} // namespace atlas::database::migrations
