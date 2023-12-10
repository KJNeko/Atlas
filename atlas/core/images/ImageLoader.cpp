//
// Created by kj16609 on 9/23/23.
//

#include "ImageLoader.hpp"

#include <moc_ImageLoader.cpp>

namespace atlas::images
{
	ImageLoader::ImageLoader( QPersistentModelIndex index, QFuture< QPixmap > future ) :
	  m_index( index ),
	  m_future( std::move( future ) )
	{
		connect( &watcher, &decltype( watcher )::finished, this, &ImageLoader::triggerReady );
		watcher.setFuture( m_future );
	}

	void ImageLoader::triggerReady()
	{
		emit imageReady( m_index );
	}
} // namespace atlas::images