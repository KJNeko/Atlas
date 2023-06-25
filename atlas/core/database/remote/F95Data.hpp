//
// Created by kj16609 on 6/24/23.
//

#ifndef ATLASGAMEMANAGER_F95DATA_HPP
#define ATLASGAMEMANAGER_F95DATA_HPP

#include <cstdint>

#include <QString>

#include "core/Types.hpp"
#include "core/database/Column.hpp"

#define COL F95DataColumn
#define KEY_TYPE F95ID

DEFINE_COL_BASE( "f95_data", "f95_id" )

DEFINE_COL_STRUCT( std::uint64_t, "atlas_id", F95DataAtlasID )
DEFINE_COL_STRUCT( QString, "banner_url", F95DataBannerURL )
DEFINE_COL_STRUCT( QString, "site_url", F95DataSiteURL )
DEFINE_COL_STRUCT( std::uint64_t, "last_thread_comment", F95DataLastThreadComment )
DEFINE_COL_STRUCT( std::uint64_t, "thread_publish_date", F95DataThreadPublishDate )
DEFINE_COL_STRUCT( std::uint64_t, "last_record_update", F95DataLastRecordUpdate )
DEFINE_COL_STRUCT( QString, "views", F95DataViews )
DEFINE_COL_STRUCT( QString, "likes", F95DataLikes )
DEFINE_COL_STRUCT( QString, "tags", F95DataTags )
DEFINE_COL_STRUCT( QString, "rating", F95DataRating )
DEFINE_COL_STRUCT( QString, "screens", F95DataScreens )
DEFINE_COL_STRUCT( QString, "replies", F95DataReplies )

#undef COL
#undef KEY_TYPE

struct F95Data
{
  private:

	std::uint64_t f95_id; //Primary key. NOT THE FORM ID

  public:

	F95DataAtlasID atlas_id { f95_id };
	F95DataBannerURL banner_url { f95_id };
	F95DataSiteURL site_url { f95_id };
	F95DataLastThreadComment last_thread_comment { f95_id };
	F95DataThreadPublishDate thread_publish_date { f95_id };
	F95DataLastRecordUpdate last_record_update { f95_id };
	F95DataViews views { f95_id };
	F95DataLikes likes { f95_id };
	F95DataTags tags { f95_id };
	F95DataRating rating { f95_id };
	F95DataScreens screens { f95_id };
	F95DataReplies replies { f95_id };

	/**
	 * @param id *INTERNAL* Id of the f95 data (Not thread id)
	 */
	F95Data( const F95ID id ) : f95_id( id ) {}
};

#endif //ATLASGAMEMANAGER_F95DATA_HPP
