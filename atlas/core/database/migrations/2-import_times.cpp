//
// Created by kj16609 on 9/4/23.
//

#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 2 >()
	{
		return
			R"(
			CREATE VIEW IF NOT EXISTS last_import_times (record_id, last_import) AS
			SELECT DISTINCT record_id, versions.date_added
			FROM games
			NATURAL JOIN versions
			ORDER BY versions.date_added DESC;
		)";
	}
} // namespace atlas::database::migrations
