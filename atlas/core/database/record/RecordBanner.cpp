//
// Created by kj16609 on 5/24/23.
//

#include "RecordBanner.hpp"

#include <QPixmapCache>

#include "RecordData.hpp"
#include "core/imageManager.hpp"

const std::filesystem::path RecordBanner::getBannerPath( const BannerType type ) const
try
{
	ZoneScoped;

	//ID 1 will always be the test record.
	if ( m_record.getID() == 1 ) return { ":/images/assets/Grid_Capsule_Default.webp" };

	std::string banner_path;
	RapidTransaction transaction;
	transaction << "SELECT path FROM banners WHERE record_id = ? AND type = ? limit 1" << m_record.getID()
				<< static_cast< int >( type )
		>> banner_path;

	return { config::paths::images::getPath() / banner_path };
}
catch ( const NoRows& e )
{
	//We didn't get a path. Do not return anything.

	/*std::string banner_path { "" };
	transaction << "SELECT path FROM banners WHERE record_id = ? ORDER BY type DESC limit 1" << m_record.getID() >>
		[ & ]( const std::string str ) { banner_path = str; };
		return config::paths::images::getPath() / banner_path;*/
	return {};
}

QPixmap RecordBanner::getBanner( const BannerType type ) const
{
	ZoneScoped;
	const auto path { getBannerPath( type ) };

	if ( path.empty() )
		return {};
	else
		return QPixmap { QString::fromStdString( path.string() ) };
}

QPixmap RecordBanner::
	getBanner( const int width, const int height, const SCALE_TYPE aspect_ratio_mode, const BannerType type ) const
{
	ZoneScopedN( "getBannerResized" );
	const auto key { QString::fromStdString( getBannerPath( type ).filename().string() ) + QString::number( width )
		             + "x" + QString::number( height )
		             + QString::number( static_cast< unsigned int >( aspect_ratio_mode ) ) };

	//spdlog::info( key );

	QPixmap banner;
	if ( QPixmapCache::find( key, &banner ) )
		return banner;
	else
	{
		banner = getBanner( type );
		if ( banner.isNull() )
		{
			spdlog::warn(
				"Failed to get image for banner in record: {}, title: {}",
				m_record.getID(),
				m_record.get< RecordColumns::Title >().toStdString() );
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

void RecordBanner::setBanner( const std::filesystem::path& path, const BannerType type )
{
	ZoneScoped;
	spdlog::debug( "Setting banner to {} for record_id {}", path, m_record.getID() );

	//Move banner to image folder
	const std::filesystem::path new_path { imageManager::importImage( path ) };
	const auto image_root { config::paths::images::getPath() };
	const auto file { std::filesystem::relative( new_path, image_root ).string() };

	//Check if it exists
	RapidTransaction transaction;
	if ( hasBanner( type, file ))
	{
		transaction << "UPDATE banners SET path = ? , type = ? WHERE record_id=?"  << file
					<< static_cast< int >( type )<< + m_record.getID();
	}
	else
	{
		transaction << "INSERT INTO banners (record_id, path, type) VALUES (?, ?, ?)" << m_record.getID()
					<< file << static_cast< int >( type );
	}
}

/*
catch ( sqlite::errors::constraint_unique& e )
{
	//In this case we want to just eat the unique constraint. Since it just means the banner was set to the same thing as before.
	return;
}*/

QPixmap RecordBanner::getBanner( const QSize size, const SCALE_TYPE aspect_ratio_mode, const BannerType type ) const
{
	ZoneScoped;
	return getBanner( size.width(), size.height(), aspect_ratio_mode, type );
}

bool RecordBanner::hasBanner( const BannerType type, const std::string file ) const
{
	ZoneScoped;
	int count { 0 };
	RapidTransaction trans;
	trans << "SELECT COUNT(*) FROM banners WHERE record_id = ? AND path = ? AND type = ?" << m_record.getID()
					<< file << static_cast< int >( type ) 
					>> count;
	
	return static_cast<bool>(count);
}
