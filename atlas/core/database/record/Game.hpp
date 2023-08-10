//
// Created by kj16609 on 4/22/23.
//

#ifndef ATLASGAMEMANAGER_GAME_HPP
#define ATLASGAMEMANAGER_GAME_HPP


#include <QFuture>		// Keep the Qt #includes up here to appease the AutoMoc.
#include <QPixmap>

#include <filesystem>

#include "core/Types.hpp"
#include "core/config.hpp"

#include <filesystem>	// Keep this down here for the same reason.

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

		//Logging/debug
		template < typename T >
		friend QJsonObject atlas::logging::dev::internalSerializer( const T& );

		Game() = default;
		~Game();
		explicit Game( const RecordID id );

		//! Returns true if ptr points to valid game data
		inline bool valid() const { return ptr != nullptr; }

		Game& operator=( const Game& other )
		{
			ptr = other.ptr;
			m_id = other.m_id;
			return *this;
		}

		Game( const Game& other ) : QObject( other.parent() ), ptr( other.ptr ), m_id( other.m_id )
		{
			this->moveToThread( other.thread() );
		}

		//! Sets the title for the game.
		void setTitle( QString title );
		//! Sets the creator for the game
		void setCreator( QString creator );
		//! Sets the engine for the game
		void setEngine( QString engine );
		//!
		void setDescription( QString description );
		bool versionExists( const QString& str );

		//! Adds a new version. Will throw if version of same name exists.
		/**
	 * @note No files are moved during this process. Any file movement must take place BEFORE this function is called.
	 * @param dir
	 * @param executable Must be a relative path sourced from dir.
	 */
		void addVersion( QString version_name, std::filesystem::path dir, std::filesystem::path executable );
		void removeVersion( Version& info );

		//! Adds playtime to the playtime counter
		void addPlaytime( const std::uint64_t );
		//! Sets the last played timestamp
		void setLastPlayed( const std::uint64_t );

		//====================Banners/Previews======================================

		//! Returns a future for the banner to be loaded.

		[[nodiscard]] QFuture< QPixmap > requestPreviewIndex( const std::uint64_t index ) const;

		void reorderPreviews( std::vector< std::filesystem::path > paths );
		//! If index is zero then it will place it at the highest possible postion (starting at 1)
		void addPreview( std::filesystem::path path, std::uint64_t index = 0 );
		void removePreview( const std::uint64_t index );
		void removePreview( const std::filesystem::path path );

		void addUserTag( QString str );
		void removeUserTag( QString str );

		void setBanner( std::filesystem::path path, const BannerType type );
		std::filesystem::path bannerPath( const BannerType type ) const;
		[[nodiscard]] QFuture< QPixmap > requestBanner( const BannerType type ) const;
		[[nodiscard]] QFuture< QPixmap >
			requestBanner( const int width, const int height, const SCALE_TYPE scale_type, const BannerType type );
		[[nodiscard]] QFuture< QPixmap >
			requestBanner( const QSize size, const SCALE_TYPE scale_type, const BannerType type );

		//=============== Remote connection ====================================

		//! Connects this record to a atlas_data mapping
		void connectAtlasData( const AtlasID id );

		// Used to accessing internal data
		const GameData* operator->() const { return ptr.get(); }

	  public:

	  signals:
		/*
	void bannerLoaded( const BannerType type, const QPixmap pixmap );
	void
		sizedBannerLoaded( const QSize size, const SCALE_TYPE scale_type, const BannerType type, const QPixmap pixmap );
	 */
		void dataChanged();
	};	// class Game

	//! Imports a record into the database
	/**
 *
 * @param title
 * @param creator
 * @param engine
 * @param transaction
 * @throws RecordAlreadyExists
 * @return
 */
	[[nodiscard]] Game importRecord( QString title, QString creator, QString engine );
	[[nodiscard]] bool recordExists( QString title, QString creator, QString engine );

	struct RecordException : public std::runtime_error
	{
		RecordException( const char* const msg ) : std::runtime_error( msg ) {}
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
