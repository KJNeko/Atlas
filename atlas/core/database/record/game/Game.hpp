//
// Created by kj16609 on 4/22/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_GAME_HPP
#define ATLASGAMEMANAGER_GAME_HPP

#include <QPixmap>

#include <filesystem> // Keep this down here for the same reason.

#include "core/Types.hpp"
#include "core/config/config.hpp"

template < typename T >
class QFuture;

namespace atlas::remote
{
	class AtlasRemoteData;
	class F95RemoteData;
} // namespace atlas::remote

[[maybe_unused]] constexpr bool USE_THUMBNAIL { true };
[[maybe_unused]] constexpr bool USE_FULLSIZE { false };

namespace atlas::records
{
	class Version;
	struct GameData;

	class Game final : public QObject
	{
		Q_OBJECT

		std::shared_ptr< GameData > ptr { nullptr };

		RecordID m_id { INVALID_RECORD_ID };

	  public:

		Game() = default;
		~Game();
		explicit Game( const RecordID id );

		Game& operator=( const Game& other )
		{
			ptr = other.ptr;
			m_id = other.m_id;
			return *this;
		}

		Game( const Game& other ) : QObject( other.parent() ), ptr( other.ptr ), m_id( other.m_id )
		{
			if ( other.m_id == INVALID_RECORD_ID )
				throw AtlasException( "Attempted to copy an object with an invalid record id" );
			this->moveToThread( other.thread() );
		}

		//! Returns true if ptr points to valid game data
		inline bool valid() const { return ptr != nullptr; }

		//! Returns the record id for this game
		[[nodiscard]] inline RecordID id() const { return m_id; }

		//! Sets the title for the game.
		void setTitle( QString title );
		//! Sets the creator for the game
		void setCreator( QString creator );
		//! Sets the engine for the game
		void setEngine( QString engine );
		//! Sets a description to be used
		void setDescription( QString description );

		bool versionExists( const QString& str );

		//! Get thumbnail from image. BLOCKING
		QPixmap requestThumbnail( const BannerType type );

		//! Adds a new version. Will throw if version of same name exists.
		/**
		 * @note No files are moved during this process. Any file movement must take place BEFORE this function is called.
		 * @param dir
		 * @param executable Must be a relative path sourced from dir.
		 */
		void addVersion(
			QString version_name,
			std::filesystem::path dir,
			std::filesystem::path executable,
			const std::uint64_t folder_size,
			const bool in_place );

		//! Removes a version from Atlas.
		/**
		 * @warning THIS WILL LEAVE FILES BEHIND. YOU MUST DELETE THEM YOURSELF
		 * @param info
		 */
		void removeVersion( const Version& info );

		//! Adds playtime to the playtime counter
		void addPlaytime( const std::uint64_t seconds );

		//! Template form of `addPlaytime` for taking in any chrono duration
		template < class Rep, class Period >
		inline void addPlaytime( const std::chrono::duration< Rep, Period > time_diff )
		{
			addPlaytime( std::chrono::duration_cast< std::chrono::seconds >( time_diff ).count() );
		}

		//! Sets the last played timestamp
		void setLastPlayed( const std::uint64_t );

		//====================Banners/Previews======================================

		//! Returns a future for the preview to be loaded.
		[[nodiscard]] QFuture< QPixmap > requestPreview( const std::uint64_t index, const bool use_thumbnail = false )
			const;

		void reorderPreviews( std::vector< std::filesystem::path > paths );
		//! If index is zero then it will place it at the highest possible postion (starting at 1)
		void addPreview( std::filesystem::path path, std::uint64_t index = 0 );
		void removePreview( const std::uint64_t index );
		void removePreview( const std::filesystem::path path );
		[[nodiscard]] QFuture< QPixmap > preview( const std::uint64_t index, const bool use_thumbnail = false );
		[[nodiscard]] QFuture< QPixmap > scaledPreview(
			const QSize size,
			const SCALE_TYPE scale_type,
			const Alignment align_type,
			const std::uint64_t index,
			const bool use_thumbnail = false );

		void addUserTag( QString str );
		void removeUserTag( QString str );

		void setBanner( std::filesystem::path path, const BannerType type );
		const std::filesystem::path bannerPath( const BannerType type ) const;
		[[nodiscard]] QFuture< QPixmap > requestBanner( const BannerType type, const bool use_thumbnail = false ) const;
		[[nodiscard]] QFuture< QPixmap > requestBanner(
			const int width,
			const int height,
			const SCALE_TYPE scale_type,
			const Alignment align_type,
			const BannerType type,
			const bool use_thumbnail = false );
		[[nodiscard]] QFuture< QPixmap > requestBanner(
			const QSize size,
			const SCALE_TYPE scale_type,
			const Alignment align_type,
			const BannerType type,
			const bool use_thumbnail = false );
		bool hasBanner( const BannerType type ) const;

		//=============== Remote connection ====================================

		//! Connects this record to a atlas_data mapping
		void connectAtlasData( const AtlasID id );

		void connectF95Data( const F95ID id );

		//! Used to accessing internal GameData as a const data member
		[[nodiscard]] const GameData* operator->() const { return ptr.get(); }

		[[nodiscard]] bool hasVersion( const QString str ) const;
		[[nodiscard]] Version& operator[]( const QString str ) const;

	  public:

	  signals:
		void dataChanged();
	}; // class Game

	//! Imports a record into the database
	/**
	 * @throws RecordAlreadyExists
	 * @return
	 */
	[[nodiscard]] Game importRecord( QString title, QString creator, QString engine );
	[[nodiscard]] bool recordExists( QString title, QString creator, QString engine );
	[[nodiscard]] RecordID fetchRecord( QString title, QString creator, QString engine );

	struct RecordException : public AtlasException
	{
		RecordException( const char* const msg ) : AtlasException( msg ) {}
	};

	struct RecordAlreadyExists : public RecordException
	{
		Game record;

		RecordAlreadyExists( Game& record_in );
	};

	struct InvalidRecordID : public RecordException
	{
		RecordID id;

		InvalidRecordID( const RecordID in_id );
	};
} // namespace atlas::records

//Doing this inside of the namespace breaks Qt stuff and prevents compilation
Q_DECLARE_METATYPE( atlas::records::Game )

#endif //ATLASGAMEMANAGER_GAME_HPP
