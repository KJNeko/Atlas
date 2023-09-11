//
// Created by kj16609 on 9/4/23.
//

#include <string_view>

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 1 >()
	{
		return
			R"(
			CREATE TABLE IF NOT EXISTS versions
			   (
				record_id        INTEGER REFERENCES games (record_id),
				version          TEXT,
				game_path        TEXT,
				exec_path        TEXT,
				in_place		 BOOLEAN,
				last_played      DATE,
				version_playtime INTEGER,
				folder_size      INTEGER,
				date_added       INTEGER,
				UNIQUE (record_id, version)
			   );
		)";
	}
} // namespace atlas::database::migrations
