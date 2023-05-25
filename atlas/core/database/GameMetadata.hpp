//
// Created by kj16609 on 1/15/23.
//

#ifndef ATLAS_GAMEMETADATA_HPP
#define ATLAS_GAMEMETADATA_HPP

#include <filesystem>

#include <QString>

#include "Database.hpp"
#include "Record.hpp"
#include "core/Types.hpp"
#include "core/database/Database.hpp"

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
	void addPlaytime( const std::uint32_t playtime, Transaction transaction = Transaction( Autocommit ) );
	//! Sets the last played timestamp for this and it's parent record
	void setLastPlayed( const std::uint64_t last_played, Transaction transaction = Transaction( Autocommit ) );
	//! Executes the game for this record.
	void playGame( Transaction transaction = Transaction( Autocommit ) );

	void setVersionName( const QString str, Transaction transaction = Transaction( Autocommit ) );
	void setRelativeExecPath( const std::filesystem::path& path, Transaction transaction = Transaction( Autocommit ) );

	//Getters
	QString getVersionName() const;
	//! If return true then the game is not located in config::paths::games::get()
	bool isInPlace( Transaction transaction = Transaction( Autocommit ) ) const;
	std::uint32_t getPlaytime( Transaction transaction = Transaction( Autocommit ) ) const;
	std::uint64_t getLastPlayed( Transaction transaction = Transaction( Autocommit ) ) const;
	std::filesystem::path getPath( Transaction transaction = Transaction( Autocommit ) ) const;
	std::filesystem::path getRelativeExecPath( Transaction transaction = Transaction( Autocommit ) ) const;
	std::filesystem::path getExecPath( Transaction transaction = Transaction( Autocommit ) ) const;
	std::uint64_t getFolderSize( Transaction transaction = Transaction( Autocommit ) ) const;
	RecordID getParentID() const;
	std::uint64_t getImportTime( Transaction transaction = Transaction( Autocommit ) ) const;

  public:

	GameMetadata() = delete;

	GameMetadata(
		const RecordID m_id, const QString& version_in, Transaction transaction = Transaction( Autocommit ) ) :
	  m_parent( m_id, transaction ),
	  m_version( version_in )
	{}

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
