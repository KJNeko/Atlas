//
// Created by kj16609 on 9/26/23.
//

#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	std::string_view migration< 17 >()
	{
		return
			R"(
				CREATE TABLE image_blurhash
				(
					image_sha256 BLOB UNIQUE PRIMARY KEY NOT NULL,
					blurhash TEXT NOT NULL,
					image_width INTEGER NOT NULL,
					image_height INTEGER NOT NULL
				);
			)";
	}
} // namespace atlas::database::migrations
