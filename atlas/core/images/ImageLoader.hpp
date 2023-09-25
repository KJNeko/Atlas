//
// Created by kj16609 on 9/23/23.
//

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

		QPersistentModelIndex m_index;
		QFuture< QPixmap > m_future;
		QFutureWatcher< QPixmap > watcher {};

	  public:

		ImageLoader( QPersistentModelIndex index, const QFuture< QPixmap > future );

		void triggerReady();

		~ImageLoader() { m_future.cancel(); }

	  signals:
		void imageReady( QPersistentModelIndex index );
	};

} // namespace atlas::images

#endif //ATLASGAMEMANAGER_IMAGELOADER_HPP
