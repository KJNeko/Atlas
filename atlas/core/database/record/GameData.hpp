//
// Created by kj16609 on 1/13/23.
//

#ifndef ATLAS_RECORD_HPP
#define ATLAS_RECORD_HPP

#include "Version.hpp"
#include "core/Types.hpp"
#include "core/database/remote/AtlasData.hpp"

namespace atlas::records
{
	struct GameData
	{
		GameData() = delete;
		GameData( GameData&& other ) = default;

		RecordID m_game_id { INVALID_RECORD_ID };
		QString m_title {};
		QString m_creator {};
		QString m_engine {};
		std::uint64_t m_last_played { 0 };
		std::uint64_t m_total_playtime { 0 };
		std::uint64_t m_preview_count { 0 };
		std::vector< QString > m_tags {};
		QString m_description {};
		std::vector< Version > m_versions {};
		std::array< std::filesystem::path, BannerType::SENTINEL > m_banner_paths {};
		std::vector< std::filesystem::path > m_preview_paths {};

		std::optional< remote::AtlasRemoteData > atlas_data { std::nullopt };

		//Some helpers for commonly used data sets.
		const Version& getVersion( const QString ) const;

		//! Fetches the RecordData for the given ID
		/**
	 * @param id
	 * @param transaction. Defaults to autocommit on return
	 */
		GameData( const RecordID id );

		/**
	 * @warning Constructing will create a new record in the database. Pass in Transaction as last parameter in order to not commit on return
	 * @param title
	 * @param creator
	 * @param engine
	 * @param transaction
	 */
		explicit GameData( QString title, QString creator, QString engine );

		friend struct RecordAlreadyExists;
		friend class Game;
	};

	//! Returns 0 if there is not record with this data
	RecordID recordID( const QString& title, const QString& creator, const QString& engine );
} // namespace atlas::records

#endif //ATLAS_RECORD_HPP
