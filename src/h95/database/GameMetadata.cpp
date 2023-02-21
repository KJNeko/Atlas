//
// Created by kj16609 on 1/15/23.
//

#include <string>

#include <tracy/Tracy.hpp>

#include <h95/database/database.hpp>

#include <h95/database/GameMetadata.hpp>

void GameMetadata::execGame() {}

std::vector< GameMetadata > GameMetadata::select( const RecordID id )
{
	ZoneScoped;
	std::vector< GameMetadata > metadata;

	database::db_ref() << "SELECT game_path, exec_path, version FROM game_metadata WHERE record_id = ?" << id >>
		[&metadata](
			const std::string& game_path_in,
			const std::string& exec_path_in,
			const std::string& version_in )
	{
		metadata.emplace_back( QString::fromStdString( version_in ), game_path_in, exec_path_in );
	};

	return metadata;
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
