//
// Created by kj16609 on 4/22/23.
//

#include "Game.hpp"

#include <moc_Game.cpp>

#include <tracy/Tracy.hpp>

#include "GameData.hpp"
#include "core/imageManager.hpp"

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

		inline static std::shared_ptr< GameData > movePtr( GameData&& data )
		{
			ZoneScoped;
			std::lock_guard guard { map_mtx };

			if ( auto itter = map.find( data.m_game_id ); itter != map.end() )
				return itter->second;
			else
			{
				auto ptr = std::make_shared< GameData >( std::move( data ) );
				map.emplace( ptr->m_game_id, ptr );
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
		RapidTransaction() << "UPDATE games SET title = ? WHERE record_id = ?" << title << m_id;
		ptr->m_title = std::move( title );
		emit dataChanged();
	}

	void Game::setCreator( QString creator )
	{
		RapidTransaction() << "UPDATE games SET creator = ? WHERE record_id = ?" << creator << m_id;
		ptr->m_creator = std::move( creator );
		emit dataChanged();
	}

	void Game::setEngine( QString engine )
	{
		RapidTransaction() << "UPDATE games SET engine = ? WHERE record_id = ?" << engine << m_id;
		ptr->m_engine = std::move( engine );
		emit dataChanged();
	}

	void Game::setDescription( QString description )
	{
		RapidTransaction() << "UPDATE games SET description = ? WHERE record_id = ?" << description << m_id;
	}

	void Game::addVersion( QString version_name, std::filesystem::path dir, std::filesystem::path executable )
	{
		auto& versions { ptr->m_versions };
		if ( versionExists( version_name ) )
		{
			//Version not found. Safe to add
			RapidTransaction()
				<< "INSERT INTO versions (record_id, version, game_path, exec_path, in_place, date_added) VALUES (?, ?, ?, ?, ?, ?)"
				<< m_id << version_name << dir << executable
				<< dir.string().starts_with( config::paths::games::getPath().string() )
				<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::steady_clock::now()
			                                                               .time_since_epoch() )
					   .count();

			versions.emplace_back( Version( m_id, version_name ) );
			emit dataChanged();
		}
		else
		{
			throw std::runtime_error( "Version with name already exists" );
		}
	}

	bool Game::versionExists( const QString& version_name )
	{
		const auto& versions { ptr->m_versions };
		return std::find_if(
				   versions.begin(),
				   versions.end(),
				   [ &version_name ]( const Version& ver ) { return ver->m_version == version_name; } )
		    == versions.end();
	}

	void Game::removeVersion( Version& info )
	{
		auto& versions { ptr->m_versions };
		const auto& version_name { info->m_version };
		//Check if version exists
		const auto itter { std::find_if(
			versions.begin(),
			versions.end(),
			[ &version_name ]( const Version& ver ) { return ver->m_version == version_name; } ) };

		if ( itter != versions.end() )
		{
			throw std::runtime_error( "Game: Tried to delete non existant version!" );
		}
		else
		{
			RapidTransaction() << "DELETE FROM versions WHERE version = ? AND record_id = ?" << info->m_version << m_id;

			//Remove from versions vector
			versions.erase( itter );
			emit dataChanged();
		}
	}

	void Game::addPlaytime( const std::uint64_t time )
	{
		const auto new_playtime { ptr->m_total_playtime + time };
		RapidTransaction() << "UPDATE games SET total_playtime = ? WHERE record_id = ?" << new_playtime << m_id;
		ptr->m_total_playtime = new_playtime;
		emit dataChanged();
	}

	void Game::setLastPlayed( const std::uint64_t time )
	{
		RapidTransaction() << "UPDATE games SET last_played_r = ? WHERE record_id = ?" << time << m_id;
		ptr->m_last_played = time;
		emit dataChanged();
	}

	void Game::reorderPreviews( std::vector< std::filesystem::path > paths )
	{
		//If the path doesn't exist then it just gets added as if it did before
		RapidTransaction() << "DELETE FROM previews WHERE record_id = ?" << m_id;

		for ( std::uint64_t i = 0; i < paths.size(); ++i )
		{
			RapidTransaction() << "INSERT INTO previews (record_id, path, position) VALUES (?,?,?)" << m_id
							   << paths[ i ] << i;
		}

		emit dataChanged();
	}

	void Game::addPreview( std::filesystem::path path, std::uint64_t index )
	{
		// If relative returns an empty string then we can safely assume that the path is not inside of the image folder
		if ( !path.string().starts_with( config::paths::images::getPath().string() ) )
		{
			path = imageManager::importImage( path, m_id ).result();
		}

		//Get the highest position
		if ( index == 0 )
		{
			RapidTransaction() << "SELECT position FROM previews WHERE record_id = ? ORDER BY position DESC LIMIT 1"
				>> index;
		}

		RapidTransaction() << "INSERT INTO previews (record_id, path, position) VALUES (?,?,?)" << m_id << path
						   << index;
		auto& previews { this->ptr->m_preview_paths };
		previews.clear();

		RapidTransaction() << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_id >>
			[ &previews ]( std::filesystem::path preview_path ) { previews.emplace_back( std::move( preview_path ) ); };

		emit dataChanged();
	}

	void Game::removePreview( const std::uint64_t index )
	{
		auto& previews { ptr->m_preview_paths };
		RapidTransaction() << "DELETE FROM previews WHERE record_id = ? AND position = ?" << m_id << index;

		auto itter { previews.begin() };
		std::advance( itter, index );
		previews.erase( itter );

		emit dataChanged();
	}

	void Game::removePreview( const std::filesystem::path path )
	{
		//Find the preview index
		const auto& previews { ptr->m_preview_paths };
		const auto index { std::find( previews.begin(), previews.end(), path ) };
		if ( index == previews.end() )
			throw std::runtime_error( "Game: Failed to find index for give preview path" );
		else
			removePreview( static_cast< uint64_t >( std::distance( previews.begin(), index ) ) );
	}

	void Game::addUserTag( QString str )
	{
		auto& tags { ptr->m_tags };
		if ( const auto itter = std::find( tags.begin(), tags.end(), str ); itter == tags.end() )
		{
			std::uint64_t tag_id { 0 };
			RapidTransaction() << "SELECT tag_id FROM tags WHERE tag = ?" << str >> tag_id;

			if ( tag_id == 0 )
			{
				RapidTransaction() << "INSERT INTO tags (tag) VALUES (?) RETURNING tag_id" << str >> tag_id;
			}

			RapidTransaction() << "INSERT INTO tag_mappings (tag_id, record_id) VALUES (?,?)" << tag_id << m_id;
			tags.emplace_back( str );
		}
	}

	void Game::removeUserTag( QString str )
	{
		auto& tags { ptr->m_tags };
		if ( const auto itter = std::find( tags.begin(), tags.end(), str ); itter != tags.end() )
		{
			RapidTransaction() << "DELETE FROM tag_mappings NATURAL JOIN WHERE tag = ? AND record_id = ?" << str
							   << m_id;
			tags.erase( itter );
		}
	}

	void Game::setBanner( std::filesystem::path path, const BannerType type )
	{
		ZoneScoped;
		int count { 0 };

		if ( !path.string().starts_with( config::paths::images::getPath().string() ) )
		{
			path = imageManager::importImage( path, m_id ).result();
		}

		RapidTransaction() << "SELECT count(*) FROM banners WHERE record_id = ? AND type = ? " << m_id
						   << static_cast< uint8_t >( type )
			>> count;

		if ( count == 0 )
		{
			//If path isn't here then we insert it instead
			RapidTransaction() << "INSERT INTO banners (record_id, path, type) VALUES (?,?,?)" << m_id << path
							   << static_cast< uint8_t >( type );
		}
		else
		{
			RapidTransaction() << "UPDATE banners SET path = ? WHERE record_id = ? AND type = ?" << path << m_id
							   << static_cast< uint8_t >( type );
		}

		this->ptr->m_banner_paths[ static_cast< std::uint64_t >( type ) ] = std::move( path );

		emit dataChanged();
	}

	void Game::connectAtlasData( const AtlasID atlas_id )
	{
		if ( atlas_id == INVALID_ATLAS_ID ) throw std::runtime_error( "Invalid atlas id" );

		AtlasID new_id { INVALID_ATLAS_ID };
		RapidTransaction() << "SELECT atlas_id FROM atlas_data WHERE atlas_id = ?" << atlas_id >> new_id;
		if ( new_id == INVALID_ATLAS_ID ) throw std::runtime_error( "No Atlas data with this id" );

		RapidTransaction() << "INSERT INTO atlas_mappings (atlas_id, game_id) VALUES (?,?)" << atlas_id << m_id;

		this->ptr->atlas_data = remote::AtlasRemoteData( atlas_id );
	}

	Version& Game::operator[]( const QString str ) const
	{
		auto ver_itter { std::find_if(
			ptr->m_versions.begin(),
			ptr->m_versions.end(),
			[ &str ]( const Version& ver ) { return ver->m_version == str; } ) };

		if ( ver_itter != ptr->m_versions.end() )
			return *ver_itter;
		else
			throw std::runtime_error( "No version of that name in game" );
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

	RecordAlreadyExists::RecordAlreadyExists( Game& record_in ) :
	  RecordException( ( "Record already exists with id " + std::to_string( record_in->m_game_id ) ).c_str() ),
	  record( record_in->m_game_id )
	{}

	InvalidRecordID::InvalidRecordID( const RecordID in_id ) :
	  RecordException( ( "Invalid record id = " + std::to_string( in_id ) ).c_str() ),
	  id( in_id )
	{}

	//Test functions
	std::string Game::findAtlasData( std::string title, std::string developer )
	{
		std::string data = "";
		spdlog::info( "{}{}", title, developer );
		/*std::string tmp = "";
		RapidTransaction() << "SELECT * FROM atlas_data WHERE id_name = ?" << title << developer >> data;
		*/
		return data;
	}
} // namespace atlas::records