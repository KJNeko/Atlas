//
// Created by kj16609 on 9/21/23.
//

#include <vector>

#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"
#include "core/images/images.hpp"
#include "core/images/import.hpp"
#include "core/images/loader.hpp"
#include "core/images/thumbnails.hpp"
#include "core/utils/ImageCache/ImageCache.hpp"

inline static atlas::cache::ImageCache preview_cache;

namespace atlas::records
{
	void Game::reorderPreviews( std::vector< std::filesystem::path > paths )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		//If the path doesn't exist then it just gets added as if it did before
		RapidTransaction() << "DELETE FROM previews WHERE record_id = ?" << m_id;

		for ( std::uint64_t i = 0; i < paths.size(); ++i )
		{
			RapidTransaction() << "INSERT INTO previews (record_id, path, position) VALUES (?,?,?)" << m_id
							   << paths[ i ] << i;
		}

		emit dataChanged();
	}

	void Game::addPreview( std::filesystem::path path, std::uint64_t index )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		// If relative returns an empty string then we can safely assume that the path is not inside of the image folder
		if ( !std::filesystem::exists( path ) )
			throw RecordException( format_ns::format( "Invalid path {} given to addPreview.", path ).c_str() );

		if ( !path.string().starts_with( config::paths::images::getPath().string() ) )
		{
			try
			{
				path = atlas::images::async::importImage( path, m_id ).result();
			}
			catch ( QUnhandledException& e )
			{
				std::rethrow_exception( e.exception() );
			}
		}

		//Get the highest position
		if ( index == 0 )
		{
			RapidTransaction() << "SELECT position FROM previews WHERE record_id = ? ORDER BY position DESC LIMIT 1"
							   << m_id
				>> index;
		}

		RapidTransaction() << "INSERT INTO previews (record_id, path, position) VALUES (?,?,?)" << m_id << path
						   << index;
		auto& previews { this->ptr->m_preview_paths };
		previews.clear();

		RapidTransaction() << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_id >>
			[ &previews ]( std::filesystem::path preview_path ) { previews.emplace_back( std::move( preview_path ) ); };

		emit dataChanged();
	}

	QFuture< QPixmap > Game::preview( const std::uint64_t index, const bool use_thumbnail )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& previews { ptr->m_preview_paths };
		if ( static_cast< std::uint64_t >( index ) > previews.size() )
			throw AtlasException( format_ns::format(
									  "Attempted to get preview for index higher then max size: index = {}, size = {}",
									  index,
									  previews.size() )
			                          .c_str() );

		const auto& path { previews.at( index ) };

		if ( use_thumbnail )
			return atlas::images::async::thumbnail( path );
		else
			return atlas::images::async::loadPixmap( path );
	}

	QFuture< QPixmap > Game::scaledPreview(
		const QSize size, const SCALE_TYPE scale_type, const std::uint64_t index, const bool use_thumbnail )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		const auto& previews { ptr->m_preview_paths };
		if ( static_cast< std::uint64_t >( index ) > previews.size() )
			throw AtlasException( format_ns::format(
									  "Attempted to get preview for index higher then max size: index = {}, size = {}",
									  index,
									  previews.size() )
			                          .c_str() );

		const auto& path { previews.at( index ) };
		if ( use_thumbnail )
			return atlas::images::async::thumbnail( path );
		else
			return atlas::images::async::loadScaledPixmap( size, scale_type, path );
	}

	void Game::removePreview( const std::uint64_t index )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		auto& previews { ptr->m_preview_paths };
		RapidTransaction() << "DELETE FROM previews WHERE record_id = ? AND position = ?" << m_id << index;

		auto itter { previews.begin() };
		std::advance( itter, index );
		previews.erase( itter );

		emit dataChanged();
	}

	void Game::removePreview( const std::filesystem::path path )
	{
		std::lock_guard guard { this->ptr->m_mtx };
		//Find the preview index
		const auto& previews { ptr->m_preview_paths };
		const auto index { std::find( previews.begin(), previews.end(), path ) };
		if ( index == previews.end() )
			throw std::runtime_error( "Game: Failed to find index for give preview path" );
		else
			removePreview( static_cast< uint64_t >( std::distance( previews.begin(), index ) ) );
	}
} // namespace atlas::records