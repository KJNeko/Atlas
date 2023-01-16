//
// Created by kj16609 on 1/15/23.
//

#include "GameMetadata.hpp"
#include <QSqlQuery>

void GameMetadata::execGame()
{

}

QString GameMetadata::insertQuery() {return "INSERT INTO game_metadata (id, game_name, creator_name, version_string, folder_path, executable_path) VALUES (:id, :game_name, :creator_name, :version_string, :folder_path, :executable_path))";}
QString GameMetadata::updateQuery() {return "UPDATE game_metadata SET game_name = :game_name, creator_name = :creator_name, version_string = :version_string, folder_path = :folder_path WHERE id = :id";}
QString GameMetadata::tableQuery() {return "CREATE TABLE game_metadata (id, game_name, creator_name, version_string, folder_path, executable_path)";}

void GameMetadata::bindTo(QSqlQuery& query) const
{
	query.bindValue(":game_name", game_name);
	query.bindValue(":creator_name", creator_name);
	query.bindValue(":version_string", version_string);
	query.bindValue(":folder_path", folder_path.c_str());
	query.bindValue(":executable_path", executable_path.c_str());
}

GameMetadata GameMetadata::select(const RecordID id)
{

}

void GameMetadata::update(const RecordID id, const GameMetadata& metadata)
{

}

void GameMetadata::insert(const RecordID id, const GameMetadata& metadata)
{

}


bool GameMetadata::operator==( const GameMetadata& other ) const
{
	return game_name == other.game_name && creator_name == other.creator_name && version_string == other.version_string && folder_path == other.folder_path && executable_path == other.executable_path;
}
