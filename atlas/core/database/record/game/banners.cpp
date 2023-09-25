//
// Created by kj16609 on 9/21/23.
//

#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"
#include "core/images/images.hpp"
#include "core/images/import.hpp"
#include "core/images/loader.hpp"
#include "core/images/thumbnails.hpp"
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

	QFuture< QPixmap > Game::requestBanner( const BannerType type, const bool use_thumbnail ) const
	{
		std::lock_guard guard { this->ptr->m_mtx };

		const auto& path { bannerPath( type ) };
		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( QPixmap() ); // Path is not valid so we return an empty pixmap.
		else if ( use_thumbnail )
			return atlas::images::async::thumbnail( path );
		else
			return atlas::images::async::loadPixmap( path );
	}

	QFuture< QPixmap > Game::
		requestBanner( const QSize size, const SCALE_TYPE scale_type, const BannerType type, const bool use_thumbnail )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& path { bannerPath( type ) };

		if ( !hasBanner( type ) ) return QtFuture::makeReadyFuture( QPixmap() );

		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( QPixmap() );
		else if ( use_thumbnail )
			return atlas::images::async::scaledThumbnail( size, scale_type, path );
		else
			return atlas::images::async::loadScaledPixmap( size, scale_type, path );
	}

	QPixmap Game::requestThumbnail( const BannerType type )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& path { bannerPath( type ) };

		return atlas::images::thumbnail( path );
	}

	//! Simple passthrough to same function but with combined size via QSize instead of seperate ints
	QFuture< QPixmap > Game::requestBanner(
		const int width,
		const int height,
		const SCALE_TYPE scale_type,
		const BannerType type,
		const bool use_thumbnail )
	{
		return requestBanner( { width, height }, scale_type, type, use_thumbnail );
	}

	QFuture< QPixmap > Game::requestPreview( const std::uint64_t index, const bool use_thumbnail ) const
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& previews { this->ptr->m_preview_paths };
		const auto& path { previews.at( index ) };

		if ( use_thumbnail )
			return atlas::images::async::thumbnail( path );
		else
			return atlas::images::async::loadPixmap( path );
	}

} // namespace atlas::records
