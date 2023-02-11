//
// Created by kj16609 on 1/15/23.
//

#include "GameMetadata.hpp"
#include "database.hpp"
#include <iostream>
#include <tracy/Tracy.hpp>

void GameMetadata::execGame() {}

GameMetadata GameMetadata::select( const RecordID id )
{
	ZoneScoped;
	std::optional< GameMetadata > metadata { std::nullopt };

	database::db_ref() << "SELECT game_path, exec_path, version FROM game_metadata WHERE record_id = ?" << id >>
		[&]( const std::string& game_path_in, const std::string& exec_path_in, const std::string& version_in )
	{
		metadata = { QString::fromStdString( version_in ), game_path_in, exec_path_in };
	};

	return metadata.value();
}

void GameMetadata::update( const RecordID id, const GameMetadata& metadata )
{
	ZoneScoped;
	database::db_ref() << "UPDATE game_metadata SET game_path = ?, exec_path = ? WHERE record_id = ? AND version = ?"
					   << metadata.game_path.string() << metadata.exec_path.string() << id
					   << metadata.version.toStdString();
	return;
}

GameMetadata GameMetadata::insert( const RecordID id, const GameMetadata& metadata )
{
	ZoneScoped;
	database::db_ref() << "INSERT INTO game_metadata (record_id, game_path, exec_path, version) VALUES (?, ?, ?, ?)"
					   << id << metadata.game_path.string() << metadata.exec_path.string()
					   << metadata.version.toStdString();
	return metadata;
}
