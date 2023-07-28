//
// Created by kj16609 on 1/15/23.
//

#include "Version.hpp"

#include "core/config.hpp"
#include "core/database/Database.hpp"
#include "core/database/record/GameData.hpp"
#include "core/logging.hpp"
#include "core/utils/execute/executeProc.hpp"

namespace atlas::records
{
	QString Version::getVersionName() const
	{
		return m_version;
	}

	bool Version::isInPlace() const
	try
	{
		ZoneScoped;
		bool in_place { false };
		RapidTransaction transaction;
		transaction << "SELECT in_place FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
					<< m_version.toStdString()
			>> in_place;

		return in_place;
	}
	catch ( const std::exception& e )
	{
		spdlog::error( "({},{})->GameMetadata::isInPlace: {}", parent()->m_game_id, this->m_version, e.what() );
		std::rethrow_exception( std::current_exception() );
	}
	catch ( ... )
	{
		spdlog::error( "({},{})->GameMetadata::isInPlace: Unknown exception", parent()->m_game_id, this->m_version );
		std::rethrow_exception( std::current_exception() );
	}

	std::uint32_t Version::getPlaytime() const
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::uint32_t total_playtime { 0 };

		transaction << "SELECT version_playtime FROM versions WHERE record_id = ? AND version = ?"
					<< parent()->m_game_id << m_version.toStdString()
			>> total_playtime;

		return total_playtime;
	}

	std::uint64_t Version::getLastPlayed() const
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::uint64_t last_played { 0 };

		transaction << "SELECT last_played FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
					<< m_version.toStdString()
			>> last_played;

		return last_played;
	}

	std::filesystem::path Version::getPath() const
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::string path;
		transaction << "SELECT game_path FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
					<< m_version.toStdString()
			>> path;

		if ( isInPlace() )
			return { path };
		else
			return config::paths::games::getPath() / std::filesystem::path( path );
	}

	std::filesystem::path Version::getRelativeExecPath() const
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::string str;
		transaction << "SELECT exec_path FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
					<< m_version.toStdString()
			>> str;
		return { str };
	}

	std::filesystem::path Version::getExecPath() const
	{
		ZoneScoped;
		return getPath() / getRelativeExecPath();
	}

	void Version::playGame()
	{
		ZoneScoped;
		if ( const auto executable = getExecPath(); std::filesystem::exists( executable ) )
		{
			const std::chrono::time_point< std::chrono::system_clock > now { std::chrono::system_clock::now() };

			executeProc( QString::fromStdString( executable.string() ) );

			const auto duration {
				std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now() - now )
			};

			addPlaytime( static_cast< uint32_t >( duration.count() ) );
			setLastPlayed( static_cast<
						   uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( now.time_since_epoch() )
			                               .count() ) );
			return;
		}
		else
			spdlog::error( "Failed to launch game with executable {}", executable.string() );
		//TODO: Need to make a hookable static object or something
		// in order to indicate to it to launch the game
		// that way we can eventually add in the ability to prevent other games from being launched and such.
	}

	void Version::addPlaytime( const std::uint32_t playtime )
	{
		ZoneScoped;
		RapidTransaction transaction;
		transaction << "UPDATE versions SET version_playtime = ? WHERE record_id = ? AND version = ?"
					<< playtime + getPlaytime() << parent()->m_game_id << m_version.toStdString();

		parent().addPlaytime( playtime );
	}

	void Version::setLastPlayed( const std::uint64_t last_played )
	{
		ZoneScoped;
		RapidTransaction transaction;
		transaction << "UPDATE versions SET last_played = ? WHERE record_id = ? AND version = ?" << last_played
					<< parent()->m_game_id << m_version.toStdString();

		parent().setLastPlayed( last_played );
	}

	std::uint64_t Version::getFolderSize() const
	{
		ZoneScoped;
		RapidTransaction transaction;
		std::size_t folder_size { 0 };
		transaction << "SELECT folder_size FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
					<< m_version.toStdString()
			>> folder_size;
		return folder_size;
	}

	RecordID Version::getParentID() const
	{
		return m_parent_id;
	}

	void Version::setVersionName( const QString str )
	{
		ZoneScoped;
		RapidTransaction transaction;
		transaction << "UPDATE versions SET version = ? WHERE record_id = ? AND version = ?" << str.toStdString()
					<< parent()->m_game_id << m_version.toStdString();
		m_version = str;
	}

	void Version::setRelativeExecPath( const std::filesystem::path& path )
	{
		ZoneScoped;
		RapidTransaction transaction;
		transaction << "UPDATE versions SET exec_path = ? WHERE record_id = ? AND version = ?" << path.string()
					<< parent()->m_game_id << m_version.toStdString();
	}

	std::uint64_t Version::getImportTime() const
	{
		ZoneScoped;
		RapidTransaction trans;
		std::uint64_t import_time { 0 };
		trans << "SELECT date_added FROM versions WHERE record_id = ? AND version = ?" << parent()->m_game_id
			  << m_version.toStdString()
			>> import_time;

		return import_time;
	}

	Game Version::parent() const
	{
		return Game( m_parent_id );
	}

	struct VersionKey
	{
		RecordID id;
		QString version_name;

		bool operator==( const VersionKey& other ) const
		{
			return id == other.id && version_name == other.version_name;
		}
	};
} // namespace atlas::records

