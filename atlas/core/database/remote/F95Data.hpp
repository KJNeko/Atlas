//
// Created by kj16609 on 7/28/23.
//

#ifndef ATLASGAMEMANAGER_F95DATA_HPP
#define ATLASGAMEMANAGER_F95DATA_HPP

#include <QString>

#include <memory>

#include "core/Types.hpp"

namespace atlas::remote
{

	namespace internal
	{
		struct F95Data
		{
			F95ID f95_id { INVALID_F95_ID };
			AtlasID atlas_id { INVALID_ATLAS_ID };
			QString banner_url {};
			QString site_url {};
			QString last_thread_comment {};
			QString thread_publish_date {};
			QString last_record_update {};
			QString views {};
			QString likes {};
			std::vector< QString > tags {};
			QString rating {};
			std::vector< QString > screens {};
			QString replies {};

			F95Data( const F95ID id );
		};

	} // namespace internal

	bool hasF95DataFor( const F95ID f95_id );
	void createDummyF95Record( const F95ID f95_id );

	class F95RemoteData
	{
		F95ID id;
		std::shared_ptr< internal::F95Data > data_ptr;

	  public:

		F95RemoteData( const F95ID f95_id );
		~F95RemoteData();

		const internal::F95Data* operator->() const { return data_ptr.get(); }
	};
} // namespace atlas::remote

#endif //ATLASGAMEMANAGER_F95DATA_HPP
