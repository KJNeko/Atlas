//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include <h95/Types.hpp>
#include <h95/database/Database.hpp>

#include <fmt/format.h>

//! Representation of a game version
struct GameMetadata
{
	QString m_version;

	//! canonical ath to the game folder
	std::filesystem::path m_game_path {};

	//! canonical path to the executable
	std::filesystem::path m_exec_path {};

	GameMetadata() = delete;
	GameMetadata(
		const QString& version_in,
		const std::filesystem::path& game_path_in,
		const std::filesystem::path& exec_path_in ) :
	  m_version( version_in ),
	  m_game_path( game_path_in ),
	  m_exec_path( exec_path_in )
	{
	}

	bool operator==( const GameMetadata& other ) const = default;

	//! Returns a populated GameMetadata for the given id from the database
	/**
	 * @param id
	 * @param transaction
	 * @return
	 */
	static std::vector< GameMetadata > select( const RecordID id, Transaction& transaction );

	//! Inserts a new set of metadata
	/**
	 * @throws MetadataAlreadyExists
	 * @param id id of the record.
	 * @param metadata
	 * @param transaction
	 * @return
	 */
	static GameMetadata insert( const RecordID id, const GameMetadata& metadata, Transaction& transaction );

	//! Erases metadata from the given id
	/**
	 * @param id
	 * @param metadata
	 * @param transaction
	 */
	static void erase( const RecordID id, const GameMetadata& metadata, Transaction& transaction );
};

struct MetadataException : public std::runtime_error
{
	MetadataException( const std::string& msg ) : std::runtime_error( msg ) {}
};

struct MetadataAlreadyExists : public MetadataException
{
	const RecordID m_id;
	const GameMetadata m_metadata;

	MetadataAlreadyExists( const RecordID id, const GameMetadata& metadata ) :
	  MetadataException( fmt::format( "Tried to insert duplicate metadata under id {}", id ) ),
	  m_id( id ),
	  m_metadata( metadata )
	{
	}
};

#endif	//HYDRUS95_GAMEMETADATA_HPP
