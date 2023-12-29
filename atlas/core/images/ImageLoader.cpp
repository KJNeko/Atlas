//
// Created by kj16609 on 9/23/23.
//

#include "ImageLoader.hpp"

#include <moc_ImageLoader.cpp>

#include <QtConcurrent>

#include "core/utils/QImageBlur.hpp"
#include "core/utils/threading/pools.hpp"
#include "loader.hpp"

namespace atlas::images
{

	ImageLoader::ImageLoader( const std::filesystem::path& path ) : m_path( path )
	{
		m_future_chain.push( QtConcurrent::
		                         run( &atlas::threading::globalPools().image_loaders,
		                              &::atlas::images::loadPixmap,
		                              path ) );
	}

	ImageLoader::~ImageLoader()
	{
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

		return this;
	}

	ImageLoader* ImageLoader::cropTo( const QRect rect )
	{
		auto future = m_future_chain.back().then(
			&atlas::threading::globalPools().image_loaders,
			[ rect ]( const QPixmap& pixmap ) -> QPixmap { return pixmap.copy( rect ); } );

		m_future_chain.push( std::move( future ) );

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

} // namespace atlas::images