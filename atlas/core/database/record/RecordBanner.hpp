//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_RECORDBANNER_HPP
#define ATLASGAMEMANAGER_RECORDBANNER_HPP

class RecordData;

#include "core/config.hpp"
#include "core/database/Database.hpp"

class RecordBanner
{
	RecordData& m_record;

	RecordBanner( RecordData& parent ) : m_record( parent ) {}

	RecordBanner() = delete;
	RecordBanner( const RecordBanner& other ) = delete;
	RecordBanner( RecordBanner&& other ) = delete;

	friend class RecordData;

  public:

	const std::filesystem::path
		getBannerPath( const BannerType type, Transaction transaction = Transaction( Autocommit ) ) const;
	bool hasBanner( const BannerType type, Transaction transaction = Transaction( Autocommit ) ) const;
	QPixmap getBanner( const BannerType type, Transaction transaction = Transaction( Autocommit ) ) const;
	QPixmap getBanner(
		const int width,
		const int height,
		const SCALE_TYPE aspect_ratio_mode,
		const BannerType type,
		Transaction transaction = Transaction( Autocommit ) ) const;
	QPixmap getBanner(
		const QSize size,
		const SCALE_TYPE aspect_ratio_mode,
		const BannerType type,
		Transaction transaction = Transaction( Autocommit ) ) const;

	void setBanner( const std::filesystem::path&, const BannerType, Transaction = Transaction( Autocommit ) );
};

#endif //ATLASGAMEMANAGER_RECORDBANNER_HPP
