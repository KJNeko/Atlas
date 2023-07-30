//
// Created by kj16609 on 7/29/23.
//

#include "tags.hpp"

#include "core/database/Transaction.hpp"

namespace atlas::tags
{
	TagID createTag( QString str )
	{
		str = str.trimmed();
		TagID tag_id { INVALID_TAG_ID };
		RapidTransaction() << "SELECT tag_id FROM tags WHERE tag = ?" << str >> tag_id;
		if ( tag_id != INVALID_TAG_ID )
			return tag_id;
		else
		{
			RapidTransaction() << "INSERT INTO tags (tag) VALUES (?) RETURNING tag_id" << str >> tag_id;
			if ( tag_id == INVALID_TAG_ID )
				throw std::runtime_error( "Something went wrong while trying to add tag to DB" );
			return tag_id;
		}
	}

} // namespace atlas::tags