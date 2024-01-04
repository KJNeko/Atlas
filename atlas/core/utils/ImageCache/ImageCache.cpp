//
// Created by kj16609 on 9/21/23.
//

#include "ImageCache.hpp"

#include "core/images/images.hpp"
#include "core/logging/logging.hpp"

namespace atlas::cache
{

	std::uint64_t ImageCache::PixmapItem::size() const
	{
		if ( pixmap.isNull() ) return 0;
		const auto result { static_cast< int64_t >( pixmap.width() ) * static_cast< int64_t >( pixmap.height() )
			                * static_cast< int64_t >( pixmap.depth() ) / 8 };

		if ( result < 0 )
			return std::numeric_limits< std::uint64_t >::max();
		else
			return static_cast< uint64_t >( result );
	}

	std::uint64_t ImageCache::PixmapItem::score( const std::chrono::time_point< std::chrono::steady_clock >&
	                                                 current_time ) const
	{
		double time_score_ratio { 1.2 };

		const std::uint64_t size_score { size() };
		const std::uint64_t time_diff {
			static_cast< uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( current_time - last_accessed )
			                             .count() )
		};

		// If size is zero then we emit a super high score to get this item removed as soon as we can. Since it's likely fucked.
		if ( size_score == 0 ) return std::numeric_limits< std::uint64_t >::max();

		// Prevent multiply by 0
		if ( time_diff == 0 ) return size_score;

		// Take the size score and multiplty it by the time diff. Making it higher the more time has ellapsed depending on the ratio.
		const auto val { static_cast< uint64_t >(
			static_cast< double >( size_score ) * ( static_cast< double >( time_diff ) * time_score_ratio ) ) };

		//If somehow. Magically. We manage to get val to zero. We just return max.
		if ( val == 0 ) return std::numeric_limits< std::uint64_t >::max();
		return val;
	}

	void ImageCache::prune()
	{
		std::lock_guard guard { mtx };
		//Timepoint at which we declare data as 'stale' and can probably safely remove it
		using namespace std::chrono_literals;
		//Time at which a image can be declared 'old' and can safely be removed if it's not been accessed in this timeframe.
		const auto stale_timepoint { Clock::now() - 10s };

		int offset { 0 };
		for ( std::uint64_t i = 0; i < cache.size(); ++i )
		{
			auto begin { cache.begin() };
			std::advance( begin, i - static_cast< uint64_t >( offset ) );

			if ( begin->second.last_accessed < stale_timepoint )
			{
				current_size -= begin->second.size();
				cache.erase( begin->first );
				++offset;
			}
		}

		if ( current_size < m_max_size ) return;

		std::vector< std::pair< KeyT, PixmapItem > > ordered { cache.begin(), cache.end() };
		const auto timepoint { Clock::now() };
		std::sort(
			ordered.begin(),
			ordered.end(),
			[ &timepoint ]( const std::pair< KeyT, PixmapItem >& first, const std::pair< KeyT, PixmapItem >& second )
			{ return first.second.score( timepoint ) > second.second.score( timepoint ); } );

		//Remove the last items from the map
		for ( const auto& itter : ordered )
		{
			const auto& [ key, item ] = itter;
			if ( current_size < m_max_size )
				break;
			else
			{
				current_size -= item.size();
				cache.erase( key );
			}
		}
	}

	void ImageCache::insert( const KeyT key, const QPixmap& pixmap )
	{
		std::lock_guard guard { mtx };

		if ( cache.contains( key ) ) return;

		if ( pixmap.isNull() )
		{
			throw ImageManagerException( format_ns::format( "Attempted to insert a null pixmap with key {}", key )
			                                 .c_str() );
		}

		PixmapItem item { std::move( pixmap ) };

		current_size += item.size();

		cache.insert( { key, std::move( item ) } );

		if ( current_size > m_max_size ) prune();
	}

	std::optional< QPixmap > ImageCache::find( KeyT key )
	{
		std::lock_guard guard { mtx };
		if ( auto itter = cache.find( key ); itter != cache.end() )
		{
			if ( itter->second.pixmap.isNull() || itter->second.pixmap.size() == QSize( 0, 0 ) )
			{
				//TODO: Invalidate cache
				return std::nullopt;
			}

			atlas::logging::debug( "Found pixmap using key {} with size {}", key, itter->second.pixmap.size() );
			itter->second();
			return itter->second.pixmap;
		}
		else
			return std::nullopt;
	}

} // namespace atlas::cache
