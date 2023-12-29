//
// Created by kj16609 on 4/14/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_QIMAGEBLUR_HPP
#define ATLASGAMEMANAGER_QIMAGEBLUR_HPP

#include <QImage>
#include <QPixmap>

#include "core/config/config.hpp"

namespace atlas::images
{

	QPixmap blurToSize(
		const QPixmap& pixmap,
		const int width,
		const int height,
		const int feather_radius,
		const int blur_radius,
		const BLUR_TYPE blur_type );

	inline QPixmap blurToSize(
		const QPixmap& pixmap,
		const QSize size,
		const int image_feather_radius,
		const int image_blur_radius,
		const BLUR_TYPE image_blur_type )
	{
		return blurToSize(
			pixmap, size.width(), size.height(), image_feather_radius, image_blur_radius, image_blur_type );
	}

} // namespace atlas::images
#endif //ATLASGAMEMANAGER_QIMAGEBLUR_HPP
