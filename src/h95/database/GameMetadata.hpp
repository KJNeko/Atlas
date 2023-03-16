//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_GAMEMETADATA_HPP
#define HYDRUS95_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include <fmt/format.h>

#include "h95/Types.hpp"
#include "h95/database/Database.hpp"

//! Representation of a game version
struct GameMetadata
{
	QString m_version;

	//! canonical ath to the game folder
	std::filesystem::path m_game_path {};

	//! canonical path to the executable
	std::filesystem::path m_exec_path {};

	//! Indicates that we don't control where the game was placed.
	bool m_in_place;

	std::uint32_t m_total_playtime;
	std::uint64_t m_last_played;

	GameMetadata() = delete;
	GameMetadata(
		const QString& version_in,
		const std::filesystem::path& game_path_in,
		const std::filesystem::path& exec_path_in,
		const bool in_place,
		const std::uint64_t last_played,
		const std::uint32_t total_playtime ) :
	  m_version( version_in ),
	  m_game_path( game_path_in ),
	  m_exec_path( exec_path_in ),
	  m_in_place( in_place ),
	  m_total_playtime( total_playtime ),
	  m_last_played( last_played )
	{
	}

	bool operator==( const GameMetadata& other ) const = default;
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
