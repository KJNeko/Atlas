//
// Created by kj16609 on 9/23/23.
//

#include "ImageLoader.hpp"

#include <moc_ImageLoader.cpp>

#include <QtConcurrent>

#include "core/utils/ImageCache/ImageCache.hpp"
#include "core/utils/QImageBlur.hpp"
#include "core/utils/literals/size_literals.hpp"
#include "core/utils/threading/pools.hpp"
#include "loader.hpp"

namespace atlas::images
{
	using namespace atlas::literals::size_literals;
	inline static cache::ImageCache loader_cache { 256_MiB };

	std::uint64_t appendHash( std::uint64_t& seed, const std::uint64_t hash )
	{
		seed ^= hash + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
		return seed;
	}

	ImageLoader::ImageLoader( const std::filesystem::path& path ) : m_path( path )
	{
		m_future_chain.push( QtConcurrent::
		                         run( &atlas::threading::globalPools().image_loaders,
		                              &::atlas::images::loadPixmap,
		                              path ) );

		operation_hash = appendHash( operation_hash, std::hash< std::filesystem::path >()( path ) );
	}

	ImageLoader::~ImageLoader()
	{
		if ( m_future_chain.empty() ) return;

		if ( m_future_chain.back().isFinished() ) return;

		if ( !m_future_chain.front().isCanceled() )
		{
			atlas::logging::warn( "Image loader discarded before cancel or finish! Is this intentional?" );
			cancel();
		}
	}

	[[nodiscard]] std::unique_ptr< ImageLoader > ImageLoader::loadPixmap( const std::filesystem::path& path )
	{
		return std::unique_ptr< ImageLoader >( new ImageLoader( path ) );
	}

	ImageLoader* ImageLoader::scaleTo( const QSize size, const SCALE_TYPE scale_type, const Alignment align_type )
	{
		auto future = m_future_chain.back().then(
			&atlas::threading::globalPools().image_loaders,
			[ size, scale_type, align_type ]( const QPixmap& pixmap ) -> QPixmap
			{ return scalePixmap( pixmap, size, scale_type, align_type ); } );

		m_future_chain.push( std::move( future ) );

		operation_hash = appendHash( operation_hash, std::hash< int >()( ( size.height() << 16 ) + ( size.width() ) ) );
		operation_hash = appendHash( operation_hash, std::hash< SCALE_TYPE >()( scale_type ) );
		operation_hash = appendHash( operation_hash, std::hash< Alignment >()( align_type ) );

		return this;
	}

	ImageLoader* ImageLoader::cropTo( const QRect rect )
	{
		auto future = m_future_chain.back().then(
			&atlas::threading::globalPools().image_loaders,
			[ rect ]( const QPixmap& pixmap ) -> QPixmap { return pixmap.copy( rect ); } );

		m_future_chain.push( std::move( future ) );

		operation_hash = appendHash( operation_hash, std::hash< int >()( rect.bottom() ) );
		operation_hash = appendHash( operation_hash, std::hash< int >()( rect.top() ) );
		operation_hash = appendHash( operation_hash, std::hash< int >()( rect.left() ) );
		operation_hash = appendHash( operation_hash, std::hash< int >()( rect.right() ) );

		return this;
	}

	QFuture< QPixmap > ImageLoader::future()
	{
		m_watcher.setFuture( m_future_chain.back() );

		connect(
			&m_watcher,
			&QFutureWatcher< QPixmap >::finished,
			this,
			[ this ]() { emit finished( m_watcher.result() ); } );

		//Find image in cache
		auto cache_result = loader_cache.find( operation_hash );
		if ( cache_result.has_value() )
		{
			if ( cache_result.value().isNull() ) //Invalidate cache
			{
				atlas::logging::error( "Image loader found invalid cache entry!" );
				return m_future_chain.back();
			}

			return QtFuture::makeReadyFuture( cache_result.value() );
		}
		else
		{
			addToCache();
		}

		return m_future_chain.back();
	}

	ImageLoader* ImageLoader::
		blurToSize( const QSize size, const int feather_radius, const int blur_radius, const BLUR_TYPE blur_type )
	{
		auto future = m_future_chain.back().then(
			&atlas::threading::globalPools().image_loaders,
			[ size, feather_radius, blur_radius, blur_type ]( const QPixmap& pixmap ) -> QPixmap
			{ return atlas::images::blurToSize( pixmap, size, feather_radius, blur_radius, blur_type ); } );

		m_future_chain.push( std::move( future ) );

		return this;
	}

	void ImageLoader::addToCache()
	{
		m_future_chain.push( m_future_chain.back().then(
			&atlas::threading::globalPools().image_loaders,
			[ this ]( const QPixmap& pixmap ) -> QPixmap
			{
				if ( !pixmap.isNull() ) loader_cache.insert( operation_hash, pixmap );
				return pixmap;
			} ) );
	}

} // namespace atlas::images