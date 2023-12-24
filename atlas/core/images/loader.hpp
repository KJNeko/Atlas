//
// Created by kj16609 on 9/22/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_LOADE_HPP
#define ATLASGAMEMANAGER_LOADE_HPP

#include <QFuture>
#include <QPixmap>

#include <filesystem>

#include "core/config/config.hpp"

namespace atlas::images
{
	[[nodiscard]] QPixmap
		scalePixmap( QPixmap img, const QSize target_size, const SCALE_TYPE scale_type, const Alignment align_type );
	[[nodiscard]] QPixmap loadScaledPixmap(
		const QSize target_size, const SCALE_TYPE type, const Alignment align_type, const std::filesystem::path& path );
	[[nodiscard]] QImage loadImage( const std::filesystem::path& path );
	[[nodiscard]] QPixmap loadPixmap( const std::filesystem::path& path );

	namespace async
	{
		[[nodiscard]] QFuture< QPixmap > loadScaledPixmap(
			const QSize target_size,
			const SCALE_TYPE scale_type,
			const Alignment align_type,
			const std::filesystem::path& path );
		[[nodiscard]] QFuture< QPixmap > loadPixmap( const std::filesystem::path& path );
	} // namespace async
} // namespace atlas::images
#endif //ATLASGAMEMANAGER_LOADE_HPP
