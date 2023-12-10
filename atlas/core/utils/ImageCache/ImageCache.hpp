//
// Created by kj16609 on 9/21/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMAGECACHE_HPP
#define ATLASGAMEMANAGER_IMAGECACHE_HPP

#include <QPixmap>

#include <chrono>
#include <map>
#include <mutex>

namespace atlas::cache
{
	using Clock = std::chrono::steady_clock;

	class ImageCache
	{
		struct PixmapItem
		{
			std::chrono::time_point< std::chrono::steady_clock > last_accessed { Clock::now() };
			QPixmap pixmap;

			PixmapItem( QPixmap map ) : pixmap( map ) {}

			void operator()() { last_accessed = Clock::now(); }

			std::uint64_t size() const;

			std::uint64_t score( const std::chrono::time_point< std::chrono::steady_clock >& current_time ) const;

			bool operator<( const PixmapItem& other ) const { return size() < other.size(); }
		};

		std::recursive_mutex mtx {};
		std::multimap< std::string, PixmapItem > cache {};
		std::uint64_t max_size { 1024 * 1024 * 128 }; // 128 MB
		std::uint64_t current_size { 0 };

	  public:

		//! Prunes the cache. Removing the lowest scores first. Determined via ImageCache::score()
		void prune();

		//! Inserts a pixmap into the cache
		void insert( const std::string& key, const QPixmap& pixmap );

		std::optional< QPixmap > find( std::string key );
	};

} // namespace atlas::cache

#endif //ATLASGAMEMANAGER_IMAGECACHE_HPP
