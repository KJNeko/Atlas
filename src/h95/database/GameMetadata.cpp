//
// Created by kj16609 on 1/15/23.
//

#include <string>

#include <h95/database/Database.hpp>
#include <h95/logging.hpp>
#include <h95/database/GameMetadata.hpp>

std::vector< GameMetadata > GameMetadata::select( const RecordID id, Transaction& transaction )
{
	std::vector< GameMetadata > metadata;

	spdlog::debug( "Selecting metadata for id {}", id );

	transaction << "SELECT game_path, exec_path, version FROM game_metadata WHERE record_id = ?" << id >>
		[&metadata]( const std::string& game_path_in, const std::string& exec_path_in, const std::string& version_in )
	{
		metadata.emplace_back( QString::fromStdString( version_in ), game_path_in, exec_path_in );
	};

	return metadata;
}

GameMetadata GameMetadata::insert( const RecordID id, const GameMetadata& metadata, Transaction& transaction )
{
	bool found { false };

	// Search for duplicates
	transaction
			<< "SELECT record_id FROM game_metadata WHERE record_id = ? AND game_path = ? AND exec_path = ? AND version = ?"
			<< id << metadata.m_game_path.string() << metadata.m_exec_path.string() << metadata.m_version.toStdString()
		>> [&]()
	{
		found = true;
	};

	if ( found )
	{
		transaction.abort();
		throw MetadataAlreadyExists( id, metadata );
	}

	transaction << "INSERT INTO game_metadata (record_id, game_path, exec_path, version) VALUES (?, ?, ?, ?)" << id
				<< metadata.m_game_path.string() << metadata.m_exec_path.string() << metadata.m_version.toStdString();

	return metadata;
}

void GameMetadata::erase( const RecordID id, const GameMetadata& metadata, Transaction& transaction )
{
	transaction << "DELETE FROM game_metadata WHERE record_id = ? AND game_path = ? AND exec_path = ? AND version = ?"
				<< id << metadata.m_game_path.string() << metadata.m_exec_path.string()
				<< metadata.m_version.toStdString();
}
