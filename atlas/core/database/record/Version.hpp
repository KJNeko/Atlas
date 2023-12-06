//
// Created by kj16609 on 1/15/23.
//

#pragma once
#ifndef ATLAS_GAMEMETADATA_HPP
#define ATLAS_GAMEMETADATA_HPP

#include <QString>

#include <filesystem>

#include "core/Types.hpp"
#include "core/database/record/game/Game.hpp"

namespace atlas::records
{
	class Game;
	struct GameData;

	struct VersionData
	{
		RecordID m_parent_id { INVALID_RECORD_ID };
		QString m_version {};
		std::filesystem::path m_game_path {};
		std::filesystem::path m_exec_path {};
		std::uint64_t m_last_played { 0 };
		std::uint64_t m_total_playtime { 0 };
		std::uint64_t m_folder_size { 0 };
		std::uint64_t m_date_added { 0 };

		VersionData() = delete;
		VersionData( const RecordID id, const QString& name );
		VersionData( const VersionData& other ) = delete;
		VersionData( VersionData&& other ) = delete;
	};

	//! Representation of a game version
	class Version
	{
		RecordID m_parent_id;
		QString m_version;

		std::shared_ptr< VersionData > data_ptr;

	  public:

		const VersionData* operator->() const
		{
			if ( data_ptr == nullptr ) throw AtlasException( "Version: Pointer was nullptr!" );
			return data_ptr.get();
		}

		//Setters
		//! Adds playtime to this and it's parent record
		void addPlaytime( const std::uint32_t playtime );

		template < class Rep, class Period >
		void addPlaytime( const std::chrono::duration< Rep, Period > time_diff )
		{
			addPlaytime( static_cast< std::uint32_t >( std::chrono::duration_cast< std::chrono::seconds >( time_diff )
			                                               .count() ) );
		}

		//! Sets the last played timestamp for this and it's parent record
		void setLastPlayed( const std::uint64_t last_played );
		//! Executes the game for this record.
		void playGame();

		void setVersionName( const QString str );
		void setRelativeExecPath( const std::filesystem::path& path );

		//Getters
		QString getVersionName() const;
		//! If true then the game is not located in config::paths::games::get()
		bool isInPlace() const;
		std::uint32_t getPlaytime() const;
		std::uint64_t getLastPlayed() const;
		std::filesystem::path getPath() const;
		std::filesystem::path getRelativeExecPath() const;
		std::filesystem::path getExecPath() const;
		std::uint64_t getFolderSize() const;
		RecordID getParentID() const;
		std::uint64_t getImportTime() const;

		Version() = delete;

		bool operator==( const Version& other ) const
		{
			return m_version == other.m_version && m_parent_id == other.m_parent_id;
		}

		Version( const RecordID, const QString& name );
		~Version();

		Version( const Version& other ) noexcept :
		  m_parent_id( other.m_parent_id ),
		  m_version( other.m_version ),
		  data_ptr( other.data_ptr )
		{}

		Version( Version&& other ) noexcept :
		  m_parent_id( other.m_parent_id ),
		  m_version( std::move( other.m_version ) ),
		  data_ptr( other.data_ptr )
		{}

		Game parent() const;

		//! Required to make std::vector happy
		Version& operator=( const Version& other ) noexcept
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

		MetadataAlreadyExists( const RecordID id, const Version& metadata ) :
		  MetadataException( "Tried to insert duplicate metadata" ),
		  m_id( id ),
		  m_metadata( metadata.getVersionName() )
		{}
	};
} // namespace atlas::records
#endif //ATLAS_GAMEMETADATA_HPP
