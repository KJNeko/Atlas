//
// Created by kj16609 on 1/15/23.
//

#include "GameMetadata.hpp"
#include <QSqlQuery>
#include <iostream>
#include <QSqlError>

void GameMetadata::execGame()
{

}

QString GameMetadata::tableQuery() {return "CREATE TABLE game_metadata (id, game_name, creator_name, version_string, folder_path, executable_path)";}

void GameMetadata::bindTo(QSqlQuery& query) const
{
	query.bindValue(":game_name", game_name);
	query.bindValue(":creator_name", creator_name);
	query.bindValue(":version_string", version_string);
	query.bindValue(":folder_path", QString::fromStdString(folder_path.string()));
	query.bindValue(":executable_path", QString::fromStdString(executable_path.string()));
}

GameMetadata GameMetadata::select(const RecordID id)
{
	QSqlQuery query;
	query.prepare("SELECT :game_name, :creator_name, :version_string, :folder_path, :executable_path FROM game_metadata WHERE id = :id");
	GameMetadata metadata;
	query.bindValue(":id", id);

	query.bindValue(":game_name",metadata.game_name, QSql::Out);
	query.bindValue(":creator_name", metadata.creator_name, QSql::Out);
	query.bindValue(":version_string", metadata.version_string, QSql::Out);
	QString folder_path_str;
	QString executable_path_str;
	query.bindValue(":folder_path", folder_path_str, QSql::Out);
	query.bindValue(":executable_path", executable_path_str, QSql::Out);

	query.exec();
	query.first();

	metadata.folder_path = folder_path_str.toStdString();
	metadata.executable_path = executable_path_str.toStdString();
	return metadata;
}

void GameMetadata::update(const RecordID id, const GameMetadata& metadata)
{
	QSqlQuery query;
	query.prepare("UPDATE game_metadata SET game_name = :game_name, creator_name = :creator_name, version_string = :version_string, folder_path = :folder_path, executable_path = :executable_path WHERE id = :id");
	query.bindValue(":id", id);
	metadata.bindTo(query);
	query.exec();
	return;
}

GameMetadata GameMetadata::insert(const RecordID id, const GameMetadata& metadata)
{
	QSqlQuery query;
	query.prepare("INSERT INTO game_metadata (id, game_name, creator_name, version_string, folder_path, executable_path) VALUES (:id, :game_name, :creator_name, :version_string, :folder_path, :executable_path)");
	query.bindValue(":id", id);
	metadata.bindTo(query);
	query.exec();

	return metadata;
}


bool GameMetadata::operator==( const GameMetadata& other ) const
{
	return game_name == other.game_name && creator_name == other.creator_name && version_string == other.version_string && folder_path == other.folder_path && executable_path == other.executable_path;
}
