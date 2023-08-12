//
// Created by kj16609 on 6/2/23.
//

#ifndef ATLASGAMEMANAGER_IMPORTER_HPP
#define ATLASGAMEMANAGER_IMPORTER_HPP

#include <QFuture>
#include <QObject>
#include <QPromise>

#include <filesystem>

#include "core/Types.hpp"
#include "core/database/record/Game.hpp"

/**
 *
 * @param root Root path. Should be a FULL CANONICAL PATH
 * @param relative_executable Path to the executable. Should be under root as a RELATIVE path
 * @param title
 * @param creator
 * @param description
 * @param version
 * @param banners
 * @param previews
 * @param owning If true. The game will be moved to Atlas' game data directory.
 * @return
 */
QFuture< RecordID > importGame(
	std::filesystem::path root,
	std::filesystem::path relative_executable,
	QString title,
	QString creator,
	QString description,
	QString version,
	std::array< QString, BannerType::SENTINEL > banners,
	std::vector< QString > previews,
	bool owning = false,
	bool scan_filesize = false,
	AtlasID atlas_id = INVALID_ATLAS_ID,
	QThreadPool& pool = *QThreadPool::globalInstance() );

struct GameImportData;

/**
 *
 * @param data
 * @param root Should be populated with the folder containing the `root` for the `data` input. Leave empty if the `data` input is already a full path.
 * @param owning see `importGame`
 * @return
 */
QFuture< RecordID >
	importGame( GameImportData data, const std::filesystem::path root, const bool owning, const bool scan_filesize );
QFuture< RecordID > importGame(
	GameImportData data,
	const std::filesystem::path root,
	const bool owning,
	const bool scan_filesize,
	QThreadPool& pool );

#endif //ATLASGAMEMANAGER_IMPORTER_HPP
