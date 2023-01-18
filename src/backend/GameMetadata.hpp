//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <QString>
#include <filesystem>
#include "Types.hpp"

class QSqlQuery;

struct GameMetadata
{
	QString game_name {};
	QString creator_name {};
	QString version_string {};

	//! Path to the game folder
	std::filesystem::path folder_path {};

	//! Path to the executable
	std::filesystem::path executable_path {};

	//! Executes the game
	void execGame();

	//! Returns the string to generate the table.
	static QString tableQuery();

	//! Binds values needed for a query
	void bindTo(QSqlQuery& query) const;

	static GameMetadata select(const RecordID id);
	static void update(const RecordID id, const GameMetadata& metadata);
	static GameMetadata insert(const RecordID id, const GameMetadata& metadata);

	bool operator==(const GameMetadata& other) const;
};



#endif	//HYDRUS95_GAMEMETADATA_HPP
