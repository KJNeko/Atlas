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

void GameMetadata::bindTo(QSqlQuery& query) const
{
	query.bindValue(":game_path", game_path.string().empty() ? QVariant() : QString::fromStdString(game_path.string()));
	query.bindValue(":exec_path", exec_path.string().empty() ? QVariant() : QString::fromStdString(exec_path.string()));
}

GameMetadata GameMetadata::select(const RecordID id)
{
	QSqlQuery query;
	query.prepare("SELECT game_path, exec_path FROM game_metadata WHERE record_id = :id");
	GameMetadata metadata;
	query.bindValue(":id", id);
	query.exec();
	query.first();

	return {query.value("game_path").toString().toStdString(), query.value("exec_path").toString().toStdString()};
}

void GameMetadata::update(const RecordID id, const GameMetadata& metadata)
{
	QSqlQuery query;
	query.prepare("UPDATE game_metadata SET game_path = :game_path, exec_path = :exec_path WHERE record_id = :id");
	query.bindValue(":id", id);
	metadata.bindTo(query);
	query.exec();
	return;
}

GameMetadata GameMetadata::insert(const RecordID id, const GameMetadata& metadata)
{
	QSqlQuery query;
	query.prepare("INSERT INTO game_metadata (record_Id, game_path, exec_path) VALUES (:id, :game_path, :exec_path)");
	query.bindValue(":id", id);
	metadata.bindTo(query);
	if(!query.exec())
		throw std::runtime_error(query.lastError().text().toStdString());

	return metadata;
}


bool GameMetadata::operator==( const GameMetadata& other ) const
{
	return game_path == other.game_path && exec_path == other.exec_path;
}
