//
// Created by kj16609 on 9/26/23.
//

#ifndef ATLASGAMEMANAGER_BLURHASH_HPP
#define ATLASGAMEMANAGER_BLURHASH_HPP

#include <QPixmap>

#include <filesystem>

#include "blurhash/decode.h"
#include "blurhash/encode.h"

namespace atlas::images
{
	QPixmap getBlurhash( const std::filesystem::path&, const QSize size );
}

#endif //ATLASGAMEMANAGER_BLURHASH_HPP
