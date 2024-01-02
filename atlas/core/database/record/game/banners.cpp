//
// Created by kj16609 on 9/21/23.
//

#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"
#include "core/images/images.hpp"
#include "core/images/import.hpp"
#include "core/images/loader.hpp"
#include "core/utils/ImageCache/ImageCache.hpp"

namespace atlas::records
{
	bool Game::hasBanner( const BannerType type ) const
	{
		return !bannerPath( type ).empty();
	}

	void Game::setBanner( std::filesystem::path path, const BannerType type )
	{
		std::lock_guard guard { this->ptr->m_mtx };

		if ( path.empty() )
		{
			RapidTransaction() << "DELETE FROM banners WHERE record_id = ? AND type = ?" << m_id
							   << static_cast< uint8_t >( type );
			this->ptr->m_banner_paths[ static_cast< std::uint64_t >( type ) ].clear();
			return;
		}

		if ( std::filesystem::relative( path, config::paths::images::getPath() ) == "" )
		{
			path = images::async::importImage( path, m_id ).result();
			if ( !std::filesystem::exists( path ) )
				throw RecordException(
					format_ns::format( "Failed to set banner. importImage returned a invalid path: {}!", path )
						.c_str() );
		}
		else if ( !std::filesystem::exists( path ) )
			throw RecordException( format_ns::format( "Invalid path {} given to setBanner.", path ).c_str() );

		if ( !hasBanner( type ) )
		{
			//If path isn't here then we insert it instead
			RapidTransaction() << "INSERT INTO banners (record_id, path, type) VALUES (?,?,?)" << m_id << path
							   << static_cast< uint8_t >( type );
		}
		else
		{
			RapidTransaction() << "UPDATE banners SET path = ? WHERE record_id = ? AND type = ?" << path << m_id
							   << static_cast< uint8_t >( type );
		}

		this->ptr->m_banner_paths[ static_cast< std::uint64_t >( type ) ] = std::move( path );

		emit dataChanged();
	}

	const std::filesystem::path Game::bannerPath( const BannerType type ) const
	{
		std::lock_guard guard { this->ptr->m_mtx };
		return this->ptr->m_banner_paths[ static_cast< unsigned long >( type ) ];
	}

} // namespace atlas::records
