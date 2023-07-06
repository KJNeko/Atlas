//
// Created by kj16609 on 5/24/23.
//

#include "RecordPreviews.hpp"

#include "RecordData.hpp"
#include "core/config.hpp"
#include "core/imageManager.hpp"

const std::vector< std::filesystem::path > RecordPreviews::getPreviewPaths() const
{
	ZoneScoped;
	RapidTransaction transaction;
	std::vector< std::filesystem::path > previews;
	const auto root_images { config::paths::images::getPath() };
	transaction << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_record.getID() >>
		[ & ]( const std::string str ) { previews.emplace_back( root_images / str ); };

	return previews;
}

std::vector< QPixmap > RecordPreviews::getPreviews() const
{
	ZoneScoped;
	std::vector< QPixmap > images;

	for ( const auto& link : getPreviewPaths() )
	{
		if ( link.empty() || !std::filesystem::exists( link ) )
		{
			spdlog::warn( "Preview image does not exist: {}", link );
			continue;
		}

		QPixmap image;
		if ( image.load( QString::fromStdString( link.string() ) ) && !image.isNull() )
			images.push_back( image );
		else
			spdlog::warn( "Failed to load preview image: {:ce}", link );
	}

	return images;
}

void RecordPreviews::addPreview( const std::filesystem::path& path )
{
	ZoneScoped;
	RapidTransaction transaction;
	//Move preview to image folder
	const auto root_images { config::paths::images::getPath() };
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	spdlog::debug( new_path );
	//assert( std::filesystem::exists( new_path ) );

	transaction << "INSERT INTO previews (record_id, path, position) VALUES (?, ?, ?)" << m_record.getID()
				<< std::filesystem::relative( new_path, root_images ).string() << 256;
}

/*
catch ( sqlite::errors::constraint_unique& e )
{
	//Eat unique constraint since it just means the preview was already added
	return;
}*/

void RecordPreviews::reorderPreviews( const std::vector< std::filesystem::path >& paths )
{
	ZoneScoped;
	const auto active_previews { getPreviewPaths() };

	RapidTransaction transaction;
	//Set all previews to max order
	constexpr std::size_t max_order { 256 };
	// max_order is just used to prevent strange previews that somehow were not in this list from being set to 0
	transaction << "UPDATE previews SET position = ? WHERE record_id = ?" << max_order << m_record.getID();

	//Update all previews
	for ( std::size_t i = 0; i < paths.size(); ++i )
	{
		const auto& path { paths[ i ] };
		if ( std::find( active_previews.begin(), active_previews.end(), path ) != active_previews.end() )
		{
			//Update it
			transaction << "UPDATE previews SET position = ? WHERE record_id = ? AND path = ?" << i << m_record.getID()
						<< path.string();
		}
		else
		{
			//Path is not something that has been added?
			spdlog::warn( "Invalid path found in preview reorder list. Ignoring. Path was {:ce}", path );
			continue;
		}
	}
}

void RecordPreviews::removePreview( const std::filesystem::path& preview )
{
	ZoneScoped;
	RapidTransaction trans;
	const auto image_root { config::paths::images::getPath() };
	trans << "DELETE FROM previews WHERE record_id = ? AND path = ?" << m_record.getID()
		  << std::filesystem::relative( preview, image_root ).string();
}
