//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 0 >()
	{
		return
			R"(
			CREATE TABLE IF NOT EXISTS games
		   	(
				record_id      INTEGER PRIMARY KEY,
				title          TEXT NOT NULL,
				creator        TEXT NOT NULL,
				engine         TEXT,
				last_played_r  DATE    DEFAULT 0,
				total_playtime INTEGER DEFAULT 0,
				description    TEXT,
				UNIQUE (title, creator, engine)
		    );
		)";
	}

} // namespace atlas::database::migrations