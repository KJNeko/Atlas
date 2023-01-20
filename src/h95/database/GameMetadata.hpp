//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <QString>
#include <filesystem>
#include "h95/Types.hpp"

class QSqlQuery;

struct GameMetadata
{
	//! Path to the game folder
	std::filesystem::path game_path {};

	//! Path to the executable
	std::filesystem::path exec_path {};

	//! Executes the game
	void execGame();

	static GameMetadata select(const RecordID id);
	static void update(const RecordID id, const GameMetadata& metadata);
	static GameMetadata insert(const RecordID id, const GameMetadata& metadata);

	bool operator==(const GameMetadata& other) const;
};



#endif	//HYDRUS95_GAMEMETADATA_HPP
