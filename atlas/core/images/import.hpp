//
// Created by kj16609 on 9/22/23.
//

#ifndef ATLASGAMEMANAGER_IMPORT_HPP
#define ATLASGAMEMANAGER_IMPORT_HPP

#include <QByteArray>
#include <QFuture>
#include <QPixmap>

#include <filesystem>

#include "core/Types.hpp"

namespace atlas::images
{
	/**
	 * @param source Source path of the image. Used for logging purposes.
	 * @param byteArray
	 * @param dest
	 */
	void saveImage( const QByteArray& byteArray, const std::filesystem::path& dest );

	[[nodiscard]] std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id );

	namespace async
	{
		//! Stores the image located at `path` in the data folder
		[[nodiscard]] QFuture< std::filesystem::path >
			importImage( const std::filesystem::path& path, const RecordID game_id );

	} // namespace async
} // namespace atlas::images

#endif //ATLASGAMEMANAGER_IMPORT_HPP
