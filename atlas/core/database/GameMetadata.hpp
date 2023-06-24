//
// Created by kj16609 on 1/15/23.
//

#ifndef ATLAS_GAMEMETADATA_HPP
#define ATLAS_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include "Database.hpp"
#include "core/Types.hpp"
#include "core/database/Database.hpp"
#include "core/database/record/Record.hpp"

struct RecordData;

//! Representation of a game version
struct GameMetadata
{
  private:

	Record m_parent;

	QString m_version {};

  public:

	//Setters
	//! Adds playtime to this and it's parent record
	void addPlaytime( const std::uint32_t playtime );
	//! Sets the last played timestamp for this and it's parent record
	void setLastPlayed( const std::uint64_t last_played );
	//! Executes the game for this record.
	void playGame();

	void setVersionName( const QString str );
	void setRelativeExecPath( const std::filesystem::path& path );

	//Getters
	QString getVersionName() const;
	//! If return true then the game is not located in config::paths::games::get()
	bool isInPlace() const;
	std::uint32_t getPlaytime() const;
	std::uint64_t getLastPlayed() const;
	std::filesystem::path getPath() const;
	std::filesystem::path getRelativeExecPath() const;
	std::filesystem::path getExecPath() const;
	std::uint64_t getFolderSize() const;
	RecordID getParentID() const;
	std::uint64_t getImportTime() const;

  public:

	GameMetadata() = delete;

	GameMetadata( const RecordID m_id, const QString& version_in ) : m_parent( m_id ), m_version( version_in ) {}

	bool operator==( const GameMetadata& other ) const
	{
		return m_version == other.m_version && m_parent == other.m_parent;
	}

	GameMetadata( const GameMetadata& other ) noexcept : m_parent( other.m_parent ), m_version( other.m_version ) {}

	GameMetadata( GameMetadata&& other ) noexcept :
	  m_parent( other.m_parent ),
	  m_version( std::move( other.m_version ) )
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
