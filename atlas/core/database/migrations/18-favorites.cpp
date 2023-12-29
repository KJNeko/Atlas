//
// Created by kj16609 on 12/28/23.
//

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 18 >()
	{
		return R"(CREATE TABLE favorites (record_id INTEGER REFERENCES games(record_id)))";
	}

} // namespace atlas::database::migrations
