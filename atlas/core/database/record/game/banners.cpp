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
	inline static atlas::cache::ImageCache banner_cache;

	bool Game::hasBanner( const BannerType type ) const
	{
		return !bannerPath( type ).empty();
	}

	void Game::setBanner( std::filesystem::path path, const BannerType type )
	{
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

	const std::filesystem::path& Game::bannerPath( const BannerType type ) const
	{
		return this->ptr->m_banner_paths[ static_cast< unsigned long >( type ) ];
	}

	QFuture< QPixmap > Game::requestBanner( const BannerType type ) const
	{
		ZoneScoped;

		const auto& path { bannerPath( type ) };
		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( QPixmap() ); // Path is not valid so we return an empty pixmap.

		const std::string cache_key { format_ns::format( "{}", path ) };

		if ( std::optional< QPixmap > opt = banner_cache.find( cache_key ); opt.has_value() )
			return QtFuture::makeReadyFuture( std::move( opt.value() ) );
		else
			return atlas::images::async::loadPixmap( path );
	}

	QFuture< QPixmap > Game::requestBanner( const QSize size, const SCALE_TYPE scale_type, const BannerType type )
	{
		const auto& path { bannerPath( type ) };

		if ( !hasBanner( type ) ) return QtFuture::makeReadyFuture( QPixmap() );

		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( QPixmap() );
		/*	return QtFuture::makeExceptionalFuture< QPixmap >( std::make_exception_ptr( ImageLoadError(
				format_ns::format( "No banner path for game id: {} with type: {}", m_id, static_cast< int >( type ) )
					.c_str() ) ) );*/
		//TODO: Rip out everywhere this is needed and check if we even have the banner via `hasBanner`

		const std::string cache_key {
			format_ns::format( "{}x{}:{}:{}", size.width(), size.height(), static_cast< int >( scale_type ), path )
		};

		if ( std::optional< QPixmap > opt = banner_cache.find( cache_key ); opt.has_value() )
			return QtFuture::makeReadyFuture( std::move( opt.value() ) );
		else
			return atlas::images::async::loadScaledPixmap( size, scale_type, path );
	}

	QPixmap Game::requestThumbnail( const BannerType type )
	{
		const auto& path { bannerPath( type ) };

		return atlas::images::thumbnail( path );
	}

	//! Simple passthrough to same function but with combined size via QSize instead of seperate ints
	QFuture< QPixmap > Game::
		requestBanner( const int width, const int height, const SCALE_TYPE scale_type, const BannerType type )
	{
		return requestBanner( { width, height }, scale_type, type );
	}

	QFuture< QPixmap > Game::requestPreview( const std::uint64_t index ) const
	{
		const auto& previews { this->ptr->m_preview_paths };
		const auto& path { previews.at( index ) };

		return atlas::images::async::loadPixmap( path );
	}

} // namespace atlas::records
