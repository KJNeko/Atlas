//
// Created by kj16609 on 1/15/23.
//

#ifndef ATLAS_GAMEMETADATA_HPP
#define ATLAS_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include "h95/Types.hpp"
#include "h95/database/Database.hpp"

struct RecordData;

//! Representation of a game version
struct GameMetadata
{
  private:

	RecordData& m_parent;

	QString m_version {};

	std::filesystem::path m_game_path {};

	std::filesystem::path m_exec_path {};

	//! Indicates that we don't control where the game was placed.
	bool m_in_place { false };

	std::uint32_t m_total_playtime { 0 };
	std::uint64_t m_last_played { 0 };

	std::uint64_t m_folder_size { 0 };

  public:

	//Setters
	//! Adds playtime to this and it's parent record
	void addPlaytime( const std::uint32_t playtime );
	//! Sets the last played timestamp for this and it's parent record
	void setLastPlayed( const std::uint64_t last_played );
	//! Executes the game for this record.
	void playGame();

	//Getters
	QString getVersionName() const;
	//! If return true then the game is not located in config::paths::games::get()
	bool isInPlace() const;
	std::uint32_t getPlaytime() const;
	std::uint64_t getLastPlayed() const;
	std::filesystem::path getPath() const;
	std::filesystem::path getRelativeExecPath() const;
	std::filesystem::path getExecPath() const;

  public:

	GameMetadata() = delete;

	GameMetadata(
		RecordData& parent,
		const QString& version_in,
		const std::filesystem::path& game_path_in,
		const std::filesystem::path& exec_path_in,
		const bool in_place,
		const std::uint64_t last_played,
		const std::uint32_t total_playtime,
		const std::uint64_t folder_size ) :
	  m_parent( parent ),
	  m_version( version_in ),
	  m_game_path( game_path_in ),
	  m_exec_path( exec_path_in ),
	  m_in_place( in_place ),
	  m_total_playtime( total_playtime ),
	  m_last_played( last_played ),
	  m_folder_size( folder_size )
	{}

	bool operator==( const GameMetadata& other ) const
	{
		return m_version == other.m_version && m_game_path == other.m_game_path && m_exec_path == other.m_exec_path;
	}

	GameMetadata( const GameMetadata& other ) :
	  m_parent( other.m_parent ),
	  m_version( other.m_version ),
	  m_game_path( other.m_game_path ),
	  m_exec_path( other.m_exec_path ),
	  m_in_place( other.m_in_place ),
	  m_total_playtime( other.m_total_playtime ),
	  m_last_played( other.m_last_played )
	{}

	GameMetadata( GameMetadata&& other ) :
	  m_parent( other.m_parent ),
	  m_version( std::move( other.m_version ) ),
	  m_game_path( std::move( other.m_game_path ) ),
	  m_exec_path( std::move( other.m_exec_path ) ),
	  m_in_place( other.m_in_place ),
	  m_total_playtime( other.m_total_playtime ),
	  m_last_played( other.m_last_played )
	{}

	//! Required to make std::vector happy
	GameMetadata& operator=( const GameMetadata& other )
	{
		std::construct_at( this, other );
		return *this;
	}
};

struct MetadataException : public std::runtime_error
{
	MetadataException( const std::string& msg ) : std::runtime_error( msg ) {}
};

struct MetadataAlreadyExists : public MetadataException
{
	const RecordID m_id;
	const QString m_metadata;

	MetadataAlreadyExists( const RecordID id, const GameMetadata& metadata ) :
	  MetadataException( "Tried to insert duplicate metadata" ),
	  m_id( id ),
	  m_metadata( metadata.getVersionName() )
	{}
};

#endif //ATLAS_GAMEMETADATA_HPP
