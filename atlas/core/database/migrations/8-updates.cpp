//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 8 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS updates
				(
					update_time    INTEGER PRIMARY KEY,
					processed_time INTEGER,
					md5            BLOB
				);
			)";
	}
} // namespace atlas::database::migrations
