//
// Created by kj16609 on 7/28/23.
//

#ifndef ATLASGAMEMANAGER_ATLASDATA_HPP
#define ATLASGAMEMANAGER_ATLASDATA_HPP

#include <QString>

#include <memory>

#include "core/Types.hpp"

namespace atlas::remote
{
	namespace internal
	{
		struct AtlasData
		{
			AtlasID atlas_id { INVALID_ATLAS_ID };
			QString id_name {};
			QString short_name {};
			QString title {};
			QString original_name {};
			QString category {};
			QString engine {};
			QString status {};
			QString version {};
			QString developer {};
			QString creator {};
			QString overview {};
			QString censored {};
			QString language {};
			QString translations {};
			QString genre {};
			std::vector< QString > tags {};
			QString voice {};
			QString os {};
			std::uint64_t release_date { 0 };
			QString length {};
			QString banner {};
			QString banner_wide {};
			QString cover {};
			QString logo {};
			QString wallpaper {};
			std::vector< QString > previews {};
			//! Time of the last remote update (Not the time we've synced)
			std::uint64_t last_db_update { 0 };

			AtlasData( const AtlasID id );
		};

	} // namespace internal

	[[nodiscard]] AtlasID atlasIDFromF95Thread( const F95ID thread_id );

	class AtlasRemoteData
	{
		std::shared_ptr< internal::AtlasData > data_ptr;

	  public:

		AtlasRemoteData( AtlasID id );

		const internal::AtlasData* operator->() const { return data_ptr.get(); }
	};

	std::optional< atlas::remote::AtlasRemoteData > findAtlasData( const AtlasID atlas_id );

	std::optional< atlas::remote::AtlasRemoteData > findAtlasData( QString title, QString developer );
} // namespace atlas::remote

#endif //ATLASGAMEMANAGER_ATLASDATA_HPP
