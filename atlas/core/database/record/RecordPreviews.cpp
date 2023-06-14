//
// Created by kj16609 on 5/24/23.
//

#include "RecordPreviews.hpp"

#include "RecordData.hpp"
#include "core/config.hpp"
#include "core/imageManager.hpp"

const std::vector< std::filesystem::path > RecordPreviews::getPreviewPaths( Transaction transaction ) const
try
{
	ZoneScoped;
	std::vector< std::filesystem::path > previews;
	const auto root_images { config::paths::images::getPath() };
	transaction << "SELECT path FROM previews WHERE record_id = ? ORDER BY position ASC" << m_record.getID() >>
		[ & ]( const std::string str ) { previews.emplace_back( root_images / str ); };

	return previews;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordPreviews::getPreviewPaths(): {} [{},{}]", m_record.getID(), e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordPreviews::getPreviewPaths(): {}", m_record.getID(), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordPreviews::getPreviewPaths(): unknown exception", m_record.getID() );
	return {};
}

std::vector< QPixmap > RecordPreviews::getPreviews( Transaction transaction ) const
try
{
	ZoneScoped;
	std::vector< QPixmap > images;

	for ( const auto& link : getPreviewPaths( transaction ) )
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
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::
		error( "{}->RecordPreviews::getPreviews(): {} [{},{}]", m_record.getID(), e.what(), e.get_code(), e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordPreviews::getPreviews(): {}", m_record.getID(), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordPreviews::getPreviews(): unknown exception", m_record.getID() );
	return {};
}

void RecordPreviews::addPreview( const std::filesystem::path& path, Transaction transaction )
try
{
	ZoneScoped;
	//Move preview to image folder
	const auto root_images { config::paths::images::getPath() };
	const std::filesystem::path new_path { imageManager::importImage( path ) };

	transaction << "INSERT INTO previews (record_id, path, position) VALUES (?, ?, ?)" << m_record.getID()
				<< std::filesystem::relative( new_path, root_images ).string() << 256;

	assert( std::filesystem::exists( new_path ) );
}
catch ( sqlite::errors::constraint_unique& e )
{
	//Eat unique constraint since it just means the preview was already added
	return;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordPreviews::addPreview({}): {} [{},{}]",
		m_record.getID(),
		path.string(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordPreviews::addPreview({}): {}", m_record.getID(), path.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordPreviews::addPreview({}): unknown exception", m_record.getID(), path.string() );
}

void RecordPreviews::reorderPreviews( const std::vector< std::filesystem::path >& paths, Transaction transaction )
try
{
	ZoneScoped;
	const auto active_previews { getPreviewPaths( transaction ) };

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
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordPreviews::reorderPreviews(): {} [{},{}]", m_record.getID(), e.what(), e.get_code(), e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordPreviews::reorderPreviews(): {}", m_record.getID(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordPreviews::reorderPreviews(): unknown exception", m_record.getID() );
}

void RecordPreviews::removePreview( const std::filesystem::path& preview, Transaction trans )
try
{
	ZoneScoped;
	const auto image_root { config::paths::images::getPath() };
	trans << "DELETE FROM previews WHERE record_id = ? AND path = ?" << m_record.getID()
		  << std::filesystem::relative( preview, image_root ).string();
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordPreviews::removePreview({}): {} [{},{}]",
		m_record.getID(),
		preview.string(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordPreviews::removePreview({}): {}", m_record.getID(), preview.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordPreviews::removePreview({}): unknown exception", m_record.getID(), preview.string() );
}
