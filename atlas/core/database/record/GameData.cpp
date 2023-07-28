//
// Created by kj16609 on 1/15/23.
//

#include "GameData.hpp"

#include <iostream>

#include <QPixmapCache>

#include "Game.hpp"
#include "Version.hpp"
#include "core/database/Database.hpp"
#include "core/imageManager.hpp"

namespace atlas::records
{
	GameData::GameData( const RecordID id ) : m_game_id( id )
	{
		ZoneScoped;
		if ( id == INVALID_RECORD_ID ) throw std::runtime_error( "Invalid record id" );

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

		if ( !exists ) throw std::runtime_error( "Record does not exist" );

		RapidTransaction() << "SELECT count(*) FROM previews WHERE record_id = ?" << m_game_id >> m_preview_count;

		AtlasID atlas_id { INVALID_ATLAS_ID };
		RapidTransaction() << "SELECT atlas_id FROM atlas_mappings WHERE record_id = ? " << m_game_id >> atlas_id;
		if ( atlas_id != INVALID_ATLAS_ID ) atlas_data = { atlas_id };

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
		RecordID record_id { 0 };
		transaction << "SELECT record_id FROM games WHERE title = ? AND creator = ? AND engine = ?"
					<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
			>> [ & ]( const RecordID id ) noexcept { record_id = id; };

		if ( record_id != 0 )
		{
			Game game { record_id };
			throw RecordAlreadyExists( game );
		}

		transaction
				<< "INSERT INTO games (title, creator, engine, last_played_r, total_playtime) VALUES (?, ?, ?, 0, 0) RETURNING record_id"
				<< title_in.toStdString() << creator_in.toStdString() << engine_in.toStdString()
			>> [ & ]( const RecordID id ) noexcept { m_game_id = id; };
	}

	const Version& GameData::getVersion( const QString name ) const
	{
		for ( const auto& version : m_versions )
			if ( version->m_version == name ) return version;

		throw std::runtime_error( "GameData: No version of name found" );
	}

	std::size_t strToTagID( const QString str )
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::size_t id { 0 };
		transaction << "SELECT tag_id FROM tags WHERE tag = ?" << str.toStdString() >> id;
		return id;
	}

	RecordID recordID( const QString& title, const QString& creator, const QString& engine )
	{
		ZoneScoped;
		RecordID record_id { INVALID_RECORD_ID };

		RapidTransaction transaction;
		transaction << "SELECT record_id FROM games WHERE title = ? AND creator = ? AND engine = ?"
					<< title.toStdString() << creator.toStdString() << engine.toStdString()
			>> [ &record_id ]( [[maybe_unused]] const RecordID id ) noexcept { record_id = id; };

		return record_id;
	}

	bool recordExists( const QString& title, const QString& creator, const QString& engine )
	try
	{
		ZoneScoped;
		return recordID( title, creator, engine );
	}
	catch ( [[maybe_unused]] const NoRows& e )
	{
		return false;
	}
} // namespace atlas::records