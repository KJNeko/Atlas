//
// Created by kj16609 on 6/24/23.
//

#ifndef ATLASGAMEMANAGER_ATLASDTA_HPP
#define ATLASGAMEMANAGER_ATLASDTA_HPP

#include <cstdint>

#include <QString>

#include "core/Types.hpp"

#define COL AtlasDataColumn
#define KEY_TYPE AtlasID

DEFINE_COL_BASE( "atlas_data", "atlas_id" )

DEFINE_COL_STRUCT( QString, "id_name", AtlasDataIDName )
DEFINE_COL_STRUCT( QString, "short_name", AtlasDataShortName )
DEFINE_COL_STRUCT( QString, "title", AtlasDataTitle )
DEFINE_COL_STRUCT( QString, "original_name", AtlasDataOriginalName )
DEFINE_COL_STRUCT( QString, "category", AtlasDataCategory )
DEFINE_COL_STRUCT( QString, "engine", AtlasDataEngine )
DEFINE_COL_STRUCT( QString, "status", AtlasDataStatus )
DEFINE_COL_STRUCT( QString, "version", AtlasDataVersion )
DEFINE_COL_STRUCT( QString, "developer", AtlasDataDeveloper )
DEFINE_COL_STRUCT( QString, "creator", AtlasDataCreator )
DEFINE_COL_STRUCT( QString, "overview", AtlasDataOverview )
DEFINE_COL_STRUCT( QString, "censored", AtlasDataCensored )
DEFINE_COL_STRUCT( QString, "language", AtlasDataLanguage )
DEFINE_COL_STRUCT( QString, "genre", AtlasDataGenre )
DEFINE_COL_STRUCT( QString, "tags", AtlasDataTags )
DEFINE_COL_STRUCT( QString, "voice", AtlasDataVoice )
DEFINE_COL_STRUCT( QString, "os", AtlasDataOS )
DEFINE_COL_STRUCT( QString, "release_date", AtlasDataReleaseDate )
DEFINE_COL_STRUCT( QString, "length", AtlasDataLength )
DEFINE_COL_STRUCT( QString, "banner", AtlasDataBanner )
DEFINE_COL_STRUCT( QString, "banner_wide", AtlasDataBannerWide )
DEFINE_COL_STRUCT( QString, "cover", AtlasDataCover )
DEFINE_COL_STRUCT( QString, "logo", AtlasDataLogo )
DEFINE_COL_STRUCT( QString, "wallpaper", AtlasDataWallpaper )
DEFINE_COL_STRUCT( QString, "previews", AtlasDataPreviews )
DEFINE_COL_STRUCT( std::uint64_t, "last_db_update", AtlasLastDBUpdate )

#undef COL
#undef KEY_TYPE

//Before: 2269 bytes
//After: 1124 bytes
struct AtlasData
{
	AtlasID atlas_id;

	AtlasDataIDName id_name { atlas_id };
	AtlasDataShortName short_name { atlas_id };
	AtlasDataTitle title { atlas_id };
	AtlasDataOriginalName original_name { atlas_id };
	AtlasDataCategory category { atlas_id };
	AtlasDataEngine engine { atlas_id };
	AtlasDataStatus status { atlas_id };
	AtlasDataVersion version { atlas_id };
	AtlasDataDeveloper developer { atlas_id };
	AtlasDataCreator creator { atlas_id };
	AtlasDataOverview overview { atlas_id };
	AtlasDataCensored censored { atlas_id };
	AtlasDataLanguage language { atlas_id };
	AtlasDataGenre genre { atlas_id };
	AtlasDataTags tags { atlas_id };
	AtlasDataVoice voice { atlas_id };
	AtlasDataOS os { atlas_id };
	AtlasDataReleaseDate release_date { atlas_id };
	AtlasDataLength length { atlas_id };
	AtlasDataBanner banner { atlas_id };
	AtlasDataBannerWide banner_wide { atlas_id };
	AtlasDataCover cover { atlas_id };
	AtlasDataLogo logo { atlas_id };
	AtlasDataWallpaper wallpaper { atlas_id };
	AtlasDataPreviews previews { atlas_id };
	AtlasLastDBUpdate last_db_update { atlas_id };

	AtlasData( const AtlasID id ) : atlas_id( id ) {}
};

#endif //ATLASGAMEMANAGER_ATLASDTA_HPP
