//
// Created by kj16609 on 9/23/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMAGELOADER_HPP
#define ATLASGAMEMANAGER_IMAGELOADER_HPP

#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QPersistentModelIndex>
#include <QPixmap>

namespace atlas::images
{
	struct ImageLoader final : public QObject
	{
		Q_OBJECT

	  public:

		QPersistentModelIndex m_index;
		QFuture< QPixmap > m_future;
		QFutureWatcher< QPixmap > watcher {};

		ImageLoader( QPersistentModelIndex index, QFuture< QPixmap > future );

		void triggerReady();

		void cancel()
		{
			watcher.cancel();
			m_future.cancel();
		}

		~ImageLoader() { cancel(); }

	  signals:
		void imageReady( QPersistentModelIndex index );
	};

} // namespace atlas::images

#endif //ATLASGAMEMANAGER_IMAGELOADER_HPP
