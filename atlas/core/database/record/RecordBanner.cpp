//
// Created by kj16609 on 5/24/23.
//

#include "RecordBanner.hpp"

#include <QPixmapCache>

#include "RecordData.hpp"
#include "core/imageManager.hpp"

const std::filesystem::path RecordBanner::getBannerPath( const BannerType type, Transaction transaction ) const
try
{
	ZoneScoped;
	std::string banner_path;
	transaction << "SELECT path FROM banners WHERE record_id = ? AND type = ? limit 1" << m_record.getID()
				<< static_cast< int >( type )
		>> banner_path;

	return { config::paths::images::getPath() / banner_path };
}
catch ( const sqlite::exceptions::no_rows& e )
{
	//We didn't get a path. Try an alternative
	std::string banner_path { "" };
	transaction << "SELECT path FROM banners WHERE record_id = ? ORDER BY type DESC limit 1" << m_record.getID() >>
		[ & ]( const std::string str ) { banner_path = str; };

	if ( banner_path.empty() )
		return banner_path;
	else
		return config::paths::images::getPath() / banner_path;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordBanner::getBannerPath({}): {} [{},{}]",
		m_record.getID(),
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordBanner::getBannerPath({}): {}", m_record.getID(), static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::
		error( "{}->RecordBanner::getBannerPath({}): unknown exception", m_record.getID(), static_cast< int >( type ) );
	return {};
}

QPixmap RecordBanner::getBanner( const BannerType type, Transaction transaction ) const
try
{
	ZoneScoped;
	const auto path { getBannerPath( type, transaction ) };

	if ( path.empty() )
		return {};
	else
		return QPixmap { QString::fromStdString( path.string() ) };
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordBanner::getBanner({}): {} [{},{}]",
		m_record.getID(),
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordBanner::getBanner({}): {}", m_record.getID(), static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordBanner::getBanner({}): unknown exception", m_record.getID(), static_cast< int >( type ) );
	return {};
}

QPixmap RecordBanner::getBanner(
	const int width,
	const int height,
	const SCALE_TYPE aspect_ratio_mode,
	const BannerType type,
	Transaction transaction ) const
try
{
	ZoneScoped;
	const auto key { QString::fromStdString( getBannerPath( type, transaction ).filename().string() )
		             + QString::number( width ) + "x" + QString::number( height )
		             + QString::number( static_cast< unsigned int >( aspect_ratio_mode ) ) };

	//spdlog::info( key );

	QPixmap banner;
	if ( QPixmapCache::find( key, &banner ) )
		return banner;
	else
	{
		banner = getBanner( type, transaction );
		if ( banner.isNull() )
		{
			spdlog::warn(
				"Failed to get image for banner in record: {}, title: {}",
				m_record.getID(),
				m_record.title.get( transaction ).toStdString() );
			return {};
		}
		else
		{
			auto banner_scaled {
				banner.scaled( width, height, Qt::AspectRatioMode( aspect_ratio_mode ), Qt::SmoothTransformation )

			};
			if ( aspect_ratio_mode == KEEP_ASPECT_RATIO_BY_EXPANDING )
			{
				//Rmove image outside of boundry and center image
				const int new_x { width == banner_scaled.width() ? 0 : ( banner_scaled.width() - width ) / 2 };
				const int new_y { height == banner_scaled.height() ? 0 : ( banner_scaled.height() - height ) / 2 };
				banner_scaled = banner_scaled.copy( new_x, new_y, width, height );
			}

			if ( !QPixmapCache::insert( key, banner_scaled ) )
				spdlog::warn( "failed to insert banner into cache with key: {}", key );
			return banner_scaled;
		}
	}
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordBanner::getBanner({}): {} [{},{}]",
		m_record.getID(),
		static_cast< int >( type ),
		e.what(),
		e.get_code(),
		e.get_sql() );
	return {};
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordBanner::getBanner({}): {}", m_record.getID(), static_cast< int >( type ), e.what() );
	return {};
}
catch ( ... )
{
	spdlog::error( "{}->RecordBanner::getBanner({}): unknown exception", m_record.getID(), static_cast< int >( type ) );
	return {};
}

void RecordBanner::setBanner( const std::filesystem::path& path, const BannerType type, Transaction transaction )
try
{
	ZoneScoped;
	spdlog::debug( "Setting banner to {} for record_id {}", path, m_record.getID() );

	//Move banner to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };
	const auto image_root { config::paths::images::getPath() };

	//Check if it exists
	if ( hasBanner( type, transaction ) )
	{
		transaction << "UPDATE banners SET record_id = ? AND path = ? AND type = ?" << m_record.getID()
					<< std::filesystem::relative( new_path, image_root ).string() << static_cast< int >( type );
	}
	else
	{
		transaction << "INSERT INTO banners (record_id, path, type) VALUES (?, ?, ?)" << m_record.getID()
					<< std::filesystem::relative( new_path, image_root ).string() << static_cast< int >( type );
	}
}
catch ( sqlite::errors::constraint_unique& e )
{
	//In this case we want to just eat the unique constraint. Since it just means the banner was set to the same thing as before.
	return;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordBanner::setBanner({}): {} [{},{}]",
		m_record.getID(),
		path.string(),
		e.what(),
		e.get_code(),
		e.get_sql() );
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordBanner::setBanner({}): {}", m_record.getID(), path.string(), e.what() );
}
catch ( ... )
{
	spdlog::error( "{}->RecordBanner::setBanner({}): unknown exception", m_record.getID(), path.string() );
}

QPixmap RecordBanner::
	getBanner( const QSize size, const SCALE_TYPE aspect_ratio_mode, const BannerType type, Transaction transaction )
		const
{
	ZoneScoped;
	return getBanner( size.width(), size.height(), aspect_ratio_mode, type, transaction );
}

bool RecordBanner::hasBanner( const BannerType type, Transaction trans ) const
try
{
	ZoneScoped;
	int count { 0 };
	trans << "SELECT COUNT(*) FROM banners WHERE record_id = ? AND type = ?" << m_record.getID()
		  << static_cast< int >( type )
		>> count;
	return count;
}
catch ( const sqlite::sqlite_exception& e )
{
	spdlog::error(
		"{}->RecordData::hasBanner({}): {} [{},{}]", m_record.getID(), type, e.what(), e.get_code(), e.get_sql() );
	return false;
}
catch ( const std::exception& e )
{
	spdlog::error( "{}->RecordData::hasBanner({}): {}", m_record.getID(), type, e.what() );
	return false;
}
catch ( ... )
{
	spdlog::error( "{}->RecordData::hasBanner({}): unknown exception", m_record.getID(), type );
	return false;
}
