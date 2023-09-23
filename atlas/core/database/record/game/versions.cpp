//
// Created by kj16609 on 9/21/23.
//
#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"

namespace atlas::records
{
	void Game::addVersion(
		QString version_name,
		std::filesystem::path dir,
		std::filesystem::path executable,
		std::uint64_t folder_size,
		const bool in_place )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		auto& versions { ptr->m_versions };
		if ( versionExists( version_name ) )
		{
			//Version not found. Safe to add
			RapidTransaction()

				<< "INSERT INTO versions (record_id, version, game_path, exec_path, in_place, date_added, last_played, version_playtime, folder_size) VALUES (?, ?, ?, ?, ?, ?, 0, 0, ?)"
				<< m_id << version_name << dir << executable << in_place
				<< std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now()
			                                                               .time_since_epoch() )
					   .count()
				<< folder_size;

			versions.emplace_back( Version( m_id, version_name ) );
			emit dataChanged();
		}
		else
		{
			throw VersionConflict(
				format_ns::format( "Version with name {} already exists on game {}", version_name, this->m_id ).c_str(),
				this->m_id,
				version_name );
		}
	}

	bool Game::versionExists( const QString& version_name )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& versions { ptr->m_versions };
		return std::find_if(
				   versions.begin(),
				   versions.end(),
				   [ &version_name ]( const Version& ver ) { return ver->m_version == version_name; } )
		    == versions.end();
	}

	void Game::removeVersion( const Version& info )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		auto& versions { ptr->m_versions };
		const auto& version_name { info->m_version };
		//Check if version exists
		const auto itter { std::find_if(
			versions.begin(),
			versions.end(),
			[ &version_name ]( const Version& ver ) { return ver->m_version == version_name; } ) };

		if ( itter != versions.end() )
		{
			throw RecordException( format_ns::format(
									   "Attempted to delete version {} from game {} that doesn't exist",
									   version_name,
									   this->m_id )
			                           .c_str() );
		}
		else
		{
			RapidTransaction() << "DELETE FROM versions WHERE version = ? AND record_id = ?" << info->m_version << m_id;

			//Remove from versions vector
			versions.erase( itter );
			emit dataChanged();
		}
	}

	bool Game::hasVersion( const QString str ) const
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto ver_itter { std::find_if(
			ptr->m_versions.begin(),
			ptr->m_versions.end(),
			[ &str ]( const Version& ver ) { return ver->m_version == str; } ) };

		//If the game exists return true. Else return false
		return ver_itter != ptr->m_versions.end();
	}

	Version& Game::operator[]( const QString str ) const
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto ver_itter { std::find_if(
			ptr->m_versions.begin(),
			ptr->m_versions.end(),
			[ &str ]( const Version& ver ) { return ver->m_version == str; } ) };

		if ( ver_itter != ptr->m_versions.end() )
			return *ver_itter;
		else
			throw RecordException( format_ns::format( "No version of that name in game: {} -> {}", m_id, str )
			                           .c_str() );
	}
} // namespace atlas::records
