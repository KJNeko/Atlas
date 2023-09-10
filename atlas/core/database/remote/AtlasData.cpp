//
// Created by kj16609 on 7/28/23.
//

#include "AtlasData.hpp"

#include "core/database/RapidTransaction.hpp"

namespace atlas::remote
{
	namespace internal
	{
		inline static std::unordered_map< AtlasID, std::shared_ptr< AtlasData > > map;
		inline static std::mutex mtx;

		AtlasData::AtlasData( const AtlasID id )
		{
			const auto main_query {
				"SELECT atlas_id, id_name, short_name, title, original_name, category, "
				"engine, status, version, developer, creator, overview, censored, language, "
				"translations, genre, " /* tags are done in a seperate table */ "voice, os, "
				"release_date, length, banner, banner_wide, cover, logo, wallpaper, " /* Previews are done in another table */
				"last_db_update FROM atlas_data WHERE atlas_id = ?"
			};

			RapidTransaction() << main_query << id >> [ this ](
														  const AtlasID atlas_id_in,
														  const QString id_name_in,
														  const QString short_name_in,
														  const QString title_in,
														  const QString original_name_in,
														  const QString category_in,
														  const QString engine_in,
														  const QString status_in,
														  const QString version_in,
														  const QString developer_in,
														  const QString creator_in,
														  const QString overview_in,
														  const QString censored_in,
														  const QString language_in,
														  const QString translations_in,
														  const QString genre_in,
														  const QString voice_in,
														  const QString os_in,
														  const std::uint64_t release_date_in,
														  const QString length_in,
														  const QString banner_in,
														  const QString banner_wide_in,
														  const QString cover_in,
														  const QString logo_in,
														  const QString wallpaper_in,
														  const std::uint64_t last_db_update_in ) noexcept
			{
				atlas_id = atlas_id_in;
				id_name = id_name_in;
				short_name = short_name_in;
				title = title_in;
				original_name = original_name_in;
				category = category_in;
				engine = engine_in;
				status = status_in;
				version = version_in;
				developer = developer_in;
				creator = creator_in;
				overview = overview_in;
				censored = censored_in;
				language = language_in;
				translations = translations_in;
				genre = genre_in;
				voice = voice_in;
				os = os_in;
				release_date = release_date_in;
				length = length_in;
				banner = banner_in;
				banner_wide = banner_wide_in;
				cover = cover_in;
				logo = logo_in;
				wallpaper = wallpaper_in;
				last_db_update = last_db_update_in;
			};

			if ( atlas_id == INVALID_ATLAS_ID ) throw std::runtime_error( "Invalid atlas id" );
		}

		std::shared_ptr< AtlasData > get( const AtlasID id )
		{
			std::lock_guard guard { mtx };
			if ( map.contains( id ) )
			{
				return map[ id ];
			}
			else
			{
				auto ptr { std::make_shared< AtlasData >( id ) };
				map.insert( { id, ptr } );
				return ptr;
			}
		}

		void release( AtlasID id )
		{
			std::lock_guard guard { mtx };
			if ( map.contains( id ) )
			{
				// If we are the only one in use then we just destroy the object from the map
				// Else we just do nothing
				if ( map[ id ].use_count() == 1 )
				{
					map.erase( id );
				}
			}
		}

	} // namespace internal

	AtlasRemoteData::AtlasRemoteData( AtlasID id ) : data_ptr( internal::get( id ) )
	{}

} // namespace atlas::remote
