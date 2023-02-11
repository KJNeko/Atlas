//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <QString>
#include <filesystem>
#include "h95/Types.hpp"

struct GameMetadata
{
	QString version;

	//! Path to the game folder
	std::filesystem::path game_path {};

	//! Path to the executable
	std::filesystem::path exec_path {};

	//! Executes the game
	void execGame();

	//! Returns a populated GameMetadata for the given id from the database
	static GameMetadata select( const RecordID id );

	//! Updates information in the database for given metadata and id
	static void update( const RecordID id, const GameMetadata& metadata );

	//! Inserts a new set of metadata (Calls update if already exists)
	static GameMetadata insert( const RecordID id, const GameMetadata& metadata );

};


#endif	//HYDRUS95_GAMEMETADATA_HPP
