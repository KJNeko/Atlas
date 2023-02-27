//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include <h95/Types.hpp>

struct GameMetadata
{
	QString version;

	//! Path to the game folder
	std::filesystem::path game_path {};

	//! Path to the executable
	std::filesystem::path m_exec_path {};

	QString m_custom_launch_str {""};
	bool m_use_custom_launch_config {false};
	bool m_ignore_global_launch_config {false};

	GameMetadata() = delete;
	GameMetadata(
		const QString& version_in,
		const std::filesystem::path& game_path_in,
		const std::filesystem::path& exec_path_in ) :
	  version( version_in ),
	  game_path( game_path_in ),
	  exec_path( exec_path_in )
	{
	}

	//! Executes the game
	void execGame();

	//! Returns a populated GameMetadata for the given id from the database
	static std::vector< GameMetadata > select( const RecordID id );

	//! Updates information in the database for given metadata and id
	static void update( const RecordID id, const GameMetadata& metadata );

	//! Inserts a new set of metadata (Calls update if already exists)
	static GameMetadata insert( const RecordID id, const GameMetadata& metadata );
};


#endif	//HYDRUS95_GAMEMETADATA_HPP
