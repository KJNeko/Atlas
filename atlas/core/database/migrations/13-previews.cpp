//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 13 >( Transaction& transaction )
	{
		transaction <<
			R"(
				CREATE TABLE IF NOT EXISTS previews
				(
					record_id REFERENCES games (record_id),
					path     TEXT UNIQUE,
					position INTEGER DEFAULT 256,
					UNIQUE (record_id, path)
				);

			)";
	}
} // namespace atlas::database::migrations
