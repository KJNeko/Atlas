//
// Created by kj16609 on 9/26/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_BLURHASH_HPP
#define ATLASGAMEMANAGER_BLURHASH_HPP

#include <QPixmap>

#include <filesystem>

namespace atlas::images
{
	std::string createBlurhash( const QPixmap& pixmap );
	std::string createBlurhash( QImage image );
	std::string createBlurhash( const std::filesystem::path& path );
	[[nodiscard]] QPixmap getBlurhash( const std::filesystem::path&, const QSize size );
} // namespace atlas::images

#endif //ATLASGAMEMANAGER_BLURHASH_HPP
