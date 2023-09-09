//
// Created by kj16609 on 9/4/23.
//
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 10 >()
	{
		return
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
