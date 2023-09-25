//
// Created by kj16609 on 7/29/23.
//

#include "tags.hpp"

#include "core/database/RapidTransaction.hpp"

namespace atlas::tags
{
	TagID createTag( const QString& str )
	{
		const auto trimmed { str.trimmed() };
		TagID tag_id { resolve( str ) };

		if ( tag_id != INVALID_TAG_ID )
			return tag_id;
		else
		{
			RapidTransaction() << "INSERT INTO tags (tag) VALUES (?) RETURNING tag_id" << trimmed >> tag_id;
			if ( tag_id == INVALID_TAG_ID )
				throw AtlasException( format_ns::
				                          format( "Something went wrong while trying to add tag {} to DB", str ) );
			return tag_id;
		}
	}

	bool exists( const QString& str )
	{
		return resolve( str ) != INVALID_TAG_ID;
	}

	TagID resolve( const QString& str )
	{
		TagID tag_id { INVALID_TAG_ID };
		RapidTransaction() << "SELECT tag_id FROM tags WHERE tag = ?" << str >> tag_id;
		return tag_id;
	}

} // namespace atlas::tags