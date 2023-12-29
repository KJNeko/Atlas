//
// Created by kj16609 on 9/23/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMAGELOADER_HPP
#define ATLASGAMEMANAGER_IMAGELOADER_HPP

#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QPixmap>

#include <queue>

#include "core/config/config.hpp"

namespace atlas::images
{
	struct ImageLoader final : public QObject
	{
		Q_OBJECT

		std::filesystem::path m_path;

		std::queue< QFuture< QPixmap > > m_future_chain {};
		QFutureWatcher< QPixmap > m_watcher {};

		ImageLoader( const std::filesystem::path& path );

	  public:

		ImageLoader( const ImageLoader& other ) = delete;
		ImageLoader( ImageLoader&& other ) = delete;

		ImageLoader& operator=( const ImageLoader& other ) = delete;
		ImageLoader& operator=( ImageLoader&& other ) = delete;

		void cancel()
		{
			//Cancel the front which will cancel all futures
			m_future_chain.front().cancel();
		}

		~ImageLoader();

		[[nodiscard]] bool isFinished() { return m_future_chain.back().isFinished(); }

		[[nodiscard]] static std::unique_ptr< ImageLoader > loadPixmap( const std::filesystem::path& path );
		ImageLoader* scaleTo( const QSize size, const SCALE_TYPE scale_type, const Alignment align_type );
		ImageLoader* cropTo( const QRect rect );
		ImageLoader*
			blurToSize( const QSize size, const int feather_radius, const int blur_radius, const BLUR_TYPE blur_type );

		[[nodiscard]] QFuture< QPixmap > future();

	  signals:
		void finished( const QPixmap& pixmap );
	};

} // namespace atlas::images

#endif //ATLASGAMEMANAGER_IMAGELOADER_HPP
