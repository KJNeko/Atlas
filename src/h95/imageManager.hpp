//
// Created by kj16609 on 3/2/23.
//

#ifndef HYDRUS95_IMAGEMANAGER_HPP
#define HYDRUS95_IMAGEMANAGER_HPP

#include <filesystem>
#include <QSize>

namespace imageManager
{
	std::filesystem::path getBannerPath();
	std::filesystem::path getPreviewPath();

	void cleanOrphans();

	std::filesystem::path
		importImage( const std::filesystem::path& path, const std::filesystem::path& dest, bool delete_after );

	std::filesystem::path importBanner( const std::filesystem::path& path, bool delete_after = false );
	std::filesystem::path importPreview( const std::filesystem::path& path, bool delete_after = false );

	QPixmap fetchImage( const std::filesystem::path& path, const QSize size = {} );
}  // namespace imageManager


#endif	//HYDRUS95_IMAGEMANAGER_HPP
