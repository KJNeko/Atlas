//
// Created by kj16609 on 7/15/23.
//

#include <QImageReader>
#include <QPixmapCache>
#include <QtConcurrent>

#include "Game.hpp"
#include "GameData.hpp"
#include "core/pools.hpp"

namespace atlas::records
{

	namespace internal
	{
		using Clock = std::chrono::steady_clock;

		struct PixmapItem
		{
			std::chrono::time_point< std::chrono::steady_clock > last_accessed { Clock::now() };
			QPixmap pixmap;

			PixmapItem( QPixmap map ) : pixmap( map ) {}

			void operator()() { last_accessed = Clock::now(); }

			std::uint64_t size() const
			{
				if ( pixmap.isNull() ) return 0;
				const auto result { static_cast< int64_t >( pixmap.width() ) * static_cast< int64_t >( pixmap.height() )
					                * static_cast< int64_t >( pixmap.depth() ) / 8 };

				if ( result < 0 )
					return std::numeric_limits< std::uint64_t >::max();
				else
					return static_cast< uint64_t >( result );
			}

			std::uint64_t score() const
			{
				double time_score_ratio { 1.2 };

				const std::uint64_t size_score { size() };
				const std::uint64_t time_diff { static_cast<
					uint64_t >( std::chrono::duration_cast< std::chrono::seconds >( Clock::now() - last_accessed )
					                .count() ) };

				// If size is zero then we emit a super high score to get this item removed as soon as we can. Since it's likely fucked.
				if ( size_score == 0 ) return std::numeric_limits< std::uint64_t >::max();
				if ( time_diff == 0 )
					return size_score; //Prevents multiply by 0 which for some reason caused SIGILL to be yeeted by the CPU.

				const auto val { static_cast< uint64_t >(
					static_cast< double >( size_score ) * ( static_cast< double >( time_diff ) * time_score_ratio ) ) };

				//If somehow. Magically. We manage to get val to zero. We just return max.
				if ( val == 0 ) return std::numeric_limits< std::uint64_t >::max();
				return val;
			}

			bool operator<( const PixmapItem& other ) const { return size() < other.size(); }
		};

		inline static std::recursive_mutex mtx;
		inline static std::multimap< std::string, PixmapItem > cache;
		inline static std::uint64_t max_size { 1024 * 1024 * 128 }; // 128 MB
		inline static std::uint64_t current_size { 0 };

		void prune()
		{
			ZoneScoped;
			std::lock_guard guard { mtx };
			//Timepoint at which we declare data as 'stale' and can probably safely remove it
			using namespace std::chrono_literals;
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

			if ( current_size < max_size ) return;

			std::vector< std::pair< std::string, PixmapItem > > ordered { cache.begin(), cache.end() };
			std::sort(
				ordered.begin(),
				ordered.end(),
				[]( const std::pair< std::string, PixmapItem >& first,
			        const std::pair< std::string, PixmapItem >& second )
				{ return first.second.score() > second.second.score(); } );

			//Remove the last items from the map
			for ( const auto& [ key, item ] : ordered )
			{
				if ( current_size < max_size )
					break;
				else
				{
					current_size -= item.size();
					cache.erase( key );
				}
			}
		}

		void insert( std::string key, QPixmap pixmap )
		{
			ZoneScoped;
			std::lock_guard guard { mtx };

			if ( cache.contains( key ) ) return;

			PixmapItem item { std::move( pixmap ) };

			current_size += item.size();

			cache.insert( { key, std::move( item ) } );

			if ( current_size > max_size ) prune();
		}

		std::optional< QPixmap > find( std::string key )
		{
			ZoneScoped;
			std::lock_guard guard { mtx };
			if ( auto itter = cache.find( key ); itter != cache.end() )
			{
				itter->second();
				return itter->second.pixmap;
			}
			else
				return std::nullopt;
		}
	} // namespace internal

	void loadImage( QPromise< QPixmap >& promise, const std::filesystem::path path )
	{
		ZoneScoped;
		if ( promise.isCanceled() ) return;
		QPixmap pixmap;

		if ( promise.isCanceled() ) return;
		if ( path.empty() || !std::filesystem::exists( path ) )
		{
			promise.addResult( pixmap );
		}

		if ( promise.isCanceled() ) return;
		if ( !pixmap.load( QString::fromStdString( path.string() ) ) )
		{
			atlas::logging::userwarn( fmt::format( "Qt Failed to load image {}", path ) );
			promise.addResult( pixmap );
		}
		if ( promise.isCanceled() ) return;
		if ( pixmap.size() == QSize( 0, 0 ) || pixmap.isNull() )
			spdlog::warn( "Suspected failed to load banner {}", path );
		promise.addResult( pixmap );
	}

