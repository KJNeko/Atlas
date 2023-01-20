//
// Created by kj16609 on 1/15/23.
//

#include "GameMetadata.hpp"
#include "database.hpp"
#include <iostream>

void GameMetadata::execGame() {}

GameMetadata GameMetadata::select( const RecordID id )
{
	std::optional< GameMetadata > metadata { std::nullopt };

	database::db_ref() << "SELECT game_path, exec_path FROM game_metadata WHERE record_id = ?" << id >>
		[&]( const std::string& game_path, const std::string exec_path )
	{
		metadata = { game_path, exec_path };
	};

	return metadata.value();
}

void GameMetadata::update( const RecordID id, const GameMetadata& metadata )
{
	database::db_ref() << "UPDATE game_metadata SET game_path = ?, exec_path = ? WHERE record_id = ?"
					   << metadata.game_path << metadata.exec_path << id;
	return;
}

GameMetadata GameMetadata::insert( const RecordID id, const GameMetadata& metadata )
{
	database::db_ref() << "INSERT INTO game_metadata (record_id, game_path, exec_path) VALUES (?, ?, ?)" << id
					   << metadata.game_path << metadata.exec_path;
	return metadata;
}


bool GameMetadata::operator==( const GameMetadata& other ) const
{
	return game_path == other.game_path && exec_path == other.exec_path;
}
