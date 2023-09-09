//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 14 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS banners
				(
					record_id REFERENCES games (record_id),
					path TEXT UNIQUE,
					type INTEGER,
					UNIQUE (record_id, path, type)
				);
			)";
	}
} // namespace atlas::database::migrations