//
// Created by kj16609 on 4/22/23.
//

#include "Game.hpp"

#include <QFuture>

#include <tracy/Tracy.hpp>

#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"
#include "core/database/remote/F95Data.hpp"
#include "core/images/images.hpp"
#include "moc_Game.cpp"

namespace atlas::records
{
	namespace internal
	{
		inline static std::unordered_map< RecordID, std::shared_ptr< GameData > > map;
		inline static std::mutex map_mtx;

		//! Returns the ptr for the given key. Returns nullptr if not found
		inline static std::shared_ptr< GameData > getPtr( const RecordID key )
		{
			ZoneScoped;
			std::lock_guard guard { map_mtx };
			if ( auto itter = map.find( key ); itter != map.end() )
				return itter->second;
			else
			{
				auto ptr = std::make_shared< GameData >( key );
				map.emplace( key, ptr );
				return ptr;
			}
		}

		inline static void releasePtr( const RecordID idkey )
		{
			ZoneScoped;
			std::lock_guard guard { map_mtx };
			auto itter { map.find( idkey ) };
			if ( itter != map.end() )
			{
				const auto& [ key, shared ] = *itter;
				if ( shared.unique() ) map.erase( itter );
			}
		}

	} // namespace internal

	Game::Game( const RecordID id ) : ptr( internal::getPtr( id ) ), m_id( id )
	{}

	Game::~Game()
	{
		internal::releasePtr( m_id );
	}

	void Game::setTitle( QString title )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		RapidTransaction() << "UPDATE games SET title = ? WHERE record_id = ?" << title << m_id;
		ptr->m_title = std::move( title );
		emit dataChanged();
	}

	void Game::setCreator( QString creator )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		RapidTransaction() << "UPDATE games SET creator = ? WHERE record_id = ?" << creator << m_id;
		ptr->m_creator = std::move( creator );
		emit dataChanged();
	}

	void Game::setEngine( QString engine )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		RapidTransaction() << "UPDATE games SET engine = ? WHERE record_id = ?" << engine << m_id;
		ptr->m_engine = std::move( engine );
		emit dataChanged();
	}

	void Game::setDescription( QString description )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		RapidTransaction() << "UPDATE games SET description = ? WHERE record_id = ?" << description << m_id;
	}

	void Game::addPlaytime( const std::uint64_t seconds )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto new_playtime { ptr->m_total_playtime + seconds };
		RapidTransaction() << "UPDATE games SET total_playtime = ? WHERE record_id = ?" << new_playtime << m_id;
		ptr->m_total_playtime = new_playtime;
		emit dataChanged();
	}

	void Game::setLastPlayed( const std::uint64_t time )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		RapidTransaction() << "UPDATE games SET last_played_r = ? WHERE record_id = ?" << time << m_id;
		ptr->m_last_played = time;
		emit dataChanged();
	}

	void Game::connectAtlasData( const AtlasID atlas_id )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		if ( atlas_id == INVALID_ATLAS_ID )
			throw RecordException( format_ns::format( "Invalid atlas id: {}", atlas_id ).c_str() );

		AtlasID new_id { INVALID_ATLAS_ID };
		RapidTransaction() << "SELECT atlas_id FROM atlas_data WHERE atlas_id = ?" << atlas_id >> new_id;
		if ( new_id == INVALID_ATLAS_ID )
			throw RecordException( format_ns::format( "No Atlas data with this id: {}", atlas_id ).c_str() );

		RapidTransaction() << "INSERT INTO atlas_mappings (atlas_id, game_id) VALUES (?,?)" << atlas_id << m_id;

		ptr->atlas_data = remote::AtlasRemoteData( atlas_id );
	}

	void Game::connectF95Data( const F95ID f95_id )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		if ( f95_id == INVALID_F95_ID )
			throw RecordException( format_ns::format( "Invalid F95 ID: {}", f95_id ).c_str() );

		F95ID new_id { INVALID_ATLAS_ID };
		RapidTransaction() << "SELECT new_id FROM f95_zone_data WHERE f95_id = ?" << f95_id >> new_id;

		if ( new_id == INVALID_F95_ID )
		{
			atlas::remote::createDummyF95Record( f95_id );
			new_id = f95_id;
		}

		RapidTransaction() << "INSERT INTO f95_zone_mappings (game_id, f95_id) VALUES (?,?)" << m_id << new_id;

		ptr->f95_data = remote::F95RemoteData( f95_id );
	}

	//! imports a new record and returns it. Will return an existing record if the record already exists
	Game importRecord( QString title, QString creator, QString engine )
	{
		ZoneScoped;
		if ( const auto id = recordID( title, creator, engine ); id != 0 ) return Game { id };

		GameData data { std::move( title ), std::move( creator ), std::move( engine ) };

		return Game( data.m_game_id );
	}

	bool recordExists( QString title, QString creator, QString engine )
	{
		ZoneScoped;
		return recordID( title, creator, engine ) != 0;
	}

	RecordID fetchRecord( QString title, QString creator, QString engine )
	{
		return recordID( title, creator, engine );
	}

	RecordAlreadyExists::RecordAlreadyExists( Game& record_in ) :
	  RecordException( ( "Record already exists with id " + std::to_string( record_in->m_game_id ) ).c_str() ),
	  record( record_in->m_game_id )
	{}

	InvalidRecordID::InvalidRecordID( const RecordID in_id ) :
	  RecordException( ( "Invalid record id = " + std::to_string( in_id ) ).c_str() ),
	  id( in_id )
	{}


} // namespace atlas::records