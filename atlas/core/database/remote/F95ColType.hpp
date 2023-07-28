//
// Created by kj16609 on 6/30/23.
//

#ifndef ATLASGAMEMANAGER_F95COLTYPE_HPP
#define ATLASGAMEMANAGER_F95COLTYPE_HPP

#include "core/database/Column.hpp"

namespace atlas::remote
{
	enum class F95Columns
	{
		AtlasIDCol,
		BannerURL,
		SiteURL,
		LastThreadComment,
		ThreadPublishDate,
		LastRecordUpdate,
		Views,
		Likes,
		Tags,
		Rating,
		Screens,
		Replies
	};

	template < F95Columns col >
	struct F95ColInfo;

	template < F95Columns col >
	using F95ColType = F95ColInfo< col >::Type;
} // namespace atlas::remote
#endif //ATLASGAMEMANAGER_F95COLTYPE_HPP
