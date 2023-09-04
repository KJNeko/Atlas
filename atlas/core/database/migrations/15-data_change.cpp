//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 15 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS data_change
				(
					timestamp INTEGER,
					delta     INTEGER
				)
			)";
	}
} // namespace atlas::database::migrations