	void scaleImage(
		QPromise< QPixmap >& promise,
		const QSize target_size,
		const SCALE_TYPE scale_type,
		const std::filesystem::path path )
	{
		ZoneScoped;
		QRect bannerRect { 0, 0, target_size.width(), target_size.height() };
		if ( promise.isCanceled() ) return;

		if ( path.empty() || !std::filesystem::exists( path ) )
		{
			promise.addResult( QPixmap() );
			return;
		}

		if ( promise.isCanceled() ) return;
		const auto key { fmt::format( "{}x{}:{}:{}", target_size.width(), target_size.height(), scale_type, path ) };

		if ( promise.isCanceled() ) return;
		if ( auto opt = internal::find( key ); opt.has_value() )
		{
			promise.addResult( opt.value() );
			return;
		}
		else
		{
			ZoneScopedN( "Read image" );
			QImageReader loader { QString::fromStdString( path.string() ) };
			const auto image_size { loader.size() };

			//Calculate the scale we want in order to fit the image properly.
			loader.setScaledSize( image_size.scaled( target_size, Qt::AspectRatioMode( scale_type ) ) );

			if ( promise.isCanceled() ) return;
			QPixmap pixmap { QPixmap::fromImageReader( &loader ) };
			if ( scale_type == SCALE_TYPE::KEEP_ASPECT_RATIO_BY_EXPANDING )
			{
				pixmap = pixmap.copy( bannerRect ); //Crop banner image. Mainly used for Fill scale option
			}
			if ( promise.isCanceled() ) return;

			if ( pixmap.isNull() )
			{
				atlas::logging::userwarn(
					fmt::format( "Qt failed to load image {} Pixmap was null after attempted loading. ", path ) );
				promise.addResult( pixmap );
			}

			if ( promise.isCanceled() ) return;

			internal::insert( key, pixmap );
			promise.addResult( std::move( pixmap ) );
		}
	}

	std::filesystem::path Game::bannerPath( const BannerType type ) const
	{
		return this->ptr->m_banner_paths[ static_cast< unsigned long >( type ) ];
	}

	QFuture< QPixmap > Game::requestBanner( const BannerType type ) const
	{
		ZoneScoped;

		QPixmap pixmap {};
		const auto path { bannerPath( type ) };
		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( pixmap );

		const auto key { fmt::format( "{}", path ) };

		if ( auto opt = internal::find( key ); opt.has_value() )
			return QtFuture::makeReadyFuture( std::move( opt.value() ) );
		else
			return QtConcurrent::run( &globalPools().image_loaders, loadImage, path );
	}

	QFuture< QPixmap > Game::requestBanner( const QSize size, const SCALE_TYPE scale_type, const BannerType type )
	{
		ZoneScopedN( "requestBannerSized" );

		const auto path { bannerPath( type ) };
		if ( path.empty() ) //Ideally we would check if the path exists too but it's too expensive do to during a paint
			return QtFuture::makeReadyFuture( QPixmap() );

		const auto key { fmt::format( "{}x{}:{}:{}", size.width(), size.height(), scale_type, path ) };

		if ( auto opt = internal::find( key ); opt.has_value() )
			return QtFuture::makeReadyFuture( std::move( opt.value() ) );
		else
			return QtConcurrent::run( &globalPools().image_loaders, scaleImage, size, scale_type, path );
	}

	QPixmap Game::requestThumbnail(const QSize size, const BannerType type){
		const auto path { bannerPath( type ) };
		spdlog::info(
			"{}",
			QString::fromStdString(
				path.parent_path().string() + "//" + path.stem().string() + "_thumb" + path.extension().string() ) );
		QPixmap pixmap { QPixmap( QString::fromStdString(
								  path.parent_path().string() + "//"
								  + path.stem().string() + "_thumb"
								  + path.extension().string() ) )
				         .scaled( size, Qt::IgnoreAspectRatio )};
		return pixmap;
	}

	//! Simple passthrough to same function but with combined size via QSize instead of seperate ints
	QFuture< QPixmap > Game::
		requestBanner( const int width, const int height, const SCALE_TYPE scale_type, const BannerType type )
	{
		return requestBanner( { width, height }, scale_type, type );
	}

	QFuture< QPixmap > Game::requestPreviewIndex( const std::uint64_t index ) const
	{
		ZoneScoped;
		const auto& previews { this->ptr->m_preview_paths };
		const auto path { previews.at( index ) };

		return QtConcurrent::run( &globalPools().image_loaders, loadImage, path );
	}
} // namespace atlas::records