template <>
struct std::hash< atlas::records::VersionKey >
{
	std::size_t operator()( const atlas::records::VersionKey& data ) const noexcept
	{
		std::size_t id_hash { std::hash< RecordID > {}( data.id ) };
		std::size_t name_hash { std::hash< QString > {}( data.version_name ) };
		return id_hash ^ ( name_hash << 1 );
	}
};

namespace atlas::records
{
	namespace internal
	{
		inline static std::unordered_map< VersionKey, std::shared_ptr< VersionData > > map;
		inline static std::mutex map_mtx;

		inline std::shared_ptr< VersionData > getPtr( const RecordID id, const QString version )
		{
			ZoneScoped;
			const VersionKey key { id, version };
			std::lock_guard guard { map_mtx };
			if ( auto itter = map.find( key ); itter != map.end() )
				return itter->second;
			else
			{
				auto ptr { std::make_shared< VersionData >( id, version ) };
				map.emplace( key, ptr );
				return ptr;
			}
		}

		inline void releasePtr( const RecordID id, const QString version )
		{
			ZoneScoped;
			const VersionKey key { id, version };
			std::lock_guard guard { map_mtx };
			if ( auto itter = map.find( key ); itter != map.end() )
			{
				const auto& [ p_key, shared ] = *itter;
				if ( shared.unique() ) map.erase( itter );
			}
		}

	} // namespace internal

	Version::Version( const RecordID id, const QString& name ) :
	  m_parent_id( id ),
	  m_version( name ),
	  data_ptr( internal::getPtr( id, name ) )
	{}

	Version::~Version()
	{
		internal::releasePtr( m_parent_id, m_version );
	}

	VersionData::VersionData( const RecordID record_id, const QString& name ) :
	  m_parent_id( record_id ),
	  m_version( name )
	{
		RapidTransaction()
				<< "SELECT game_path, exec_path, last_played, version_playtime, folder_size, date_added FROM versions WHERE record_id = ? AND version = ?"
				<< record_id << name
			>> [ this ](
				   std::filesystem::path game_path,
				   std::filesystem::path exec_path,
				   const uint64_t last_played,
				   const uint64_t total_playtime,
				   const uint64_t folder_size,
				   const uint64_t date_added ) noexcept
		{
			m_game_path = std::move( game_path );
			m_exec_path = std::move( exec_path );
			m_last_played = last_played;
			m_total_playtime = total_playtime;
			m_folder_size = folder_size;
			m_date_added = date_added;
		};
	}
} // namespace atlas::records