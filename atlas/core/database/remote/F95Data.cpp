//
// Created by kj16609 on 7/28/23.
//

#include "F95Data.hpp"

#include <unordered_map>

#include "core/database/RapidTransaction.hpp"

namespace atlas::remote
{
	namespace internal
	{
		inline static std::unordered_map< F95ID, std::shared_ptr< F95Data > > map;
		inline static std::mutex mtx;

		F95Data::F95Data( const F95ID id )
		{
			const auto main_query {
				"SELECT f95_id, atlas_id, banner_url, site_url, last_thread_comment, thread_publish_date, "
				"last_record_update, views, likes, tags, rating, screens, replies FROM f95_zone_data WHERE f95_id = ?"
			};

			RapidTransaction() << main_query << id >> [ this ](
														  const F95ID f95_id_in,
														  const AtlasID atlas_id_in,
														  const QString banner_url_in,
														  const QString site_url_in,
														  const QString last_thread_comment_in,
														  const QString thread_publish_date_in,
														  const QString last_record_update_in,
														  const QString views_in,
														  const QString likes_in,
														  const QString tags_in,
														  const QString rating_in,
														  const QString screens_in,
														  const QString replies_in ) noexcept
			{
				f95_id = f95_id_in;
				atlas_id = atlas_id_in;
				banner_url = banner_url_in;
				site_url = site_url_in;
				last_thread_comment = last_thread_comment_in;
				thread_publish_date = thread_publish_date_in;
				last_record_update = last_record_update_in;
				views = views_in;
				likes = likes_in;
				tags = tags_in.split(",");
				rating = rating_in;
				screens = screens_in.split(",");
				replies = replies_in;
			};
		}

		std::shared_ptr< F95Data > getPtr( const F95ID id )
		{
			std::lock_guard guard { mtx };
			if ( map.contains( id ) )
			{
				return map[ id ];
			}
			else
			{
				auto ptr { std::make_shared< F95Data >( id ) };
				map.insert( { id, ptr } );
				return ptr;
			}
		}

		void releasePtr( const F95ID id )
		{
			std::lock_guard guard { mtx };
			if ( map.contains( id ) )
			{
				auto itter { map[ id ] };
				if ( itter.use_count() <= 1 ) map.erase( id );
			}
		}

	} // namespace internal

	bool hasF95DataFor( const F95ID f95_id )
	{
		F95ID id { INVALID_F95_ID };
		RapidTransaction() << "SELECT f95_id FROM f95_zone_data WHERE f95_id = ?" << f95_id >> id;
		return id != INVALID_F95_ID;
	}

	void createDummyF95Record( const F95ID f95_id )
	{
		RapidTransaction() << "INSERT INTO f95_zone_data (f95_id) VALUES (?) ON CONFLICT DO NOTHING" << f95_id;
	}

	F95RemoteData::F95RemoteData( const F95ID f95_id ) : id( f95_id ), data_ptr( internal::getPtr( f95_id ) )
	{}

	F95RemoteData::~F95RemoteData()
	{
		internal::releasePtr( id );
	}

	std::optional< atlas::remote::F95RemoteData > findF95Data( QString atlas_id )
	{
		//std::vector< std::string > data;
		std::optional< atlas::remote::F95RemoteData > data;
		//spdlog::info( "{}{}", title, developer );
		RapidTransaction() << "SELECT * FROM f95_zone_data WHERE atlas_id=?" << atlas_id >>
			[ &data ]( const F95ID f95_id ) { data = { f95_id }; };
		return data;
	}

} // namespace atlas::remote
