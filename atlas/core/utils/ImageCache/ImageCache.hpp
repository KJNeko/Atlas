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

#include "core/utils/literals/size_literals.hpp"

namespace atlas::cache
{
	using namespace atlas::literals::size_literals;

	using Clock = std::chrono::steady_clock;

	class ImageCache
	{
		using KeyT = std::uint64_t;

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
		std::multimap< KeyT, PixmapItem > cache {};
		std::uint64_t m_max_size;
		std::uint64_t current_size { 0 };

	  public:

		explicit ImageCache( std::uint64_t max_size = 256_MiB ) : m_max_size( max_size ) {}

		void setMax( std::uint64_t max_size ) { this->m_max_size = max_size; }

		//! Prunes the cache. Removing the lowest scores first. Determined via ImageCache::score()
		void prune();

		//! Inserts a pixmap into the cache
		void insert( const KeyT key, const QPixmap& pixmap );

		std::optional< QPixmap > find( KeyT key );
	};

} // namespace atlas::cache

#endif //ATLASGAMEMANAGER_IMAGECACHE_HPP
