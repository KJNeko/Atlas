//
// Created by kj16609 on 1/15/23.
//

#include "GameData.hpp"

#include <QPixmapCache>

#include <iostream>

#include "Version.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/game/Game.hpp"

namespace atlas::records
{
	GameData::GameData( const RecordID id ) : m_game_id( id )
	{
		ZoneScoped;
		if ( id == INVALID_RECORD_ID ) throw AtlasException( "Invalid record id" );

		bool exists { false };
		RapidTransaction()
				<< "SELECT title, creator, engine, last_played_r, total_playtime, description FROM games WHERE record_id = ?"
				<< m_game_id
			>> [ & ](
				   QString title,
				   QString creator,
				   QString engine,
				   std::uint64_t last_played,
				   const std::uint64_t total_playtime,
				   QString desc ) noexcept
		{
			exists = true;
			m_title = std::move( title );
			m_creator = std::move( creator );
			m_engine = std::move( engine );
			m_last_played = last_played;
			m_total_playtime = total_playtime;
			m_description = std::move( desc );
		};

		if ( !exists ) throw AtlasException( "Record does not exist" );

		RapidTransaction() << "SELECT count(*) FROM previews WHERE record_id = ?" << m_game_id >> m_preview_count;

		AtlasID atlas_id { INVALID_ATLAS_ID };
		RapidTransaction() << "SELECT atlas_id FROM atlas_mappings WHERE record_id = ? " << m_game_id >> atlas_id;
		if ( atlas_id != INVALID_ATLAS_ID ) atlas_data = { atlas_id };

		F95ID f95_id { INVALID_F95_ID };
		RapidTransaction() << "SELECT f95_id FROM f95_zone_mappings WHERE record_id = ?" << m_game_id >> f95_id;
		if ( f95_id != INVALID_F95_ID ) f95_data = { f95_id };

		RapidTransaction() << "SELECT version FROM versions WHERE record_id = ?" << m_game_id >>
			[ & ]( const QString version ) { m_versions.emplace_back( Version( this->m_game_id, version ) ); };

		RapidTransaction() << "SELECT path, type FROM banners WHERE record_id = ?" << m_game_id >>
			[ & ]( std::filesystem::path path, const uint8_t type ) noexcept
		{ this->m_banner_paths[ type ] = std::move( path ); };

		RapidTransaction() << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_game_id >>
			[ & ]( std::filesystem::path path ) { this->m_preview_paths.emplace_back( std::move( path ) ); };
	}

	GameData::GameData( QString title_in, QString creator_in, QString engine_in )
	{
		ZoneScoped;
		RapidTransaction transaction;
		RecordID record_id { INVALID_RECORD_ID };
		transaction << "SELECT record_id FROM games WHERE title = ? AND creator = ? AND engine = ?" << title_in
					<< creator_in << engine_in
			>> record_id;

		if ( record_id != INVALID_RECORD_ID )
		{
			Game game { record_id };
			throw RecordAlreadyExists( game );
		}

		transaction
				<< "INSERT INTO games (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, 0, 0) RETURNING record_id"
				<< title_in << creator_in << engine_in
			>> [ & ]( const RecordID id ) noexcept { m_game_id = id; };
	}

	const Version& GameData::getVersion( const QString name ) const
	{
		for ( const auto& version : m_versions )
			if ( version->m_version == name ) return version;

		throw AtlasException( "GameData: No version of name found" );
	}

	RecordID recordID( const QString& title, const QString& creator, const QString& engine )
	{
		ZoneScoped;
		RecordID record_id { INVALID_RECORD_ID };

		RapidTransaction transaction;
		transaction << "SELECT record_id FROM games WHERE title = ? AND creator = ? AND engine = ?" << title << creator
					<< engine
			>> record_id;

		return record_id;
	}

	//! Helper function. Returns if a title,creator,engine combo can be found.
	bool recordExists( const QString& title, const QString& creator, const QString& engine )
	{
		ZoneScoped;
		return recordID( title, creator, engine ) != INVALID_RECORD_ID;
	}
} // namespace atlas::records