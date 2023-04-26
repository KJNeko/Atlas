//
// Created by kj16609 on 4/14/23.
//

#ifndef ATLASGAMEMANAGER_QIMAGEBLUR_HPP
#define ATLASGAMEMANAGER_QIMAGEBLUR_HPP

#include <QImage>
#include <QPixmap>

#include "atlas/config.hpp"

QPixmap blurToSize(
	const QPixmap& pixmap,
	const int width,
	const int height,
	const int feather_radius,
	const int blur_radius,
	const BLUR_TYPE blur_type );

#endif //ATLASGAMEMANAGER_QIMAGEBLUR_HPP
