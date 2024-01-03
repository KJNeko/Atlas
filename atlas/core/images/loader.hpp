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
	[[nodiscard]] QImage loadImage( const std::filesystem::path& path );
	[[nodiscard]] QPixmap loadPixmap( const std::filesystem::path& path );

} // namespace atlas::images
#endif //ATLASGAMEMANAGER_LOADE_HPP
