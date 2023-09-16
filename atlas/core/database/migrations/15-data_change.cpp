//
// Created by kj16609 on 9/4/23.
//
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 15 >()
	{
		return
			R"(
				CREATE TABLE IF NOT EXISTS data_change
				(
					timestamp INTEGER,
					delta     INTEGER
				)
			)";
	}
} // namespace atlas::database::migrations
