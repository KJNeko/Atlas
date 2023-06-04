//
// Created by kj16609 on 6/2/23.
//

#ifndef ATLASGAMEMANAGER_IMPORTER_HPP
#define ATLASGAMEMANAGER_IMPORTER_HPP

#include <filesystem>

#include <QFuture>
#include <QObject>
#include <QPromise>

#include "core/Types.hpp"
#include "core/database/record/Record.hpp"

QFuture< RecordID > importGame(
	const std::filesystem::path root,
	const std::filesystem::path relative_executable,
	const QString title,
	const QString creator,
	const QString description,
	const QString version,
	const std::array< QString, BannerType::SENTINEL > banners,
	const std::vector< QString > previews,
	const bool owning = false );

#endif //ATLASGAMEMANAGER_IMPORTER_HPP
